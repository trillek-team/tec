#pragma once

#include <deque>
#include <mutex>
#include <set>

#include <asio.hpp>
#include <components.pb.h>

#include "components/lua-script.hpp"
#include "lua-system.hpp"

#include "event-queue.hpp"
#include "event-system.hpp"
#include "events.hpp"
#include "net-message.hpp"

using asio::ip::tcp;

namespace tec {
namespace networking {

extern unsigned short PORT;
static eid BASE_ENTITY_ID = 10000;

class ClientConnection;

class Server : public EventQueue<EntityCreated>, public EventQueue<EntityDestroyed> {
public:
	Server(tcp::endpoint& endpoint);

	// Deliver a message to all clients.
	// save_to_recent is used to save a recent list of message each client gets when they connect.
	void Deliver(const ServerMessage& msg, bool save_to_recent = true);

	// Deliver a message to a specific client.
	void Deliver(std::shared_ptr<ClientConnection> client, const ServerMessage& msg);

	// Calls when a client leaves, usually when the connection is no longer valid.
	void Leave(std::shared_ptr<ClientConnection> client);

	void Start();

	void Stop();

	// Get a list of all connected clients.
	const std::set<std::shared_ptr<ClientConnection>>& GetClients() { return this->clients; }

	// For calling ProcessEvents() in main.cpp
	LuaSystem* GetLuaSystem() { return &this->lua_sys; }

	using EventQueue<EntityCreated>::On;
	using EventQueue<EntityDestroyed>::On;
	void On(std::shared_ptr<EntityCreated> data);
	void On(std::shared_ptr<EntityDestroyed> data);

private:
	// Method that handles and accepts incoming connections.
	void AcceptHandler();

	// Lua system
	LuaSystem lua_sys;

	// ASIO variables
	asio::io_context io_context;
	tcp::acceptor acceptor;
	tcp::socket peer_socket;
	tcp::endpoint peer_endpoint;

	// Server event log
	std::shared_ptr<spdlog::logger> _log;

	ServerMessage greeting_msg; // Greeting chat message.

	std::map<eid, proto::Entity> entities;

	std::set<std::shared_ptr<ClientConnection>> clients; // All connected clients.
	std::uint64_t base_id = BASE_ENTITY_ID; // Starting client_id

	// Recent message list all clients get on connecting,
	enum { max_recent_msgs = 100 };
	std::deque<ServerMessage> recent_msgs;
	static std::mutex recent_msgs_mutex;

public:
	std::mutex client_list_mutex;
};
} // namespace networking
} // namespace tec
