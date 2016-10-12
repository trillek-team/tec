// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "server/client-connection.hpp"
#include "proto/game_state.pb.h"
#include "event-system.hpp"
#include "events.hpp"
#include "server/server.hpp"
#include "entity.hpp"
#include "components/transforms.hpp"
#include "client/graphics/view.hpp"
#include "components/collision-body.hpp"
#include <iostream>
#include <thread>

namespace tec {
	namespace networking {
		std::mutex ClientConnection::write_msg_mutex;
		void ClientConnection::StartRead() {
			read_header();
		}

		void ClientConnection::QueueWrite(const ServerMessage& msg) {
			bool write_in_progress;
			{
				std::lock_guard<std::mutex> lock(write_msg_mutex);
				write_in_progress = !write_msgs_.empty();
				write_msgs_.push_back(msg);
			}
			if (!write_in_progress) {
				do_write();
			}
		}

		void ClientConnection::SetID(eid id) {
			this->id = id;
			this->entity.set_id(id);
			std::string message(std::to_string(this->id));
			static ServerMessage id_message;
			id_message.SetMessageType(CLIENT_ID);
			id_message.SetBodyLength(message.size());
			memcpy(id_message.GetBodyPTR(), message.c_str(), id_message.GetBodyLength());
			id_message.encode_header();
			QueueWrite(id_message);
		}

		void ClientConnection::DoJoin() {
			// Build an entity
			Entity self(this->id);
			self.Add<Position, Orientation, Velocity, View>(glm::vec3(0,1.0,0), Orientation(), Velocity(), true);
			CollisionBody* body = new CollisionBody();
			proto::Component* component = this->entity.add_components();
			proto::CollisionBody* body_component = component->mutable_collision_body();
			body_component->set_mass(1.0f);
			body_component->set_disable_deactivation(true);
			body_component->set_disable_rotation(true);
			proto::CollisionBody_Capsule* capsule_component = body_component->mutable_capsule();
			capsule_component->set_height(1.6f);
			capsule_component->set_radius(0.5f);
			body->entity_id = this->id;
			body->In(*component);
			Multiton<eid, CollisionBody*>::Set(this->id, body);
			self.Out<Position, Orientation, Velocity, View>(this->entity);
					
			ServerMessage entity_create_msg;
			entity_create_msg.SetBodyLength(this->entity.ByteSize());
			this->entity.SerializeToArray(entity_create_msg.GetBodyPTR(), entity_create_msg.GetBodyLength());
			entity_create_msg.SetMessageType(ENTITY_CREATE);
			entity_create_msg.encode_header();
			QueueWrite(entity_create_msg);
			std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
			data->entity = this->entity;
			data->entity_id = this->entity.id();
			EventSystem<EntityCreated>::Get()->Emit(data);
		}

		void ClientConnection::DoLeave() {
			static ServerMessage leave_msg;
			leave_msg.SetMessageType(CLIENT_LEAVE);
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
			asio::async_read(socket,
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
			asio::async_read(socket,
				asio::buffer(this->current_read_msg.GetBodyPTR(), this->current_read_msg.GetBodyLength()),
				[this, self] (std::error_code error, std::size_t /*length*/) {
				if (!error) {
					switch (this->current_read_msg.GetMessageType()) {
						case CHAT_MESSAGE:
							server->Deliver(this->current_read_msg);
							std::cout.write(this->current_read_msg.GetBodyPTR(),
								this->current_read_msg.GetBodyLength()) << std::endl;
							break;
						case SYNC:
							QueueWrite(this->current_read_msg);
							break;
						case ENTITY_UPDATE:
							this->entity.ParseFromArray(current_read_msg.GetBodyPTR(),
								current_read_msg.GetBodyLength());
							this->last_confirmed_state_id = current_read_msg.GetStateID();
							std::shared_ptr<EntityUpdated> data = std::make_shared<EntityUpdated>();
							data->entity = this->entity;
							EventSystem<EntityUpdated>::Get()->Emit(data);
							break;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					read_header();
				}
				else {
					server->Leave(shared_from_this());
				}
			});
		}

		void ClientConnection::do_write() {
			auto self(shared_from_this());
			std::lock_guard<std::mutex> lock(write_msg_mutex);
			asio::async_write(socket,
				asio::buffer(write_msgs_.front().GetDataPTR(),
				write_msgs_.front().length()),
				[this, self] (std::error_code error, std::size_t /*length*/) {
				if (!error) {
					bool more_to_write = false;
					{
						std::lock_guard<std::mutex> lock(write_msg_mutex);
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

		void ClientConnection::UpdateGameState(std::set<eid> updated_entities, const GameState& full_state) {
			for (eid entity : updated_entities) {
				if (full_state.positions.find(entity) != full_state.positions.end()) {
					this->state_changes_since_confirmed.positions[entity] = full_state.positions.at(entity);
				}
				if (full_state.orientations.find(entity) != full_state.orientations.end()) {
					this->state_changes_since_confirmed.orientations[entity] = full_state.orientations.at(entity);
				}
				if (full_state.velocities.find(entity) != full_state.velocities.end()) {
					this->state_changes_since_confirmed.velocities[entity] = full_state.velocities.at(entity);
				}
			}
		}

		tec::networking::ServerMessage ClientConnection::PrepareGameStateUpdateMessage(state_id_t current_state_id) {
			tec::proto::GameStateUpdate gsu_msg;
			gsu_msg.set_state_id(current_state_id);
			for (auto pos : this->state_changes_since_confirmed.positions) {
				tec::proto::Entity* entity = gsu_msg.add_entity();
				entity->set_id(pos.first);
				pos.second.Out(entity->add_components());
				if (this->state_changes_since_confirmed.orientations.find(pos.first) != this->state_changes_since_confirmed.orientations.end()) {
					tec::Orientation ori = this->state_changes_since_confirmed.orientations.at(pos.first);
					ori.Out(entity->add_components());
				}
				if (this->state_changes_since_confirmed.velocities.find(pos.first) != this->state_changes_since_confirmed.velocities.end()) {
					tec::Velocity vel = this->state_changes_since_confirmed.velocities.at(pos.first);
					vel.Out(entity->add_components());
				}
			}
			tec::networking::ServerMessage update_message;
			update_message.SetMessageType(tec::networking::GAME_STATE_UPDATE);
			update_message.SetBodyLength(gsu_msg.ByteSize());
			gsu_msg.SerializeToArray(update_message.GetBodyPTR(), update_message.GetBodyLength());
			update_message.encode_header();
			return std::move(update_message);
		}
	}
}
