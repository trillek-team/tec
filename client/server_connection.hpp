#pragma once

#include <asio.hpp>
#include <spdlog/spdlog.h>
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <atomic>
#include "chat_message.hpp"

using asio::ip::tcp;

namespace tec {
	namespace networking {
		extern const char* SERVER_PORT_STR;
		extern const char* LOCAL_HOST;

		// Used to connect to a server.
		class ServerConnection {
		public:
			ServerConnection();

			bool Connect(std::string ip = LOCAL_HOST);

			void Disconnect();

			void StopRead();

			void StartRead();

			void Write(std::string message);
		private:
			void read_body();
			void read_header();

			static std::shared_ptr<spdlog::logger> _log;
			asio::io_service io_service;
			asio::ip::tcp::socket socket;
			std::atomic<bool> stopped;
			chat_message current_read_msg;
		};
	}
}
