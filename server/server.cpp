#include "server/server.hpp"
#include "server/client_connection.hpp"
#include "spdlog/spdlog.h"
#include "../proto/components.pb.h"
#include "simulation.hpp"
#include "filesystem.hpp"

#include <fstream>

using asio::ip::tcp;

namespace tec {
	namespace networking {
		const int SERVER_PORT = 0xa10c;
		std::mutex Server::recent_msgs_mutex;

		Server::Server(tcp::endpoint& endpoint, Simulation& simulation) :
			acceptor(io_service, endpoint), socket(io_service), simulation(simulation) {
			std::string message("Hello from server\n");
			greeting_msg.SetBodyLength(message.size());
			memcpy(greeting_msg.GetBodyPTR(), message.c_str(), greeting_msg.GetBodyLength());
			greeting_msg.encode_header();
			do_accept();
		}

		// TODO: Implement a method to deliver a message to all clients except the source.
		void Server::Deliver(const ServerMessage& msg) {
			{
				std::lock_guard<std::mutex> lock(recent_msgs_mutex);
				this->recent_msgs.push_back(msg);
				while (this->recent_msgs.size() > max_recent_msgs) {
					this->recent_msgs.pop_front();
				}
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

		void LoadProtoPack(proto::Entity& entity, const FilePath& fname) {
			if (fname.isValidPath() && fname.FileExists()) {
				std::fstream input(fname.GetNativePath(), std::ios::in | std::ios::binary);
				entity.ParseFromIstream(&input);
			}
			else {
				std::cout << "error loading protopack " << fname.toString() << std::endl;
			}
		}

		void Server::do_accept() {
			acceptor.async_accept(socket,
				[this] (std::error_code error) {
				if (!error) {
					asio::write(socket, asio::buffer(greeting_msg.GetDataPTR(), greeting_msg.length()));
					std::shared_ptr<ClientConnection> client = std::make_shared<ClientConnection>(std::move(socket), this, this->simulation);
					FilePath self_protopack = FilePath::GetAssetPath("protopacks/self.proto");
					FilePath others_protopack = FilePath::GetAssetPath("protopacks/others.proto");
					LoadProtoPack(client->GetEntity(), self_protopack);
					client->SetID(++base_id);
					proto::Entity other_entity;
					LoadProtoPack(other_entity, others_protopack);
					std::string message(std::to_string(client->GetID()));
					static ServerMessage id_message;
					id_message.SetMessageType(CLIENT_ID);
					id_message.SetBodyLength(message.size());
					memcpy(id_message.GetBodyPTR(), message.c_str(), id_message.GetBodyLength());
					id_message.encode_header();
					client->QueueWrite(id_message);

					ServerMessage client_create_msg;
					proto::Entity& client_entity = client->GetEntity();
					client_create_msg.SetBodyLength(client_entity.ByteSize());
					client_entity.SerializeToArray(client_create_msg.GetBodyPTR(), client_create_msg.GetBodyLength());
					client_create_msg.SetMessageType(ENTITY_CREATE);
					client_create_msg.encode_header();
					client->QueueWrite(client_create_msg);
					proto::Entity& other_client_entity = other_entity;

					other_entity.set_id(client->GetID());
					ServerMessage self_client_entity_msg;
					self_client_entity_msg.SetBodyLength(other_client_entity.ByteSize());
					other_client_entity.SerializeToArray(self_client_entity_msg.GetBodyPTR(), self_client_entity_msg.GetBodyLength());
					self_client_entity_msg.SetMessageType(ENTITY_CREATE);
					self_client_entity_msg.encode_header();

					for (auto other_client : clients) {
						other_entity.set_id(other_client->GetID());
						ServerMessage other_client_entity_msg;
						other_client_entity_msg.SetBodyLength(other_client_entity.ByteSize());
						other_client_entity.SerializeToArray(other_client_entity_msg.GetBodyPTR(), other_client_entity_msg.GetBodyLength());
						other_client_entity_msg.SetMessageType(ENTITY_CREATE);
						other_client_entity_msg.encode_header();
						client->QueueWrite(other_client_entity_msg);
						other_client->QueueWrite(self_client_entity_msg);
					}

					clients.insert(client);
					{
						std::lock_guard<std::mutex> lock(recent_msgs_mutex);
						for (auto msg : this->recent_msgs) {
							client->QueueWrite(msg);
						}
					}
					client->StartRead();
				}

				do_accept();
			});
		}
	}
}
