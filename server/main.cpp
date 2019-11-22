// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include <game_state.pb.h>
#include <google/protobuf/util/json_util.h>
#include <asio.hpp>

#include "client-connection.hpp"
#include "filesystem.hpp"
#include "game-state-queue.hpp"
#include "server.hpp"
#include "simulation.hpp"

using asio::ip::tcp;

tec::state_id_t current_state_id = 0;

namespace tec {
	eid active_entity;

	std::string LoadJSON(const FilePath& fname) {
		std::fstream input(fname.GetNativePath(), std::ios::in | std::ios::binary);
		if (!input.good()) throw std::runtime_error("can't open ." + fname.toString());

		std::string in;
		input.seekg(0, std::ios::end);
		in.reserve(static_cast<std::size_t>(input.tellg()));
		input.seekg(0, std::ios::beg);
		std::copy((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>(),
			std::back_inserter(in));
		input.close();
		return in;
	}

	void ProtoLoadEntity(const FilePath& fname) {
		std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
		std::string json_string = LoadJSON(fname);
		google::protobuf::util::JsonStringToMessage(json_string, &data->entity);
		data->entity_id = data->entity.id();
		EventSystem<EntityCreated>::Get()->Emit(data);
	}
} // namespace tec

int main() {
	std::chrono::high_resolution_clock::time_point last_time, next_time;
	std::chrono::duration<double> elapsed_seconds;
	bool closing = false;

	// Accumulated deltas since the last update was sent.
	double delta_accumulator = 0.0;

	tec::GameStateQueue game_state_queue;
	tec::Simulation simulation;

	try {
		tcp::endpoint endpoint(asio::ip::tcp::v4(), tec::networking::PORT);
		tec::networking::Server server(endpoint);
		std::cout << "Server ready" << std::endl;

		tec::ProtoLoadEntity(tec::FilePath::GetAssetPath("json/1000.json"));

		last_time = std::chrono::high_resolution_clock::now();
		std::thread simulation_thread([&]() {
			while (!closing) {
				next_time = std::chrono::high_resolution_clock::now();
				elapsed_seconds = next_time - last_time;
				last_time = next_time;
				delta_accumulator += elapsed_seconds.count();
				if (delta_accumulator >= tec::UPDATE_RATE) {
					current_state_id++;
					game_state_queue.ProcessEventQueue();
					tec::GameState full_state =
						simulation.Simulate(tec::UPDATE_RATE, game_state_queue.GetBaseState());
					full_state.state_id = current_state_id;
					tec::proto::GameStateUpdate full_state_update;
					full_state_update.set_command_id(current_state_id);
					full_state.Out(&full_state_update);
					tec::networking::ServerMessage full_state_update_message;
					full_state_update_message.SetStateID(current_state_id);
					full_state_update_message.SetMessageType(
						tec::networking::MessageType::GAME_STATE_UPDATE);
					full_state_update_message.SetBodyLength(full_state_update.ByteSize());
					full_state_update.SerializeToArray(full_state_update_message.GetBodyPTR(),
						static_cast<int>(full_state_update_message.GetBodyLength()));
					full_state_update_message.encode_header();

					{
						std::lock_guard lg(server.client_list_mutex);
						for (std::shared_ptr<tec::networking::ClientConnection> client :
							server.GetClients()) {
							client->UpdateGameState(full_state);
							if (current_state_id - client->GetLastConfirmedStateID() >
								tec::TICKS_PER_SECOND * 2.0) {
								server.Deliver(client, full_state_update_message);
								std::cout << "sending full state " << current_state_id
										  << " to: " << client->GetID() << " client state ID was: "
										  << client->GetLastConfirmedStateID() << std::endl;
							}
							else {
								server.Deliver(client,
									client->PrepareGameStateUpdateMessage(current_state_id));
							}
						}
					}

					delta_accumulator -= tec::UPDATE_RATE;
					game_state_queue.SetBaseState(std::move(full_state));
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		});
		std::cout << "Starting time: " << last_time.time_since_epoch().count() << std::endl;
		server.Start();
		closing = true;
		simulation_thread.join();
	} catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}
