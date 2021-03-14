/**
 * Unit tests
 */

#include <gtest/gtest.h>

#include "client/server-connection.hpp"
#include "server-stats.hpp"
#include "server/server.hpp"
#include "tec-types.hpp"
#include <asio.hpp>
#include <file-factories.hpp>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

TEST(ClientServerConnection_test, Constructor) {
	std::vector<spdlog::sink_ptr> sinks;
	auto log = std::make_shared<spdlog::logger>("console_log", begin(sinks), end(sinks));
	spdlog::register_logger(log);

	tcp::endpoint endpoint(asio::ip::tcp::v4(), tec::networking::PORT);
	tec::networking::Server server(endpoint);
	std::thread start_thread([&server]() { server.Start(); });

	std::promise<tec::eid> promise_client_id;
	std::future<tec::eid> client_id = promise_client_id.get_future();

	tec::ServerStats stats;
	tec::networking::ServerConnection connection(stats);
	std::shared_ptr<std::thread> asio_thread;

	connection.RegisterMessageHandler(
			tec::networking::MessageType::CLIENT_ID,
			[&connection, &promise_client_id](const tec::networking::ServerMessage&) {
				promise_client_id.set_value(connection.GetClientID());
			});
	connection.RegisterConnectFunc([&]() {
		asio_thread = std::make_shared<std::thread>(std::thread(([&connection]() { connection.StartRead(); })));
	});
	connection.Connect();

	auto status{client_id.wait_until(std::chrono::system_clock::now() + std::chrono::seconds(1))};
	server.Stop();
	start_thread.join();
	connection.Stop();
	asio_thread->join();

	EXPECT_EQ(status, std::future_status::ready);
	EXPECT_EQ(client_id.get(), 10001);
}
