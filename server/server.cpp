#include "server/server.hpp"
#include "server/client_connection.hpp"

using asio::ip::tcp;

namespace tec {
	namespace networking {
		const int SERVER_PORT = 0xa10c;

		Server::Server(tcp::endpoint& endpoint) : acceptor(io_service, endpoint), socket(io_service) {
			std::string message("Hello from server\n");
			greeting_msg.SetBodyLength(message.size());
			memcpy(greeting_msg.GetBodyPTR(), message.c_str(), greeting_msg.GetBodyLength());
			greeting_msg.encode_header();
			do_accept();
		}

		// TODO: Implement a method to deliver a message to all clients except the source.
		void Server::Deliver(const ServerMessage& msg) {
			this->recent_msgs.push_back(msg);
			while (this->recent_msgs.size() > max_recent_msgs) {
				this->recent_msgs.pop_front();
			}

			for (auto client : this->clients) {
				client->QueueWrite(msg);
			}
		}

		void Server::Leave(std::shared_ptr<ClientConnection> client) {
			this->clients.erase(client);
		}

		void Server::Start() {
			this->io_service.run();
		}

		void Server::Stop() {
			this->io_service.stop();
		}

		void Server::do_accept() {
			acceptor.async_accept(socket,
				[this] (std::error_code error) {
				if (!error) {
					asio::write(socket, asio::buffer(greeting_msg.GetDataPTR(), greeting_msg.length()));
					std::shared_ptr<ClientConnection> client = std::make_shared<ClientConnection>(std::move(socket), this);
					client->SetID(++base_id);
					std::string message(std::to_string(client->GetID()));
					static ServerMessage id_message;
					id_message.SetMessageType(CLIENT_ID);
					id_message.SetBodyLength(message.size());
					memcpy(id_message.GetBodyPTR(), message.c_str(), id_message.GetBodyLength());
					id_message.encode_header();
					client->QueueWrite(id_message);
					clients.insert(client);
					for (auto msg : this->recent_msgs) {
						client->QueueWrite(msg);
					}
					client->StartRead();
				}

				do_accept();
			});
		}
	}
}
