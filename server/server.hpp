#pragma once

#include <set>
#include <deque>
#include <asio.hpp>
#include "server-message.hpp"

using asio::ip::tcp;

namespace tec {
	namespace networking {
		extern const int SERVER_PORT;
		class ClientConnection;

		class Server {
		public:
			Server(tcp::endpoint& endpoint);

			void Deliver(const ServerMessage& msg);

			void Leave(std::shared_ptr<ClientConnection> client);

			void Start();

			void Stop();
		private:
			void do_accept();
			
			asio::io_service io_service;
			tcp::acceptor acceptor;
			tcp::socket socket;
			ServerMessage greeting_msg;
			std::set<std::shared_ptr<ClientConnection>> clients;
			enum { max_recent_msgs = 100 };
			std::deque<ServerMessage> recent_msgs;
			std::uint64_t base_id = 10000;
		};
	}
}
