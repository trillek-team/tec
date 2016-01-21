#pragma once

#include <memory>
#include <asio.hpp>
#include <mutex>
#include <deque>
#include "simulation.hpp"
#include "types.hpp"
#include "server-message.hpp"

using asio::ip::tcp;

namespace tec {
	class GameState;

	namespace networking {
		class Server;
		
		// Used to represent a client connection to the server.
		class ClientConnection : public std::enable_shared_from_this < ClientConnection > {
		public:
			ClientConnection(tcp::socket socket, Server* server, Simulation& simulation) : last_confirmed_state_id(0), 
				socket(std::move(socket)), server(server), simulation(simulation) { }

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

			void DoJoin();

			void ConfirmStateID(state_id_t state_id) {
				this->last_confirmed_state_id = state_id;
			}

			state_id_t GetLastConfirmedStateID() {
				return this->last_confirmed_state_id;
			}

			void UpdateGameState(std::set<eid> updated_entities, const GameState& full_state);

			tec::networking::ServerMessage PrepareGameStateUpdateMessage(state_id_t current_state_id);

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

			state_id_t last_confirmed_state_id; // That last state_id the client confirmed it received.
			GameState state_changes_since_confirmed; // That state changes that happened since last_confirmed_state_id.
			Simulation& simulation;
		};
	}
}
