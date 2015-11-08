#pragma once

#include <memory>
#include <asio.hpp>
#include <mutex>
#include <deque>
#include "types.hpp"
#include "server-message.hpp"

using asio::ip::tcp;

namespace tec {
	namespace networking {
		class Server;
		
		// Used to represent a client connection to the server.
		class ClientConnection : public std::enable_shared_from_this < ClientConnection > {
		public:
			ClientConnection(tcp::socket socket, Server* server) : socket(std::move(socket)), server(server) { }

			void StartRead();

			void QueueWrite(const ServerMessage& msg);

			eid GetID() {
				return this->id;
			}

			void SetID(eid id) {
				this->id = id;
				this->entity.set_id(id);
			}

			proto::Entity& GetEntity() {
				return this->entity;
			}

		private:
			void read_header();

			void read_body();

			void do_write();

			tcp::socket socket;
			ServerMessage current_read_msg;
			std::deque<ServerMessage> write_msgs_;
			Server* server;
			eid id;
			proto::Entity entity;
			static std::mutex write_msg_mutex;
		};
	}
}
