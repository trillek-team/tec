#include "client/server_connection.hpp"

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
				chat_message msg;
				msg.body_length(message.size());
				memcpy(msg.body(), message.c_str(), msg.body_length());
				msg.encode_header();
				asio::write(this->socket, asio::buffer(msg.data(), msg.length()));
			}
		}

		void ServerConnection::read_body() {
			asio::error_code error = asio::error::eof;
			std::size_t len = asio::read(this->socket, 
				asio::buffer(current_read_msg.body(), current_read_msg.body_length()), error);

			if (!error) {
				std::string msg(current_read_msg.body(), current_read_msg.body_length());
				_log->info(msg);
			}
			else if (error) {
				throw asio::system_error(error);
				this->socket.close();
			}
		}

		void ServerConnection::read_header() {
			asio::error_code error = asio::error::eof;
			std::size_t len = asio::read(this->socket, 
				asio::buffer(this->current_read_msg.data(), chat_message::header_length), error);

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
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				catch (std::exception& e) {
					std::cerr << e.what() << std::endl;
				}
			}
		}
	}
}
