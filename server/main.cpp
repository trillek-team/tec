#include <iostream>
#include <string>
#include <asio.hpp>
#include <chrono>
#include <thread>
#include <map>
#include "server/server.hpp"
#include "simulation.hpp"

using asio::ip::tcp;

const double UPDATE_RATE = 1.0 / 50.0;  // TODO: Make this configurable via a run-time property.


namespace tec {
	std::map<std::string, std::function<void(std::string)>> file_factories;
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
		tec::networking::Server server(endpoint, simulation);
		std::cout << "Server ready" << std::endl;

		last_time = std::chrono::high_resolution_clock::now();
		std::thread simulation_thread([&] () {
			while (!closing) {
				next_time = std::chrono::high_resolution_clock::now();
				elapsed_seconds = next_time - last_time;
				last_time = next_time;
				//std::cout << "delta " << elapsed_seconds.count() << " accumulator " << delta_accumulator << std::endl;
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
