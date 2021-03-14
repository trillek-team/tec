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
	this->current_read_msg = MessagePool::get();
	RegisterMessageHandler(
			MessageType::GAME_STATE_UPDATE, [this](MessageIn& message) { this->GameStateUpdateHandler(message); });
	RegisterMessageHandler(MessageType::CHAT_MESSAGE, [](MessageIn& message) {
		std::string msg = message.ToString();
		_log->info(msg);
	});
	RegisterMessageHandler(MessageType::CLIENT_ID, [this](MessageIn& message) {
		std::string id_message = message.ToString();
		this->client_id = std::atoi(id_message.c_str());
	});
	RegisterMessageHandler(MessageType::CLIENT_LEAVE, [](MessageIn& message) {
		std::string id_message = message.ToString();
		eid entity_id = std::atoi(id_message.c_str());
		_log->info("Entity {} left", entity_id);
		std::shared_ptr<EntityDestroyed> data = std::make_shared<EntityDestroyed>();
		data->entity_id = entity_id;
		EventSystem<EntityDestroyed>::Get()->Emit(data);
	});
	RegisterMessageHandler(MessageType::ENTITY_CREATE, [this](MessageIn& message) {
		std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
		data->entity.ParseFromZeroCopyStream(&message);
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
		MessageOut msg;
		msg.FromString(message);
		Send(std::move(msg));
	}
}

void ServerConnection::Send(Message::ptr_type msg) {
	bool write_in_progress;
	try {
		std::lock_guard<std::mutex> guard(write_msg_mutex);
		write_in_progress = !write_msg_queue.empty();
		write_msg_queue.push_back(msg);
	}
	catch (std::exception& e) {
		_log->error("ServerConnection::Send(): {}", e.what());
		Disconnect();
		return;
	}
	if (!write_in_progress) {
		do_write();
	}
}
void ServerConnection::Send(MessageOut& msg) {
	bool write_in_progress;
	try {
		std::lock_guard<std::mutex> guard(write_msg_mutex);
		write_in_progress = !write_msg_queue.empty();
		for (auto& msg_ptr : msg.GetMessages()) {
			write_msg_queue.push_back(msg_ptr);
		}
	}
	catch (std::exception& e) {
		_log->error("ServerConnection::Send(): {}", e.what());
		Disconnect();
		return;
	}
	if (!write_in_progress) {
		do_write();
	}
}
void ServerConnection::Send(MessageOut&& msg) { Send(msg); }

void ServerConnection::RegisterConnectFunc(std::function<void()> func) { this->onConnect = std::move(func); }

void ServerConnection::read_body() {
	asio::async_read(
			this->socket,
			this->current_read_msg->buffer_body(),
			[this](const asio::error_code& error, std::size_t length) {
				if (error) {
					this->socket.close();
					throw asio::system_error(error, "read_body");
				}
				uint32_t current_msg_id = current_read_msg->GetMessageID();
				uint32_t current_msg_seq = current_read_msg->GetSequence();
				auto message_iter = read_messages.find(current_msg_id);
				std::unique_ptr<MessageIn> message_in;
				switch (current_read_msg->GetMessageType()) {
				case MessageType::SYNC: this->SyncHandler(current_read_msg.get()); break;
				case MessageType::MULTI_PART:
					if (message_iter == read_messages.cend()) {
						message_in = std::make_unique<MessageIn>();
						message_in->PushMessage(current_read_msg);
						read_messages[current_msg_id] = std::move(message_in);
						current_read_msg = MessagePool::get();
					}
					break;
				default:
					if (message_iter == read_messages.cend()) {
						message_in = std::make_unique<MessageIn>();
						message_in->PushMessage(current_read_msg);
					}
					else {
						message_iter->second->PushMessage(current_read_msg);
					}
					if (message_in) {
						if (message_in->DecodeMessages()) {
							for (auto handler : this->message_handlers[message_in->GetMessageType()]) {
								handler(*message_in);
							}
							if (message_iter != read_messages.cend()) {
								read_messages.erase(message_iter);
							}
						}
						else {
							_log->warn(
									"ServerConnection read an invalid message sequence seq={} id={}",
									current_msg_seq,
									current_msg_id);
							if (message_iter != read_messages.cend()) {
								read_messages.erase(message_iter);
							}
						}
					}
					break;
				}
				read_header();
			});
}

void ServerConnection::read_header() {
	asio::async_read(
			this->socket,
			this->current_read_msg->buffer_header(),
			[this](const asio::error_code& error, std::size_t length) {
				if (error) {
					this->socket.close();
					throw asio::system_error(error, "read_header");
				}
				this->recv_time = std::chrono::high_resolution_clock::now();
				if (this->current_read_msg->decode_header()) {
					read_body();
				}
			});
}

void ServerConnection::do_write() {
	std::lock_guard<std::mutex> guard(write_msg_mutex);
	auto msg_ptr = write_msg_queue.front().get();
	asio::async_write(
			this->socket,
			asio::buffer(msg_ptr->GetDataPTR(), msg_ptr->length()),
			[this](std::error_code error, std::size_t length) {
				if (error) {
					throw asio::system_error(error, "do_write");
				}
				bool more_to_write = false;
				{
					std::lock_guard<std::mutex> guard(write_msg_mutex);
					write_msg_queue.pop_front();
					more_to_write = !write_msg_queue.empty();
				}
				if (more_to_write) {
					do_write();
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
	auto sync_msg = MessagePool::get();
	sync_msg->SetBodyLength(1);
	sync_msg->SetMessageType(MessageType::SYNC);
	sync_msg->encode_header();

	this->run_sync = true;
	while (this->run_sync) {
		if (this->client_id) {
			Send(sync_msg);
		}
		this->sync_start = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void ServerConnection::SyncHandler(Message::cptr_type message) {
	std::chrono::milliseconds round_trip =
			std::chrono::duration_cast<std::chrono::milliseconds>(recv_time - sync_start);
	std::lock_guard<std::mutex> recent_ping_lock(recent_ping_mutex);
	if (message->GetBodyLength() >= sizeof(uint64_t)) {
		memcpy(&this->stats.estimated_server_time, message->GetBodyPTR(), sizeof(uint64_t));
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

void ServerConnection::GameStateUpdateHandler(MessageIn& message) {
	proto::GameStateUpdate gsu;
	gsu.ParseFromZeroCopyStream(&message);
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
