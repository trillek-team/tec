#include "client-connection.hpp"

#include <iostream>
#include <thread>

#include <commands.pb.h>
#include <game_state.pb.h>

#include "event-system.hpp"
#include "events.hpp"
#include "server.hpp"

namespace tec {
eid GetNextEntityId();

namespace networking {
ClientConnection::ClientConnection(tcp::socket _socket, tcp::endpoint _endpoint, Server* server) :
		socket(std::move(_socket)), endpoint(std::move(_endpoint)), server(server) {
	current_read_msg = MessagePool::get();
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

void ClientConnection::OnJoinWorld() {
	this->server->GetLuaSystem()->CallFunctions("onClientJoin", this);
	eid entity_id = this->user->GetEntityId();
	// Send this clients entity id
	MessageOut id_message(MessageType::CLIENT_ID);
	id_message.FromString(std::to_string(entity_id));
	QueueWrite(std::move(id_message));

	// Inform other clients
	MessageOut join_msg(MessageType::CLIENT_JOIN);
	join_msg.FromString(std::to_string(entity_id));
	this->server->Deliver(join_msg, false);
}

void ClientConnection::OnLeaveWorld() {
	this->server->GetLuaSystem()->CallFunctions("onClientLeave", this);
	// Inform other clients
	eid entity_id = this->user->GetEntityId();
	MessageOut leave_msg(MessageType::CLIENT_LEAVE);
	leave_msg.FromString(std::to_string(entity_id));
	this->server->Deliver(leave_msg, false);
}

void ClientConnection::OnOtherLeaveWorld(eid entity_id) {
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
					server->OnDisconnect(shared_from_this());
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
					server->OnDisconnect(shared_from_this());
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
			auto last_state_id = proto_client_commands.laststateid();
			if (last_state_id != 0) {
				this->last_confirmed_state_id = proto_client_commands.laststateid();
			}
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
	case MessageType::LOGIN:
	{
		proto::UserLogin user_login;
		user_login.ParseFromZeroCopyStream(&msg);
		this->server->GetLuaSystem()->CallFunctions("onUserLogin", this, user_login.username());

		this->server->SendWorld(shared_from_this());

		this->OnJoinWorld();
		this->user->AddEntityToWorld();

		break;
	}
	case MessageType::CLIENT_READY_TO_RECEIVE:
	{
		this->ready_to_recv_states = true;
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
			server->OnDisconnect(shared_from_this());
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
	for (const auto& pair : full_state.positions) {
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
void ClientConnection::RegisterLuaType(sol::state& state) {
	// clang-format off
	state.new_usertype<ClientConnection>(
			"ClientConnection", sol::no_constructor,
			"user", &ClientConnection::user
		);
	// clang-format on
}
} // namespace networking
} // namespace tec
