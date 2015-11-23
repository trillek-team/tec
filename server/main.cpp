#include <iostream>
#include <string>
#include <asio.hpp>
#include <chrono>
#include <thread>
#include <map>
#include "server/server.hpp"
#include "simulation.hpp"

using asio::ip::tcp;

std::uint64_t UPDATE_RATE = 10; // Updates should be sent every this many milliseconds.


namespace tec {
	std::map<std::string, std::function<void(std::string)>> file_factories;
	eid active_entity;
}

int main() {
	std::chrono::high_resolution_clock::time_point last_time, next_time;
	std::chrono::milliseconds elapsed_seconds;
	bool closing = false;
	std::uint64_t delta_accumulator = 0; // Accumulated deltas since the last update was sent.

	tec::Simulation simulation;

	try {
		tcp::endpoint endpoint(asio::ip::tcp::v4(), tec::networking::SERVER_PORT);
		tec::networking::Server server(endpoint);
		std::cout << "Server ready" << std::endl;

		last_time = std::chrono::high_resolution_clock::now();
		std::thread simulation_thread([&] () {
			while (!closing) {
				next_time = std::chrono::high_resolution_clock::now();
				elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(next_time - last_time);
				last_time = next_time;
				simulation.Simulate(elapsed_seconds.count() / 1000.0);
				delta_accumulator += elapsed_seconds.count();
				if (delta_accumulator >= UPDATE_RATE) {
					/*std::map<tec::eid, std::map<tec::tid, tec::proto::Component>>&& results = simulation.GetResults();
					for (const auto& entity_update_set : results) {
						tec::proto::Entity entity;
						entity.set_id(entity_update_set.first);
						for (const auto& compoennt_update : entity_update_set.second) {
							tec::proto::Component* comp = entity.add_components();
							*comp = compoennt_update.second;
						}
						tec::networking::ServerMessage msg;
						msg.SetBodyLength(entity.ByteSize());
						entity.SerializeToArray(msg.GetBodyPTR(), msg.GetBodyLength());
						msg.SetMessageType(tec::networking::ENTITY_UPDATE);
						msg.encode_header();
						server.Deliver(msg);
					}*/
					delta_accumulator -= UPDATE_RATE;
				}
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
