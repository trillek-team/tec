#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>

#include <asio.hpp>
#include <google/protobuf/util/json_util.h>
#include <game_state.pb.h>

#include <file-factories.hpp>

#include "filesystem.hpp"
#include "server.hpp"
#include "server-stats.hpp"
#include "client-connection.hpp"
#include "server-game-state-queue.hpp"
#include "simulation.hpp"

#include "resources/script-file.hpp"

const double SERVER_SIMULATE_RATE = 1.0 / 60.0;

using asio::ip::tcp;

tec::state_id_t current_state_id = 0;

namespace tec {
	void RegisterFileFactories() {
		AddFileFactory<ScriptFile>();
	}
	std::string LoadJSON(const FilePath& fname) {
		std::fstream input(fname.GetNativePath(), std::ios::in | std::ios::binary);
		if (!input.good())
			throw std::runtime_error("can't open ." + fname.toString());

		std::string in;
		input.seekg(0, std::ios::end);
		in.reserve(static_cast<std::size_t>(input.tellg()));
		input.seekg(0, std::ios::beg);
		std::copy((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>(), std::back_inserter(in));
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
}

int main() {
	tec::RegisterFileFactories();
	std::chrono::high_resolution_clock::time_point last_time, next_time;
	std::chrono::duration<double> elapsed_seconds;
	bool closing = false;

	// Accumulated deltas since the last update was sent.
	double delta_accumulator = 0.0;
	// Accumulated deltas since the last physics simulation.
	double step_accumulator = 0.0;

	tec::ServerStats stats;
	tec::ServerGameStateQueue game_state_queue(stats);
	tec::Simulation simulation;

	// use constant mode stepping, because we don't need interpolated states on the server
	simulation.GetPhysicsSystem().SetSubstepping(0);

	try {
		tcp::endpoint endpoint(asio::ip::tcp::v4(), tec::networking::PORT);
		tec::networking::Server server(endpoint);
		std::cout << "Server ready" << std::endl;

		tec::ProtoLoadEntity(tec::FilePath::GetAssetPath("json/1000.json"));

		last_time = std::chrono::high_resolution_clock::now();
		std::thread simulation_thread(
			[&] () {
				while (!closing) {
					next_time = std::chrono::high_resolution_clock::now();
					uint64_t current_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(next_time.time_since_epoch()).count();
					elapsed_seconds = next_time - last_time;
					last_time = next_time;
					double delta = elapsed_seconds.count();
					delta_accumulator += delta;
					step_accumulator += delta;

					if (step_accumulator >= SERVER_SIMULATE_RATE) {
						step_accumulator -= SERVER_SIMULATE_RATE;
						game_state_queue.ProcessEventQueue();
						tec::GameState full_state = simulation.Simulate(SERVER_SIMULATE_RATE, game_state_queue.GetBaseState());

						if (delta_accumulator >= tec::UPDATE_RATE) {
							current_state_id++;
							full_state.state_id = current_state_id;
							full_state.timestamp = current_timestamp;
							tec::proto::GameStateUpdate full_state_update;
							full_state_update.set_command_id(current_state_id);
							full_state.Out(&full_state_update);
							tec::networking::ServerMessage full_state_update_message;
							full_state_update_message.SetStateID(current_state_id);
							full_state_update_message.SetMessageType(tec::networking::MessageType::GAME_STATE_UPDATE);
							full_state_update_message.SetBodyLength(full_state_update.ByteSizeLong());
							full_state_update.SerializeToArray(full_state_update_message.GetBodyPTR(), static_cast<int>(full_state_update_message.GetBodyLength()));
							full_state_update_message.encode_header();

							{
								std::lock_guard lg(server.client_list_mutex);
								for (std::shared_ptr<tec::networking::ClientConnection> client : server.GetClients()) {
									client->UpdateGameState(full_state);
									if (current_state_id - client->GetLastConfirmedStateID() > tec::TICKS_PER_SECOND * 2.0) {
										server.Deliver(client, full_state_update_message);
										std::cout << "sending full state " << current_state_id << " to: " << client->GetID() << " client state ID was: " << client->GetLastConfirmedStateID() << std::endl;
									}
									else {
										server.Deliver(client, client->PrepareGameStateUpdateMessage(current_state_id, current_timestamp));
									}
								}
							}

							delta_accumulator -= tec::UPDATE_RATE;
						}

						game_state_queue.SetBaseState(std::move(full_state));
					}
					else {
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
				}
			});
		std::cout << "Starting time: " << last_time.time_since_epoch().count() << std::endl;
		server.Start();
		closing = true;
		simulation_thread.join();
	}
	catch (std::exception & e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}
