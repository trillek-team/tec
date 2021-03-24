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
#include <mutex>
#include <save-game.hpp>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

const tec::eid BASE_ENTITY_ID = 10000;

namespace tec {
eid GetNextEntityId() {
	static eid entity_id = BASE_ENTITY_ID;
	return entity_id++;
}
namespace networking {

class ClientCommandReceptor : public EventQueue<ClientCommandsEvent> {
public:
	ClientCommandReceptor() { last_event = promised_event.get_future(); }
	void process() { EventQueue<ClientCommandsEvent>::ProcessEventQueue(); }
	using EventQueue<ClientCommandsEvent>::On;
	void On(std::shared_ptr<ClientCommandsEvent> data) override { promised_event.set_value(data); }
	std::promise<std::shared_ptr<ClientCommandsEvent>> promised_event;
	std::future<std::shared_ptr<ClientCommandsEvent>> last_event;
};

TEST(ServerClientCommunications, TCPConnection) {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
	auto log = std::make_shared<spdlog::logger>("console_log", begin(sinks), end(sinks));
	log->set_level(spdlog::level::trace);
	spdlog::register_logger(log);

	// helper object to capture events, since the server doesn't have registerable message handlers
	ClientCommandReceptor client_reply;
	bool test_running = true;

	// use the loopback, it's more likely to work in limited environments
	tcp::endpoint endpoint(asio::ip::address_v4::loopback(), tec::networking::PORT);

	// standard server initialization
	tec::networking::Server server(endpoint);
	tec::SaveGame save; // need a SaveGame object with Users to test logins
	auto lua_sys = server.GetLuaSystem();
	tec::SaveGame::RegisterLuaType(lua_sys->GetGlobalState());
	tec::UserList::RegisterLuaType(lua_sys->GetGlobalState());
	tec::User::RegisterLuaType(lua_sys->GetGlobalState());
	tec::networking::ClientConnection::RegisterLuaType(lua_sys->GetGlobalState());
	lua_sys->GetGlobalState()["save"] = &save; // provide a pointer

	// add a fake user to test login
	auto test_user = save.GetUserList()->CreateUser("login-test-user", "bob");

	// setup scripting environment for tests
	lua_sys->ExecuteString("lua_test={}");
	sol::table lua_was_called = lua_sys->GetGlobalState()["lua_test"];
	lua_was_called["error"] = [](std::string message) { FAIL() << message; };
	// scripts to test if the events trigger calls
	lua_sys->ExecuteString("function onClientConnected(info) lua_test[\"onClientConnected\"] = true end");
	lua_sys->ExecuteString( // simple script to handle logins
			"function onUserLogin(client, name)\n"
			"lua_test[\"onUserLogin\"] = true\n"
			"client.user = save.user_list:FindUser(name)\n"
			"if client.user==nil then lua_test.error(\"login test failed\") end\n"
			"end");
	std::thread server_thread([&server]() { server.Start(); });

	std::promise<tec::eid> promise_client_id;
	std::future<tec::eid> client_id = promise_client_id.get_future();

	tec::ServerStats stats;
	tec::networking::ServerConnection connection(stats);
	std::thread client_asio_thread([&connection]() { connection.StartDispatch(); });

	connection.RegisterMessageHandler(
			// when we recieve a client ID, that signals that the server completed our login successfully
			tec::networking::MessageType::CLIENT_ID,
			[&connection, &promise_client_id](tec::networking::MessageIn&) {
				promise_client_id.set_value(connection.GetClientID());
			});
	std::thread response_thread([&client_reply, &test_running]() {
		while (test_running) {
			client_reply.process(); // handle events on the "server"
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	});
	std::timed_mutex connect_function_called;
	connect_function_called.lock();
	connection.RegisterConnectFunc([&]() {
		// when we connect, send a test login
		proto::UserLogin user_login;
		user_login.set_username("bob");
		user_login.set_password("test");
		networking::MessageOut msg(tec::networking::LOGIN);
		user_login.SerializeToZeroCopyStream(&msg);
		connection.Send(msg);
		connect_function_called.unlock(); // signal that we have connected
	});
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
	// did the client connect and send login?
	EXPECT_TRUE(connect_function_called.try_lock_for(std::chrono::seconds(1)));

	// wait for our transactions to complete (or timeout)
	auto client_id_received{client_id.wait_until(std::chrono::system_clock::now() + std::chrono::seconds(1))};
	auto client_command_received{
			client_reply.last_event.wait_until(std::chrono::system_clock::now() + std::chrono::seconds(1))};
	auto clients = server.GetClients();
	ASSERT_GE(clients.size(), 1); // the client must not have been disconnected
	// the server side Lua events should have been called
	ASSERT_TRUE(lua_was_called["onClientConnected"].get_or(false));
	ASSERT_TRUE(lua_was_called["onUserLogin"].get_or(false));
	auto test_client = *clients.cbegin();
	auto last_confirmed_state = test_client->GetLastConfirmedStateID();
	// the inbound message queues should not have any partially transmitted messages
	size_t unfullfilled_messages_from_client = test_client->GetPartialMessageCount();
	size_t unfullfilled_messages_from_server = connection.GetPartialMessageCount();
	EXPECT_EQ(unfullfilled_messages_from_client, 0);
	EXPECT_EQ(unfullfilled_messages_from_server, 0);
	// our promises must have been set
	ASSERT_EQ(client_command_received, std::future_status::ready);
	ASSERT_EQ(client_id_received, std::future_status::ready);

	// stop services and threads
	test_running = false;
	response_thread.join();

	server.Stop();
	connection.Stop();
	server_thread.join();
	client_asio_thread.join();

	// final checks
	// did we login as the test user created earlier?
	EXPECT_EQ(client_id.get(), test_user->GetEntityId());
	std::shared_ptr<ClientCommandsEvent> result_event = client_reply.last_event.get();
	// did the server get our command message?
	ASSERT_TRUE(result_event);
	// did the server correctly parse our command message?
	EXPECT_EQ(last_confirmed_state, 42);
	auto& client_commands = result_event->client_commands;
	EXPECT_TRUE(client_commands.has_laststateid());
	EXPECT_EQ(client_commands.laststateid(), 42);
	ASSERT_EQ(client_commands.commandlist_size(), 1);
	EXPECT_EQ(client_commands.commandlist(0), teststring);
}
} // namespace networking
} // namespace tec
