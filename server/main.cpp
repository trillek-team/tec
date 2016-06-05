// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include <iostream>
#include <string>
#include <asio.hpp>
#include <chrono>
#include <thread>
#include <map>
#include "server/server.hpp"
#include "server/client-connection.hpp"
#include "proto/game_state.pb.h"
#include "simulation.hpp"

using asio::ip::tcp;

tec::state_id_t current_state_id = 0;

namespace tec {
	std::map<std::string, std::function<void(std::string)>> file_factories;
	std::map<tid, std::function<void(const proto::Entity&, const proto::Component&)>> in_functors;
	eid active_entity;
}

int main() {
	std::chrono::high_resolution_clock::time_point last_time, next_time;
	std::chrono::duration<double> elapsed_seconds;
	bool closing = false;
	double delta_accumulator = 0.0; // Accumulated deltas since the last update was sent.

	tec::Simulation simulation;

	try {
		tcp::endpoint endpoint(asio::ip::tcp::v4(), tec::networking::SERVER_PORT);
		tec::networking::Server server(endpoint);
		std::cout << "Server ready" << std::endl;

		last_time = std::chrono::high_resolution_clock::now();
		std::thread simulation_thread([&] () {
			while (!closing) {
				next_time = std::chrono::high_resolution_clock::now();
				elapsed_seconds = next_time - last_time;
				last_time = next_time;
				delta_accumulator += elapsed_seconds.count();
				if (delta_accumulator >= tec::UPDATE_RATE) {
					auto updated_entities = simulation.Simulate(tec::UPDATE_RATE);
					current_state_id++;
					tec::GameState& full_state = simulation.GetClientState();
					full_state.state_id = current_state_id;
					tec::proto::GameStateUpdate full_state_update;
					full_state.Out(&full_state_update);
					tec::networking::ServerMessage full_state_update_message;
					full_state_update_message.SetStateID(current_state_id);
					full_state_update_message.SetMessageType(tec::networking::GAME_STATE_UPDATE);
					full_state_update_message.SetBodyLength(full_state_update.ByteSize());
					full_state_update.SerializeToArray(full_state_update_message.GetBodyPTR(), full_state_update_message.GetBodyLength());
					full_state_update_message.encode_header();
					server.LockClientList();
					for (std::shared_ptr<tec::networking::ClientConnection> client : server.GetClients()) {
						client->UpdateGameState(updated_entities, full_state);
						if (current_state_id - client->GetLastConfirmedStateID() > tec::UPDATE_RATE * 2.0) {
							server.Deliver(client, full_state_update_message);
							std::cout << "sending full state " << current_state_id << " to: " << client->GetID() << " client state ID was: " << client->GetLastConfirmedStateID() << std::endl;
						}
						else {
							server.Deliver(client, client->PrepareGameStateUpdateMessage(current_state_id));
						}
					}
					
					server.UnlockClientList();
					delta_accumulator -= tec::UPDATE_RATE;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		});
		std::cout << "Starting time: " << last_time.time_since_epoch().count() << std::endl;
		server.Start();
		closing = true;
		simulation_thread.join();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}
