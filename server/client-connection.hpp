// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <memory>
#include <asio.hpp>
#include <mutex>
#include <deque>
#include "types.hpp"
#include "server-message.hpp"
#include "game-state.hpp"

using asio::ip::tcp;

namespace tec {
	struct FPSController;

	namespace networking {
		class Server;

		// Used to represent a client connection to the server.
		class ClientConnection
			: public std::enable_shared_from_this<ClientConnection> {
		public:
			ClientConnection(tcp::socket socket, Server* server) :
				socket(std::move(socket)), server(server) { }

			~ClientConnection();

			void StartRead();

			void QueueWrite(const ServerMessage& msg);

			eid GetID() {
				return this->id;
			}

			// Sets the client id and sends it to this client.
			void SetID(eid id);

			proto::Entity& GetEntity() {
				return this->entity;
			}

			void DoJoin(); // Emits an EntityCreated event and ENTITY_CREATE message to this client.

			void DoLeave(); // Emits an EntityDestroyed event.

			// Called when another client leaves.
			void OnClientLeave(eid entity_id);

			void ConfirmStateID(state_id_t state_id) {
				this->last_confirmed_state_id = state_id;
			}

			state_id_t GetLastConfirmedStateID() {
				return this->last_confirmed_state_id;
			}

			void UpdateGameState(const GameState& full_state);

			tec::networking::ServerMessage PrepareGameStateUpdateMessage(state_id_t current_state_id);

		private:
			void read_header();

			void read_body();

			void do_write();

			tcp::socket socket;
			ServerMessage current_read_msg;
			std::deque<ServerMessage> write_msgs_;
			Server* server;
			eid id{ 0 };
			proto::Entity entity;
			static std::mutex write_msg_mutex;

			FPSController* controller{ nullptr };

			state_id_t last_confirmed_state_id{ 0 }; // That last state_id the client confirmed it received.
			state_id_t last_recv_command_id{ 0 };
			GameState state_changes_since_confirmed; // That state changes that happened since last_confirmed_state_id.
		};
	}
}
