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

class ClientConnection;

class Server : public EventQueue<EntityCreated>, public EventQueue<EntityDestroyed> {
public:
	Server(tcp::endpoint& endpoint);

	// Deliver a message to all clients.
	// save_to_recent is used to save a recent list of message each client gets when they connect.
	void Deliver(MessageOut& msg, bool save_to_recent = true);
	void Deliver(MessageOut&& msg, bool save_to_recent = true);

	// Deliver a message to a specific client.
	void Deliver(std::shared_ptr<ClientConnection> client, MessageOut& msg);
	void Deliver(std::shared_ptr<ClientConnection> client, MessageOut&& msg);

	// Calls when a client connects. This provides a chance to reject the client before joining the world.
	bool OnConnect();

	// Calls when a client leaves, usually when the connection is no longer valid.
	void OnDisconnect(std::shared_ptr<ClientConnection> client);

	void Start();

	void Stop();

	// Get a list of all connected clients.
	const std::set<std::shared_ptr<ClientConnection>>& GetClients() { return this->clients; }

	// For calling ProcessEvents() in main.cpp
	LuaSystem* GetLuaSystem() { return &this->lua_sys; }

	void ProcessEvents();

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

	MessageOut greeting_msg; // Greeting chat message.

	std::map<eid, proto::Entity> entities;

	// Sends the "world" to a given client including all entities and recent chats.
	void SendWorld(std::shared_ptr<ClientConnection> client);

	friend class ClientConnection;

	std::set<std::shared_ptr<ClientConnection>> clients; // All connected clients.

	// Recent message list all clients get on connecting,
	enum { max_recent_msgs = 100 };
	std::deque<std::unique_ptr<MessageOut>> recent_msgs;
	static std::mutex recent_msgs_mutex;

public:
	std::mutex client_list_mutex;
};
} // namespace networking
} // namespace tec
