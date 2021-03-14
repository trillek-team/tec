/**
 * Unit tests
 */

#include <gtest/gtest.h>

#include "client/server-connection.hpp"
#include "server-stats.hpp"
#include "server/client-connection.hpp"
#include "server/server.hpp"
#include "tec-types.hpp"
#include <asio.hpp>
#include <file-factories.hpp>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

namespace tec {
namespace networking {

class ClientCommandReceptor : public EventQueue<ClientCommandsEvent> {
public:
	ClientCommandReceptor() { last_event = promised_event.get_future(); }
	void process() { EventQueue<ClientCommandsEvent>::ProcessEventQueue(); }
	using EventQueue<ClientCommandsEvent>::On;
	virtual void On(std::shared_ptr<ClientCommandsEvent> data) { promised_event.set_value(data); }
	std::promise<std::shared_ptr<ClientCommandsEvent>> promised_event;
	std::future<std::shared_ptr<ClientCommandsEvent>> last_event;
};
TEST(ServerClientCommunications, TCPConnection) {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
	auto log = std::make_shared<spdlog::logger>("console_log", begin(sinks), end(sinks));
	log->set_level(spdlog::level::trace);
	spdlog::register_logger(log);

	ClientCommandReceptor client_reply;
	bool test_running = true;

	tcp::endpoint endpoint(asio::ip::address_v4::loopback(), tec::networking::PORT);
	tec::networking::Server server(endpoint);
	std::thread start_thread([&server]() { server.Start(); });

	std::promise<tec::eid> promise_client_id;
	std::future<tec::eid> client_id = promise_client_id.get_future();

	tec::ServerStats stats;
	tec::networking::ServerConnection connection(stats);
	std::thread asio_thread([&connection]() { connection.StartDispatch(); });

	connection.RegisterMessageHandler(
			tec::networking::MessageType::CLIENT_ID, [&connection, &promise_client_id](tec::networking::MessageIn&) {
				promise_client_id.set_value(connection.GetClientID());
			});
	std::thread response_thread([&client_reply, &test_running]() {
		while (test_running) {
			client_reply.process();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	});
	bool connect_function_called = false;
	connection.RegisterConnectFunc([&]() { connect_function_called = true; });
	connection.Connect();
	std::string teststring;
	for (int i = 0; i < 500; i++) {
		teststring.append("Test");
	}
	{
		proto::ClientCommands proto_client_commands;
		proto_client_commands.set_id(7);
		proto_client_commands.set_commandid(16);
		proto_client_commands.set_laststateid(42);

		proto_client_commands.add_commandlist(teststring);
		MessageOut command_message(MessageType::CLIENT_COMMAND);
		proto_client_commands.SerializeToZeroCopyStream(&command_message);
		connection.Send(command_message);
	}

	auto client_id_received{client_id.wait_until(std::chrono::system_clock::now() + std::chrono::seconds(1))};
	auto client_command_received{
			client_reply.last_event.wait_until(std::chrono::system_clock::now() + std::chrono::seconds(1))};
	auto clients = server.GetClients();
	ASSERT_GE(clients.size(), 1);
	ASSERT_TRUE(connect_function_called);
	auto test_client = *clients.cbegin();
	auto last_confirmed_state = test_client->GetLastConfirmedStateID();
	size_t unfullfilled_messages_from_client = test_client->GetPartialMessageCount();
	size_t unfullfilled_messages_from_server = connection.GetPartialMessageCount();
	EXPECT_EQ(unfullfilled_messages_from_client, 0);
	EXPECT_EQ(unfullfilled_messages_from_server, 0);

	ASSERT_EQ(client_id_received, std::future_status::ready);
	ASSERT_EQ(client_command_received, std::future_status::ready);

	test_running = false;
	response_thread.join();

	server.Stop();
	connection.Stop();
	start_thread.join();
	asio_thread.join();

	EXPECT_EQ(client_id.get(), 10001);
	std::shared_ptr<ClientCommandsEvent> result_event = client_reply.last_event.get();
	ASSERT_TRUE(result_event);
	EXPECT_EQ(last_confirmed_state, 42);
	auto& client_commands = result_event->client_commands;
	EXPECT_TRUE(client_commands.has_laststateid());
	EXPECT_EQ(client_commands.laststateid(), 42);
	ASSERT_EQ(client_commands.commandlist_size(), 1);
	EXPECT_EQ(client_commands.commandlist(0), teststring);
}
} // namespace networking
} // namespace tec
