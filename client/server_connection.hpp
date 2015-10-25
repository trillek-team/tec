#pragma once

#include <asio.hpp>
#include <spdlog/spdlog.h>
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <atomic>
#include "server-message.hpp"
#include "event-system.hpp"

using asio::ip::tcp;

namespace tec {
	struct EnttityComponentUpdatedEvent;
	extern std::map<proto::Component::ComponentCase, std::function<void(const proto::Entity&, const proto::Component&, const frame_id_t)>> update_functors;
	extern eid client_id;

	namespace networking {
		extern const char* SERVER_PORT_STR;
		extern const char* LOCAL_HOST;
		typedef std::chrono::milliseconds::rep ping_time_t;

		// Used to connect to a server.
		class ServerConnection : public EventQueue < EnttityComponentUpdatedEvent > {
		public:
			ServerConnection();

			bool Connect(std::string ip = LOCAL_HOST);

			void Disconnect();

			void StopRead();

			void StartRead();

			void StartSync();

			void Write(std::string message);

			void Send(ServerMessage& msg);

			std::list<ping_time_t> GetRecentPings() {
				return this->recent_pings;
			}

			ping_time_t GetAveragePing() {
				return this->average_ping;
			}

			eid GetClientID() {
				return this->client_id;
			}

			void On(std::shared_ptr<EnttityComponentUpdatedEvent> data);
		private:
			void read_body();
			void read_header();

			static std::shared_ptr<spdlog::logger> _log;
			asio::io_service io_service;
			asio::ip::tcp::socket socket;
			std::atomic<bool> stopped;
			ServerMessage current_read_msg;

			std::chrono::high_resolution_clock::time_point sync_start, recv_time;
			std::list<ping_time_t> recent_pings;
			ping_time_t average_ping;
			eid client_id = 0;
		};
	}
}
