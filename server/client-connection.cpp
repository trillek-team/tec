#include "client-connection.hpp"

#include <iostream>
#include <thread>

#include <commands.pb.h>
#include <game_state.pb.h>

#include "client/graphics/view.hpp"
#include "components/collision-body.hpp"
#include "components/transforms.hpp"
#include "controllers/fps-controller.hpp"
#include "entity.hpp"
#include "event-system.hpp"
#include "events.hpp"
#include "server.hpp"

namespace tec {
namespace networking {

ClientConnection::ClientConnection(tcp::socket _socket, tcp::endpoint _endpoint, Server* server) :
		socket(std::move(_socket)), endpoint(std::move(_endpoint)), server(server) {
	current_read_msg = MessagePool::get();
}

ClientConnection::~ClientConnection() {
	std::shared_ptr<ControllerRemovedEvent> data = std::make_shared<ControllerRemovedEvent>();
	data->controller = this->controller;
	EventSystem<ControllerRemovedEvent>::Get()->Emit(data);
}
void ClientConnection::StartRead() { read_header(); }

void ClientConnection::QueueWrite(MessagePool::ptr_type msg) {
	bool write_in_progress;
	{
		std::lock_guard<std::mutex> lg(write_msg_mutex);
		write_in_progress = !write_msg_queue.empty();
		write_msg_queue.push_back(msg);
	}
	if (!write_in_progress) {
		do_write();
	}
}

void ClientConnection::QueueWrite(MessageOut& msg) {
	bool write_in_progress;
	{
		std::lock_guard<std::mutex> lg(write_msg_mutex);
		write_in_progress = !write_msg_queue.empty();
		for (auto& msg_ptr : msg.GetMessages()) {
			write_msg_queue.push_back(msg_ptr);
		}
	}
	if (!write_in_progress) {
		do_write();
	}
}

void ClientConnection::QueueWrite(MessageOut&& msg) { QueueWrite(msg); }

void ClientConnection::SetID(eid _id) {
	this->id = _id;
	this->entity.set_id(this->id);
	MessageOut id_message(MessageType::CLIENT_ID);
	id_message.FromString(std::to_string(this->id));
	QueueWrite(std::move(id_message));
}

void ClientConnection::DoJoin() {
	// Build an entity
	Entity self(this->id);
	self.Add<Position, Orientation, Velocity, View>(glm::vec3(5, 1.0, 5), glm::vec3(.5f, 0.f, 0.f), Velocity(), true);
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

	MessageOut entity_create_msg(MessageType::ENTITY_CREATE);
	this->entity.SerializeToZeroCopyStream(&entity_create_msg);
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
	MessageOut leave_msg(MessageType::CLIENT_LEAVE);
	leave_msg.FromString(std::to_string(this->id));
	this->server->Deliver(leave_msg, false);
	std::shared_ptr<EntityDestroyed> data = std::make_shared<EntityDestroyed>();
	data->entity_id = this->id;
	EventSystem<EntityDestroyed>::Get()->Emit(data);
}

void ClientConnection::OnClientLeave(eid entity_id) {
	if (this->state_changes_since_confirmed.positions.find(entity_id)
		!= this->state_changes_since_confirmed.positions.end()) {
		this->state_changes_since_confirmed.positions.erase(entity_id);
	}
	if (this->state_changes_since_confirmed.orientations.find(entity_id)
		!= this->state_changes_since_confirmed.orientations.end()) {
		this->state_changes_since_confirmed.orientations.erase(entity_id);
	}
	if (this->state_changes_since_confirmed.velocities.find(entity_id)
		!= this->state_changes_since_confirmed.velocities.end()) {
		this->state_changes_since_confirmed.velocities.erase(entity_id);
	}
}

void ClientConnection::read_header() {
	auto self(shared_from_this());
	asio::async_read(
			this->socket,
			this->current_read_msg->buffer_header(),
			[this, self](std::error_code error, std::size_t /*length*/) {
				if (!error && current_read_msg->decode_header()) {
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
			this->socket,
			this->current_read_msg->buffer_body(),
			[this, self](std::error_code error, std::size_t /*length*/) {
				if (error) {
					server->Leave(shared_from_this());
					return;
				}
				auto _log = spdlog::get("console_log");

				MessagePool::ptr_type last_read_msg = current_read_msg;
				current_read_msg = MessagePool::get();
				uint32_t current_msg_id = last_read_msg->GetMessageID();
				uint32_t current_msg_seq = last_read_msg->GetSequence();
				auto message_iter = read_messages.find(current_msg_id);

				if (last_read_msg->GetMessageType() == MessageType::MULTI_PART) {
					if (message_iter == read_messages.cend()) {
						auto message_in = std::make_unique<MessageIn>();
						message_in->PushMessage(last_read_msg);
						read_messages[current_msg_id] = std::move(message_in);
					}
					else {
						message_iter->second->PushMessage(last_read_msg);
					}
				}
				else {
					// a single use MessageIn object for single fragment messages
					MessageIn short_message_in;
					MessageIn* message_in; // pointer to the message we use
					if (message_iter == read_messages.cend()) {
						message_in = &short_message_in;
					}
					else {
						message_in = message_iter->second.get();
					}
					message_in->PushMessage(last_read_msg);

					if (message_in->DecodeMessages()) {
						process_message(*message_in);
					}
					else {
						_log->warn(
								"ClientConnection read an invalid message sequence seq={} id={}",
								current_msg_seq,
								current_msg_id);
					}
					// drop it after processing
					if (message_iter != read_messages.cend()) {
						read_messages.erase(message_iter);
					}
				}

				read_header();
			});
}

void ClientConnection::process_message(MessageIn& msg) {
	auto _log = spdlog::get("console_log");
	auto now_time = std::chrono::high_resolution_clock::now();
	uint64_t current_timestamp =
			std::chrono::duration_cast<std::chrono::milliseconds>(now_time.time_since_epoch()).count();

	switch (msg.GetMessageType()) {
	case MessageType::CHAT_MESSAGE:
		server->Deliver(msg.ToOut());
		_log->info("[CHAT] {}", msg.ToString());
		break;
	case MessageType::SYNC:
	{
		auto sync_response = MessagePool::get();
		sync_response->SetMessageType(MessageType::SYNC);
		sync_response->SetBodyLength(sizeof(uint64_t));
		memcpy(sync_response->GetBodyPTR(), &current_timestamp, sizeof(uint64_t));
		sync_response->encode_header();
		QueueWrite(std::move(sync_response));
		break;
	}
	case MessageType::CLIENT_COMMAND:
	{
		// Pass this along to be handled in simulation to allow for
		// processing of string commands as well as movement.

		// TODO: just apply movement commands here and split string commands
		// to a different message.
		proto::ClientCommands proto_client_commands;
		proto_client_commands.ParseFromZeroCopyStream(&msg);
		if (proto_client_commands.has_laststateid()) {
			this->last_confirmed_state_id = proto_client_commands.laststateid();
		}
		this->last_recv_command_id = proto_client_commands.commandid();
		std::shared_ptr<ClientCommandsEvent> data = std::make_shared<ClientCommandsEvent>();
		data->client_commands = std::move(proto_client_commands);
		EventSystem<ClientCommandsEvent>::Get()->Emit(data);
		break;
	}
	case MessageType::CHAT_COMMAND:
	{
		proto::ChatCommand chat_command;
		chat_command.ParseFromZeroCopyStream(&msg);
		EventSystem<ChatCommandEvent>::Get()->Emit(std::make_shared<ChatCommandEvent>(chat_command));
		break;
	}
	case MessageType::ENTITY_CREATE:
	case MessageType::ENTITY_DESTROY:
	case MessageType::CLIENT_JOIN:
	case MessageType::CLIENT_ID:
	case MessageType::CLIENT_LEAVE:
	case MessageType::GAME_STATE_UPDATE:
	default: break;
	}
}

void ClientConnection::do_write() {
	auto self(shared_from_this());
	std::lock_guard<std::mutex> lg(write_msg_mutex);
	auto msg_ptr = write_msg_queue.front().get();
	asio::async_write(this->socket, msg_ptr->buffer(), [this, self](std::error_code error, std::size_t /*length*/) {
		if (error) {
			server->Leave(shared_from_this());
			return;
		}
		bool more_to_write = false;
		{
			std::lock_guard<std::mutex> lg(write_msg_mutex);
			write_msg_queue.pop_front();
			more_to_write = !write_msg_queue.empty();
		}
		if (more_to_write) {
			do_write();
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

MessageOut ClientConnection::PrepareGameStateUpdateMessage(state_id_t current_state_id, uint64_t current_timestamp) {
	tec::proto::GameStateUpdate gsu_msg;
	gsu_msg.set_state_id(current_state_id);
	gsu_msg.set_command_id(this->last_recv_command_id);
	gsu_msg.set_timestamp(current_timestamp);
	for (auto pos : this->state_changes_since_confirmed.positions) {
		tec::proto::Entity* _entity = gsu_msg.add_entity();
		_entity->set_id(pos.first);
		pos.second.Out(_entity->add_components());
		if (this->state_changes_since_confirmed.orientations.find(pos.first)
			!= this->state_changes_since_confirmed.orientations.end()) {
			tec::Orientation ori = this->state_changes_since_confirmed.orientations.at(pos.first);
			ori.Out(_entity->add_components());
		}
		if (this->state_changes_since_confirmed.velocities.find(pos.first)
			!= this->state_changes_since_confirmed.velocities.end()) {
			tec::Velocity vel = this->state_changes_since_confirmed.velocities.at(pos.first);
			vel.Out(_entity->add_components());
		}
	}
	MessageOut update_message(MessageType::GAME_STATE_UPDATE);
	gsu_msg.SerializeToZeroCopyStream(&update_message);
	return update_message;
}
} // namespace networking
} // namespace tec
