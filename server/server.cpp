// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "server.hpp"
#include "client-connection.hpp"
#include "components.pb.h"
#include "filesystem.hpp"

#include <iostream>
#include <fstream>
#include <thread>

using asio::ip::tcp;

namespace tec {
	namespace networking {
		unsigned short SERVER_PORT = 0xa10c;
		std::mutex Server::recent_msgs_mutex;

		Server::Server(tcp::endpoint& endpoint) : acceptor(io_service, endpoint), socket(io_service) {

			// Create a simple greeting chat message that all clients get.
			std::string message("Hello from server\n");
			this->greeting_msg.SetBodyLength(message.size());
			memcpy(this->greeting_msg.GetBodyPTR(), message.c_str(), this->greeting_msg.GetBodyLength());
			this->greeting_msg.encode_header();

			AcceptHandler();
		}

		// TODO: Implement a method to deliver a message to all clients except the source.
		void Server::Deliver(const ServerMessage& msg, bool save_to_recent) {
			if (save_to_recent) {
				std::lock_guard<std::mutex> lock(recent_msgs_mutex);
				this->recent_msgs.push_back(msg);
				while (this->recent_msgs.size() > max_recent_msgs) {
					this->recent_msgs.pop_front();
				}
			}

			LockClientList();
			for (auto client : this->clients) {
				client->QueueWrite(msg);
			}
			UnlockClientList();
		}

		void Server::Deliver(std::shared_ptr<ClientConnection> client, const ServerMessage& msg) {
			client->QueueWrite(msg);
		}

		void Server::Leave(std::shared_ptr<ClientConnection> client) {
			eid leaving_client_id = client->GetID();
			client->DoLeave(); // Send out entity destroyed events and client leave messages.

			LockClientList();
			this->clients.erase(client);
			UnlockClientList();

			// Notify other clients that a client left.
			for (auto _client : this->clients) {
				_client->OnClientLeave(leaving_client_id);
			}
		}

		void Server::Start() {
			this->io_service.run();
		}

		void Server::Stop() {
			LockClientList();
			this->clients.clear();
			UnlockClientList();
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

		void Server::On(std::shared_ptr<EntityCreated> data) {
			this->entities[data->entity_id] = data->entity;
		}

		void Server::On(std::shared_ptr<EntityDestroyed> data) {
			this->entities.erase(data->entity_id);
		}

		void Server::AcceptHandler() {
			acceptor.async_accept(
				socket,
				[this] (std::error_code error) {
					EventQueue<EntityCreated>::ProcessEventQueue();
					EventQueue<EntityDestroyed>::ProcessEventQueue();
					if (!error) {
						asio::write(socket, asio::buffer(greeting_msg.GetDataPTR(), greeting_msg.length()));
						std::shared_ptr<ClientConnection> client = std::make_shared<ClientConnection>(std::move(socket), this);

						// self_protopack does contain a renderable component
						static FilePath others_protopack = FilePath::GetAssetPath("protopacks/others.proto");
						proto::Entity other_entity;
						LoadProtoPack(other_entity, others_protopack);

						client->SetID(++base_id);
						client->DoJoin();

						static ServerMessage connecting_client_entity_msg;
						other_entity.set_id(client->GetID());
						connecting_client_entity_msg.SetBodyLength(other_entity.ByteSize());
						other_entity.SerializeToArray(connecting_client_entity_msg.GetBodyPTR(), static_cast<int>(connecting_client_entity_msg.GetBodyLength()));
						connecting_client_entity_msg.SetMessageType(MessageType::ENTITY_CREATE);
						connecting_client_entity_msg.encode_header();

						static ServerMessage other_client_entity_msg;
						for (auto other_client : clients) {
							other_entity.set_id(other_client->GetID());
							other_client_entity_msg.SetBodyLength(other_entity.ByteSize());
							other_entity.SerializeToArray(other_client_entity_msg.GetBodyPTR(), static_cast<int>(other_client_entity_msg.GetBodyLength()));
							other_client_entity_msg.SetMessageType(MessageType::ENTITY_CREATE);
							other_client_entity_msg.encode_header();

							client->QueueWrite(other_client_entity_msg);
							other_client->QueueWrite(connecting_client_entity_msg);
						}
						for (auto entity : entities) {
							other_client_entity_msg.SetBodyLength(entity.second.ByteSize());
							entity.second.SerializeToArray(other_client_entity_msg.GetBodyPTR(), static_cast<int>(other_client_entity_msg.GetBodyLength()));
							other_client_entity_msg.SetMessageType(MessageType::ENTITY_CREATE);
							other_client_entity_msg.encode_header();

							client->QueueWrite(other_client_entity_msg);
						}

						LockClientList();
						clients.insert(client);
						UnlockClientList();
						{
							std::lock_guard<std::mutex> lock(recent_msgs_mutex);
							for (auto msg : this->recent_msgs) {
								client->QueueWrite(msg);
							}
						}
						client->StartRead();
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					AcceptHandler();
				});
		}
	}
}
