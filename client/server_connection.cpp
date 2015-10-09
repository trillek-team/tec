#include "client/server_connection.hpp"
#include "events.hpp"

using asio::ip::tcp;

namespace tec {
	namespace networking {
		const char* SERVER_PORT_STR = "41228";
		const char* LOCAL_HOST = "127.0.0.1";
		std::shared_ptr<spdlog::logger> ServerConnection::_log;

		ServerConnection::ServerConnection() : socket(io_service) {
			_log = spdlog::get("console_log");
		}

		bool ServerConnection::Connect(std::string ip) {
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
		}

		void ServerConnection::StopRead() {
			this->stopped = true;
		}

		void ServerConnection::Write(std::string message) {
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
			catch (std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		}

		void ServerConnection::read_body() {
			asio::error_code error = asio::error::eof;
			std::size_t len = asio::read(this->socket,
				asio::buffer(current_read_msg.GetBodyPTR(), current_read_msg.GetBodyLength()), error);

			if (!error) {
				if (!current_read_msg.EntityUpdateMessage()) {
					std::string msg(current_read_msg.GetBodyPTR(), current_read_msg.GetBodyLength());
					_log->info(msg);
				}
				else {
					proto::Entity entity;
					entity.ParseFromArray(current_read_msg.GetBodyPTR(), current_read_msg.GetBodyLength());
					eid entity_id = entity.id();
					for (int i = 0; i < entity.components_size(); ++i) {
						const proto::Component& comp = entity.components(i);
						if (update_functors.find(comp.component_case()) != update_functors.end()) {
							// TODO: extract frame_id and pass it as the 3 param instead of 0.
							update_functors[comp.component_case()](entity, comp, 0);
						}
					}
				}
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
				EventQueue<EnttityComponentUpdatedEvent>::ProcessEventQueue();
				try {
					if (this->socket.is_open() && this->socket.available()) {
						read_header();
					}
					if (this->stopped) {
						return;
					}
				}
				catch (std::exception& e) {
					std::cerr << e.what() << std::endl;
				}
			}
		}

		void ServerConnection::On(std::shared_ptr<EnttityComponentUpdatedEvent> data) {
			ServerMessage msg;
			msg.SetBodyLength(data->entity.ByteSize());
			// TODO: encode frame_id into the message.
			data->entity.SerializeToArray(msg.GetBodyPTR(), msg.GetBodyLength());
			msg.EntityUpdateMessage(true);
			msg.encode_header();
			Send(msg);
		}
	}
}
