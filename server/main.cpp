#include <algorithm>
#include <chrono>
#include <string>
#include <thread>

#include <asio.hpp>
#include <game_state.pb.h>

#include <file-factories.hpp>

#include "client-connection.hpp"
#include "filesystem.hpp"
#include "proto-load.hpp"
#include "server-game-state-queue.hpp"
#include "server-stats.hpp"
#include "server.hpp"
#include "simulation.hpp"

#include "resources/script-file.hpp"
#include <save-game.hpp>

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

const double SERVER_SIMULATE_RATE = 1.0 / 60.0;
std::shared_ptr<spdlog::logger> server_log;

using asio::ip::tcp;

tec::state_id_t current_state_id = 0;

namespace tec {
void RegisterFileFactories() { AddFileFactory<ScriptFile>(); }
eid GetNextEntityId() {
	static eid entity_id = 10000;
	return entity_id++;
}
} // namespace tec

void InitializeLogger() {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
	server_log = std::make_shared<spdlog::logger>("console_log", begin(sinks), end(sinks));
	server_log->set_level(spdlog::level::debug);
	server_log->set_pattern("[%l] %v"); // [%l] [thread %t] %v"); // Format on stdout
	spdlog::register_logger(server_log);
}

int main() {
	InitializeLogger();
	tec::RegisterFileFactories();
	std::chrono::high_resolution_clock::time_point last_time, next_time;
	std::chrono::duration<double> elapsed_seconds;
	bool closing = false;

	// Accumulated deltas since the last update send and physics simulation.
	double delta_accumulator = 0.0;
	double step_accumulator = 0.0;

	tec::ServerStats stats;
	tec::ServerGameStateQueue game_state_queue(stats);
	tec::Simulation simulation;

	// use constant mode stepping, because we don't need interpolated states on the server
	simulation.GetPhysicsSystem().SetSubstepping(0);

	tec::FilePath save_directory = tec::FilePath::GetAssetPath("save");

	if (!save_directory.DirExists()) {
		tec::FilePath::MkPath(save_directory);
	}

	try {
		tcp::endpoint endpoint(asio::ip::tcp::v4(), tec::networking::PORT);
		tec::networking::Server server(endpoint);

		const auto lua_sys = server.GetLuaSystem();

		tec::SaveGame::RegisterLuaType(lua_sys->GetGlobalState());
		tec::UserList::RegisterLuaType(lua_sys->GetGlobalState());
		tec::User::RegisterLuaType(lua_sys->GetGlobalState());
		tec::networking::ClientConnection::RegisterLuaType(lua_sys->GetGlobalState());

		tec::SaveGame save;
		save.Load(tec::FilePath::GetAssetPath("save/save1.json"));
		lua_sys->GetGlobalState()["save"] = save;

		// Load test script
		tec::FilePath fp = tec::FilePath::GetAssetPath("scripts/server-test.lua");
		if (fp.FileExists()) {
			lua_sys->LoadFile(fp);
		}

		last_time = std::chrono::high_resolution_clock::now();
		std::thread simulation_thread([&]() {
			while (!closing) {
				next_time = std::chrono::high_resolution_clock::now();
				uint64_t current_timestamp =
						std::chrono::duration_cast<std::chrono::milliseconds>(next_time.time_since_epoch()).count();
				elapsed_seconds = next_time - last_time;
				last_time = next_time;
				double delta = elapsed_seconds.count();
				delta_accumulator += delta;
				step_accumulator += delta;

				if (step_accumulator >= SERVER_SIMULATE_RATE) {
					server.ProcessEvents();
					step_accumulator -= SERVER_SIMULATE_RATE;
					game_state_queue.ProcessEventQueue();
					tec::GameState full_state =
							simulation.Simulate(SERVER_SIMULATE_RATE, game_state_queue.GetBaseState());

					if (delta_accumulator >= tec::UPDATE_RATE) {
						current_state_id++;
						full_state.state_id = current_state_id;
						full_state.timestamp = current_timestamp;
						tec::proto::GameStateUpdate full_state_update;
						full_state_update.set_command_id(current_state_id);
						full_state.Out(&full_state_update);
						tec::networking::MessageOut full_state_update_message(tec::networking::GAME_STATE_UPDATE);
						full_state_update.SerializeToZeroCopyStream(&full_state_update_message);

						{
							std::lock_guard lg(server.client_list_mutex);
							for (std::shared_ptr<tec::networking::ClientConnection> client : server.GetClients()) {
								if (!client->ReadyToReceive()) {
									client->ConfirmStateID(current_state_id);
									continue; // Don't send them state updates yet, the client is still loading
								}
								client->UpdateGameState(full_state);
								if (current_state_id - client->GetLastConfirmedStateID()
									> tec::TICKS_PER_SECOND * 2.0) {
									server.Deliver(client, full_state_update_message);
									server_log->debug(
											"sending full state {} to: {} client state ID was: {}",
											current_state_id,
											client->GetID(),
											client->GetLastConfirmedStateID());
								}
								else {
									server.Deliver(
											client,
											client->PrepareGameStateUpdateMessage(current_state_id, current_timestamp));
								}
							}
						}

						delta_accumulator -= tec::UPDATE_RATE;
					}
					game_state_queue.SetBaseState(std::move(full_state));

					// Processing events in LuaSystem
					tec::LuaSystem* lua_sys = server.GetLuaSystem();
					lua_sys->ProcessEvents();
				}
				else {
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}
		});
		server_log->info("Starting time: {}", last_time.time_since_epoch().count());
		server.Start();

		closing = true;
		simulation_thread.join();
	}
	catch (std::exception& e) {
		server_log->critical("Exception: {}", e.what());
	}
}
