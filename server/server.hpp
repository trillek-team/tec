// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <set>
#include <deque>
#include <asio.hpp>
#include <mutex>
#include "server-message.hpp"

using asio::ip::tcp;

namespace tec {
	namespace networking {
		extern const int SERVER_PORT;
		class ClientConnection;

		class Server {
		public:
			Server(tcp::endpoint& endpoint);

			// Deliver a message to all clients.
			// save_to_recent is used to save a recent list of message each client gets when they connect.
			void Deliver(const ServerMessage& msg, bool save_to_recent = true);

			// Deliver a message to a specific client.
			void Deliver(std::shared_ptr<ClientConnection> client, const ServerMessage& msg);

			// Calls when a client leaves, usually when the connection is no longer valid.
			void Leave(std::shared_ptr<ClientConnection> client);

			void Start();

			void Stop();

			void LockClientList() {
				this->client_list_mutex.lock();
			}

			void UnlockClientList() {
				
				this->client_list_mutex.unlock();
			}

			// Get a list of all connected clients.
			const std::set<std::shared_ptr<ClientConnection>>& GetClients() {
				return this->clients;
			}
		private:
			// Method that handles and accepts incoming connections.
			void AcceptHandler();
			
			// ASIO variables
			asio::io_service io_service;
			tcp::acceptor acceptor;
			tcp::socket socket;

			ServerMessage greeting_msg; // Greeting chat message.

			std::set<std::shared_ptr<ClientConnection>> clients; // All connected clients.
			std::uint64_t base_id = 10000; // Starting client_id

			// Recent message list all clients get on connecting,
			enum { max_recent_msgs = 100 };
			std::deque<ServerMessage> recent_msgs;
			static std::mutex recent_msgs_mutex;
			std::mutex client_list_mutex;
		};
	}
}
