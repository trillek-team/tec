#include "client/server_connection.hpp"
#include "proto/game_state.pb.h"
#include "events.hpp"
#include "simulation.hpp"

using asio::ip::tcp;

namespace tec {
	namespace networking {
		const char* SERVER_PORT_STR = "41228";
		const char* LOCAL_HOST = "127.0.0.1";
		std::shared_ptr<spdlog::logger> ServerConnection::_log;
		std::mutex ServerConnection::recent_ping_mutex;

		ServerConnection::ServerConnection(Simulation& simulation) : simulation(simulation), socket(io_service), last_received_state_id(0) {
			_log = spdlog::get("console_log");
			RegisterMessageHandler(SYNC, [this] (const ServerMessage& message) {
				this->SyncHandler(message);
			});
			RegisterMessageHandler(GAME_STATE_UPDATE, [this] (const ServerMessage& message) {
				this->GameStateUpdateHandler(message);
			});
			RegisterMessageHandler(CHAT_MESSAGE, [ ] (const ServerMessage& message) {
				std::string msg(message.GetBodyPTR(), message.GetBodyLength());
				_log->info(msg);
			});
			RegisterMessageHandler(CLIENT_ID, [this] (const ServerMessage& message) {
				std::string id_message(message.GetBodyPTR(), message.GetBodyLength());
				client_id = std::atoi(id_message.c_str());
			});
			RegisterMessageHandler(ENTITY_CREATE, [this] (const ServerMessage& message) {
				proto::Entity entity;
				entity.ParseFromArray(current_read_msg.GetBodyPTR(), current_read_msg.GetBodyLength());
				this->simulation.SetEntityState(std::move(entity));
			});
		}

		bool ServerConnection::Connect(std::string ip) {
			this->client_id = 0;
			this->socket.close();
			tcp::resolver resolver(this->io_service);
			tcp::resolver::query query(ip, SERVER_PORT_STR);
			asio::error_code error = asio::error::host_not_found;
			tcp::resolver::iterator endpoint_iterator = resolver.resolve(query), end;
			try {
				while (error && endpoint_iterator != end) {
					this->socket.close();
					this->socket.connect(*endpoint_iterator++, error);
				}
				if (error) {
					this->socket.close();
					throw asio::system_error(error);
				}
			}
			catch (std::exception& e) {
				std::cerr << e.what() << std::endl;
				return false;
			}
			return true;
		}

		void ServerConnection::Disconnect() {
			this->socket.close();
			this->stopped = true;
		}

		void ServerConnection::Stop() {
			this->stopped = true;
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
			catch (std::exception) {
			}
		}

		void ServerConnection::read_body() {
			asio::error_code error = asio::error::eof;
			std::size_t len = asio::read(this->socket,
				asio::buffer(current_read_msg.GetBodyPTR(), current_read_msg.GetBodyLength()), error);

			if (!error) {
				this->message_handlers[current_read_msg.GetMessageType()](current_read_msg);
			}
			else if (error) {
				throw asio::system_error(error);
				this->socket.close();
			}
		}

		void ServerConnection::read_header() {
			asio::error_code error = asio::error::eof;
			std::size_t len = asio::read(this->socket,
				asio::buffer(this->current_read_msg.GetDataPTR(), ServerMessage::header_length), error);
			this->recv_time = std::chrono::high_resolution_clock::now();

			if (!error && this->current_read_msg.decode_header()) {
				read_body();
			}
			else if (error) {
				throw asio::system_error(error);
				this->socket.close();
			}
		}

		void ServerConnection::StartRead() {
			this->stopped = false;
			asio::error_code error = asio::error::eof;
			while (1) {
				try {
					if (this->socket.is_open() && this->socket.available()) {
						read_header();
					}
					if (this->stopped) {
						return;
					}
				}
				catch (std::exception) {
				}
			}
		}

		void ServerConnection::StartSync() {
			ServerMessage sync_msg;

			sync_msg.SetBodyLength(1);
			sync_msg.SetMessageType(SYNC);
			sync_msg.encode_header();
			while (1) {
				if (this->stopped) {
					return;
				}
				Send(sync_msg);
				this->sync_start = std::chrono::high_resolution_clock::now();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}

		void ServerConnection::SyncHandler(const ServerMessage& message) {
			std::chrono::milliseconds round_trip = std::chrono::duration_cast<std::chrono::milliseconds>(recv_time - sync_start);
			std::lock_guard<std::mutex> recent_ping_lock(recent_ping_mutex);
			if (this->recent_pings.size() >= 10) {
				this->recent_pings.pop_front();
			}
			this->recent_pings.push_back(round_trip.count() / 2);
			ping_time_t total_pings = 0;
			for (ping_time_t ping : this->recent_pings) {
				total_pings += ping;
			}
			this->average_ping = total_pings / 10;
		}

		void ServerConnection::GameStateUpdateHandler(const ServerMessage& message) {
			proto::GameStateUpdate gsu;
			gsu.ParseFromArray(message.GetBodyPTR(), message.GetBodyLength());
			state_id_t recv_state_id = gsu.state_id();
			if (recv_state_id <= this->last_received_state_id) {
				_log->warn("Received an older GameStateUpdate");
			}
			else {
				this->last_received_state_id = gsu.state_id();
				GameState next_state;
				next_state.In(gsu);
				next_state.positions.erase(this->client_id);
				next_state.orientations.erase(this->client_id);
				next_state.velocties.erase(this->client_id);
				this->simulation.onServerStateUpdate(std::move(next_state));
			}
			//_log->info(last_received_state_id);
		}
	}
}
