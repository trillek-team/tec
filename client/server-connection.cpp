#include "server-connection.hpp"
#include "event-system.hpp"
#include "events.hpp"
#include "game-state.hpp"

using asio::ip::tcp;

namespace tec {
namespace networking {
const std::string_view SERVER_PORT = "41228";
const std::string_view LOCAL_HOST = "127.0.0.1";

std::shared_ptr<spdlog::logger> ServerConnection::_log;
std::mutex ServerConnection::recent_ping_mutex;

ServerConnection::ServerConnection(ServerStats& s) : socket(io_context), stats(s) {
	_log = spdlog::get("console_log");
	RegisterMessageHandler(MessageType::SYNC, [this](const ServerMessage& message) { this->SyncHandler(message); });
	RegisterMessageHandler(MessageType::GAME_STATE_UPDATE, [this](const ServerMessage& message) {
		this->GameStateUpdateHandler(message);
	});
	RegisterMessageHandler(MessageType::CHAT_MESSAGE, [](const ServerMessage& message) {
		std::string msg(message.GetBodyPTR(), message.GetBodyLength());
		_log->info(msg);
	});
	RegisterMessageHandler(MessageType::CLIENT_ID, [this](const ServerMessage& message) {
		std::string id_message(message.GetBodyPTR(), message.GetBodyLength());
		this->client_id = std::atoi(id_message.c_str());
	});
	RegisterMessageHandler(MessageType::CLIENT_LEAVE, [](const ServerMessage& message) {
		std::string id_message(message.GetBodyPTR(), message.GetBodyLength());
		eid entity_id = std::atoi(id_message.c_str());
		_log->info("Entity {} left", entity_id);
		std::shared_ptr<EntityDestroyed> data = std::make_shared<EntityDestroyed>();
		data->entity_id = entity_id;
		EventSystem<EntityDestroyed>::Get()->Emit(data);
	});
	RegisterMessageHandler(MessageType::ENTITY_CREATE, [this](const ServerMessage&) {
		std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
		data->entity.ParseFromArray(current_read_msg.GetBodyPTR(), static_cast<int>(current_read_msg.GetBodyLength()));
		data->entity_id = data->entity.id();
		EventSystem<EntityCreated>::Get()->Emit(data);
	});
}

bool ServerConnection::Connect(std::string_view ip) {
	Disconnect();
	tcp::resolver resolver(this->io_context);
	tcp::resolver::query query(std::string(ip).data(), std::string(SERVER_PORT).data());
	asio::error_code error = asio::error::host_not_found;
	tcp::resolver::results_type endpoints = resolver.resolve(query);
	try {
		for (auto& point : endpoints) {
			this->socket.close();
			this->socket.connect(point, error);
			if (!error) {
				break;
			}
		}
		if (error) {
			this->socket.close();
			throw asio::system_error(error);
		}
	}
	catch (std::exception& e) {
		_log->error("Connect: {}", e.what());
		return false;
	}

	asio::ip::tcp::no_delay option(true);
	this->socket.set_option(option);

	if (this->onConnect) {
		this->onConnect();
	}
	read_header();

	return true;
}

void ServerConnection::Disconnect() {
	this->run_sync = false;
	this->socket.close();
	this->client_id = 0;
}

void ServerConnection::Stop() {
	this->run_dispatch = false;
	this->run_sync = false;
}

void ServerConnection::SendChatMessage(std::string message) {
	if (this->socket.is_open()) {
		ServerMessage msg;
		msg.SetBodyLength(message.size());
		memcpy(msg.GetBodyPTR(), message.c_str(), msg.GetBodyLength());
		msg.encode_header();
		Send(msg);
	}
}

void ServerConnection::Send(ServerMessage& msg) {
	try {
		asio::write(this->socket, asio::buffer(msg.GetDataPTR(), msg.length()));
	}
	catch (std::exception e) {
		_log->error("ServerConnection::Send(ServerMessage&): asio::write: {}", e.what());
		Disconnect();
	}
}

void ServerConnection::RegisterConnectFunc(std::function<void()> func) { this->onConnect = std::move(func); }

void ServerConnection::read_body() {
	asio::async_read(
			this->socket,
			asio::buffer(current_read_msg.GetBodyPTR(), current_read_msg.GetBodyLength()),
			[this](const asio::error_code& error, std::size_t) {
				if (error) {
					this->socket.close();
					throw asio::system_error(error, "read_body");
				}
				for (auto handler : this->message_handlers[current_read_msg.GetMessageType()]) {
					handler(current_read_msg);
				}
				read_header();
			});
}

void ServerConnection::read_header() {
	asio::async_read(
			this->socket,
			asio::buffer(this->current_read_msg.GetDataPTR(), ServerMessage::header_length),
			[this](const asio::error_code& error, std::size_t) {
				if (error) {
					this->socket.close();
					throw asio::system_error(error, "read_header");
				}
				this->recv_time = std::chrono::high_resolution_clock::now();
				if (this->current_read_msg.decode_header()) {
					read_body();
				}
			});
}

void ServerConnection::StartDispatch() {
	this->run_dispatch = true;
	while (this->run_dispatch) {
		try {
			this->io_context.run();
		}
		catch (std::exception e) {
			_log->error("ServerConnection asio exception: {}", e.what());
			Disconnect();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	_log->info("StartDispatch() is ending");
}

void ServerConnection::StartSync() {
	ServerMessage sync_msg;

	sync_msg.SetBodyLength(1);
	sync_msg.SetMessageType(MessageType::SYNC);
	sync_msg.encode_header();
	this->run_sync = true;
	while (this->run_sync) {
		if (this->client_id) {
			Send(sync_msg);
		}
		this->sync_start = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void ServerConnection::SyncHandler(const ServerMessage& message) {
	std::chrono::milliseconds round_trip =
			std::chrono::duration_cast<std::chrono::milliseconds>(recv_time - sync_start);
	std::lock_guard<std::mutex> recent_ping_lock(recent_ping_mutex);
	if (message.GetBodyLength() >= sizeof(uint64_t)) {
		memcpy(&this->stats.estimated_server_time, message.GetBodyPTR(), sizeof(uint64_t));
	}
	if (this->recent_pings.size() >= PING_HISTORY_SIZE) {
		this->recent_pings.pop_front();
	}
	this->recent_pings.push_back(round_trip.count() / 2);
	ping_time_t total_pings = 0;
	for (ping_time_t ping : this->recent_pings) {
		total_pings += ping;
	}
	this->average_ping = total_pings / PING_HISTORY_SIZE;
	this->stats.estimated_server_time += average_ping;
}

void ServerConnection::GameStateUpdateHandler(const ServerMessage& message) {
	proto::GameStateUpdate gsu;
	gsu.ParseFromArray(message.GetBodyPTR(), static_cast<int>(message.GetBodyLength()));
	state_id_t recv_state_id = gsu.state_id();
	if (recv_state_id <= this->last_received_state_id) {
		_log->warn("Received an older GameStateUpdate");
	}
	else {
		this->last_received_state_id = recv_state_id;
		GameState next_state;
		next_state.In(gsu);
		std::shared_ptr<NewGameStateEvent> new_game_state_msg = std::make_shared<NewGameStateEvent>();
		new_game_state_msg->new_state = std::move(next_state);
		EventSystem<NewGameStateEvent>::Get()->Emit(new_game_state_msg);
	}
}

} // namespace networking
} // namespace tec
