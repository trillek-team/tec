#include "client-connection.hpp"

#include <iostream>
#include <thread>

#include <game_state.pb.h>
#include <commands.pb.h>

#include "event-system.hpp"
#include "events.hpp"
#include "server.hpp"
#include "entity.hpp"
#include "components/transforms.hpp"
#include "controllers/fps-controller.hpp"
#include "client/graphics/view.hpp"
#include "components/collision-body.hpp"

namespace tec {
	namespace networking {
		std::mutex ClientConnection::write_msg_mutex;
		ClientConnection::~ClientConnection() {
			std::shared_ptr<ControllerRemovedEvent> data = std::make_shared<ControllerRemovedEvent>();
			data->controller = this->controller;
			EventSystem<ControllerRemovedEvent>::Get()->Emit(data);
		}
		void ClientConnection::StartRead() {
			read_header();
		}

		void ClientConnection::QueueWrite(const ServerMessage& msg) {
			bool write_in_progress;
			{
				std::lock_guard<std::mutex> lg(write_msg_mutex);
				write_in_progress = !write_msgs_.empty();
				write_msgs_.push_back(msg);
			}
			if (!write_in_progress) {
				do_write();
			}
		}

		void ClientConnection::SetID(eid _id) {
			this->id = _id;
			this->entity.set_id(this->id);
			std::string message(std::to_string(this->id));
			static ServerMessage id_message;
			id_message.SetMessageType(MessageType::CLIENT_ID);
			id_message.SetBodyLength(message.size());
			memcpy(id_message.GetBodyPTR(), message.c_str(), id_message.GetBodyLength());
			id_message.encode_header();
			QueueWrite(id_message);
		}

		void ClientConnection::DoJoin() {
			// Build an entity
			Entity self(this->id);
			self.Add<Position, Orientation, Velocity, View>(glm::vec3(5, 1.0, 5), glm::vec3(0.0f, 0.0f, 0.0f), Velocity(), true);
			CollisionBody* body = new CollisionBody();
			proto::Component* component = this->entity.add_components();
			proto::CollisionBody* body_component = component->mutable_collision_body();
			body_component->set_mass(10.0f);
			body_component->set_disable_deactivation(true);
			body_component->set_disable_rotation(true);
			proto::CollisionBody_Capsule* capsule_component = body_component->mutable_capsule();
			capsule_component->set_height(1.6f);
			capsule_component->set_radius(0.5f);
			body->entity_id = this->id;
			body->In(*component);
			Multiton<eid, CollisionBody*>::Set(this->id, body);
			self.Out<Position, Orientation, Velocity, View, CollisionBody>(this->entity);

			ServerMessage entity_create_msg;
			entity_create_msg.SetBodyLength(this->entity.ByteSizeLong());
			this->entity.SerializeToArray(entity_create_msg.GetBodyPTR(), static_cast<int>(entity_create_msg.GetBodyLength()));
			entity_create_msg.SetMessageType(MessageType::ENTITY_CREATE);
			entity_create_msg.encode_header();
			QueueWrite(entity_create_msg);
			std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
			data->entity = this->entity;
			data->entity_id = this->entity.id();
			EventSystem<EntityCreated>::Get()->Emit(data);

			this->controller = std::make_shared<FPSController>(data->entity_id);
			std::shared_ptr<ControllerAddedEvent> dataX = std::make_shared<ControllerAddedEvent>();
			dataX->controller = controller;
			EventSystem<ControllerAddedEvent>::Get()->Emit(dataX);
		}

		void ClientConnection::DoLeave() {
			static ServerMessage leave_msg;
			leave_msg.SetMessageType(MessageType::CLIENT_LEAVE);
			std::string message(std::to_string(this->id));
			leave_msg.SetBodyLength(message.size());
			memcpy(leave_msg.GetBodyPTR(), message.c_str(), leave_msg.GetBodyLength());
			leave_msg.encode_header();
			this->server->Deliver(leave_msg, false);
			std::shared_ptr<EntityDestroyed> data = std::make_shared<EntityDestroyed>();
			data->entity_id = this->id;
			EventSystem<EntityDestroyed>::Get()->Emit(data);
		}

		void ClientConnection::OnClientLeave(eid entity_id) {
			if (this->state_changes_since_confirmed.positions.find(entity_id) != this->state_changes_since_confirmed.positions.end()) {
				this->state_changes_since_confirmed.positions.erase(entity_id);
			}
			if (this->state_changes_since_confirmed.orientations.find(entity_id) != this->state_changes_since_confirmed.orientations.end()) {
				this->state_changes_since_confirmed.orientations.erase(entity_id);
			}
			if (this->state_changes_since_confirmed.velocities.find(entity_id) != this->state_changes_since_confirmed.velocities.end()) {
				this->state_changes_since_confirmed.velocities.erase(entity_id);
			}
		}

		void ClientConnection::read_header() {
			auto self(shared_from_this());
			asio::async_read(
				socket,
				asio::buffer(current_read_msg.GetDataPTR(), ServerMessage::header_length),
				[this, self] (std::error_code error, std::size_t /*length*/) {
					if (!error && current_read_msg.decode_header()) {
						read_body();
					}
					else {
						server->Leave(shared_from_this());
					}
				});
		}

		void ClientConnection::read_body() {
			auto self(shared_from_this());
			asio::async_read(
				socket,
				asio::buffer(this->current_read_msg.GetBodyPTR(), this->current_read_msg.GetBodyLength()),
				[this, self] (std::error_code error, std::size_t /*length*/) {
					auto now_time = std::chrono::high_resolution_clock::now();
					uint64_t current_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now_time.time_since_epoch()).count();
					if (error) {
						server->Leave(shared_from_this());
						return;
					}
					switch (this->current_read_msg.GetMessageType()) {
					case MessageType::CHAT_MESSAGE:
						server->Deliver(this->current_read_msg);
						std::cout.write(this->current_read_msg.GetBodyPTR(),
										this->current_read_msg.GetBodyLength()) << std::endl;
						break;
					case MessageType::SYNC:
					{
						ServerMessage sync_response;
						sync_response.SetMessageType(MessageType::SYNC);
						sync_response.SetBodyLength(8);
						memcpy(sync_response.GetBodyPTR(), &current_timestamp, 8);
						sync_response.encode_header();
						QueueWrite(sync_response);
						break;
					}
					case MessageType::CLIENT_COMMAND:
					{
						// Pass this along to be handled in simulation to allow for
						// processing of string commands as well as movement.

						// TODO: just apply movement commands here and split string commands
						// to a different message.
						proto::ClientCommands proto_client_commands;
						proto_client_commands.ParseFromArray(
							current_read_msg.GetBodyPTR(),
							static_cast<int>(current_read_msg.GetBodyLength()));
						this->last_confirmed_state_id = current_read_msg.GetStateID();
						std::shared_ptr<ClientCommandsEvent> data = std::make_shared<ClientCommandsEvent>();
						this->last_recv_command_id = proto_client_commands.commandid();
						data->client_commands = std::move(proto_client_commands);
						EventSystem<ClientCommandsEvent>::Get()->Emit(data);
						break;
					}
					case MessageType::ENTITY_CREATE:
					case MessageType::ENTITY_DESTROY:
					case MessageType::CLIENT_JOIN:
					case MessageType::CLIENT_ID:
					case MessageType::CLIENT_LEAVE:
					case MessageType::GAME_STATE_UPDATE:
						break;
					}
					read_header();
				});
		}

		void ClientConnection::do_write() {
			auto self(shared_from_this());
			std::lock_guard<std::mutex> lg(write_msg_mutex);
			asio::async_write(
				socket,
				asio::buffer(write_msgs_.front().GetDataPTR(),
							 write_msgs_.front().length()),
				[this, self] (std::error_code error, std::size_t /*length*/) {
					if (!error) {
						bool more_to_write = false;
						{
							std::lock_guard<std::mutex> lg(write_msg_mutex);
							write_msgs_.pop_front();
							more_to_write = !write_msgs_.empty();
						}
						if (more_to_write) {
							do_write();
						}
					}
					else {
						server->Leave(shared_from_this());
					}
				});
		}

		void ClientConnection::UpdateGameState(const GameState& full_state) {
			for (auto pair : full_state.positions) {
				if (full_state.positions.find(pair.first) != full_state.positions.end()) {
					this->state_changes_since_confirmed.positions[pair.first] = full_state.positions.at(pair.first);
				}
				if (full_state.orientations.find(pair.first) != full_state.orientations.end()) {
					this->state_changes_since_confirmed.orientations[pair.first] = full_state.orientations.at(pair.first);
				}
				if (full_state.velocities.find(pair.first) != full_state.velocities.end()) {
					this->state_changes_since_confirmed.velocities[pair.first] = full_state.velocities.at(pair.first);
				}
			}
		}

		tec::networking::ServerMessage ClientConnection::PrepareGameStateUpdateMessage(state_id_t current_state_id, uint64_t current_timestamp) {
			tec::proto::GameStateUpdate gsu_msg;
			gsu_msg.set_state_id(current_state_id);
			gsu_msg.set_command_id(this->last_recv_command_id);
			gsu_msg.set_timestamp(current_timestamp);
			for (auto pos : this->state_changes_since_confirmed.positions) {
				tec::proto::Entity* _entity = gsu_msg.add_entity();
				_entity->set_id(pos.first);
				pos.second.Out(_entity->add_components());
				if (this->state_changes_since_confirmed.orientations.find(pos.first) != this->state_changes_since_confirmed.orientations.end()) {
					tec::Orientation ori = this->state_changes_since_confirmed.orientations.at(pos.first);
					ori.Out(_entity->add_components());
				}
				if (this->state_changes_since_confirmed.velocities.find(pos.first) != this->state_changes_since_confirmed.velocities.end()) {
					tec::Velocity vel = this->state_changes_since_confirmed.velocities.at(pos.first);
					vel.Out(_entity->add_components());
				}
			}
			tec::networking::ServerMessage update_message;
			update_message.SetMessageType(tec::networking::MessageType::GAME_STATE_UPDATE);
			update_message.SetBodyLength(gsu_msg.ByteSizeLong());
			gsu_msg.SerializeToArray(update_message.GetBodyPTR(), static_cast<int>(update_message.GetBodyLength()));
			update_message.encode_header();
			return update_message;
		}
	}
}
