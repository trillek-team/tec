#include <fstream>
#include <iostream>
#include <thread>

#include <components.pb.h>

#include "client-connection.hpp"
#include "proto-load.hpp"
#include "server.hpp"

using asio::ip::tcp;

namespace tec {
namespace networking {
unsigned short PORT = 0xa10c;
std::mutex Server::recent_msgs_mutex;

// proxy structure passed to lua functions: onClientConnected onClientDisconnected
// for connect, setting cancel to true will reject the connection with the "reason" string sent to the client
struct client_connection_info {
	bool cancel;
	std::string reason;
	int port;
	std::string address;
	std::string family;
	std::string protocol;

	void from_endpoint(const asio::ip::tcp::endpoint& endpoint) {
		this->port = endpoint.port();
		this->address = endpoint.address().to_string();
		this->family = endpoint.address().is_v6() ? "ipv6" : "ipv4";
		this->protocol = "tcp";
	}

	void from_endpoint(const asio::ip::udp::endpoint& endpoint) {
		this->port = endpoint.port();
		this->address = endpoint.address().to_string();
		this->family = endpoint.address().is_v6() ? "ipv6" : "ipv4";
		this->protocol = "udp";
	}
};

Server::Server(tcp::endpoint& endpoint) : acceptor(io_context, endpoint), peer_socket(io_context) {
	_log = spdlog::get("console_log");

	// Create a simple greeting chat message that all clients get.
	this->greeting_msg.FromString(std::string{"Hello from server\n"});

	asio::ip::tcp::no_delay option(true);
	acceptor.set_option(option);

	this->lua_sys.GetGlobalState().new_usertype<client_connection_info>(
			"client_connection_info",
			"cancel",
			&client_connection_info::cancel,
			"reason",
			&client_connection_info::reason,
			"port",
			sol::readonly(&client_connection_info::port),
			"address",
			sol::readonly(&client_connection_info::address),
			"family",
			sol::readonly(&client_connection_info::family),
			"protocol",
			sol::readonly(&client_connection_info::protocol));

	// Load test script
	FilePath fp = FilePath::GetAssetPath("scripts/server-test.lua");
	if (fp.FileExists()) {
		this->lua_sys.LoadFile(fp);
	}

	AcceptHandler();

	tcp::endpoint my_endpoint = acceptor.local_endpoint();
	if (acceptor.is_open()) {
		_log->info("Server ready, listening on [{}]:{}", my_endpoint.address().to_string(), my_endpoint.port());
	}
	else {
		_log->error(
				"Server failed to open listen socket on [{}]:{}",
				my_endpoint.address().to_string(),
				my_endpoint.port());
	}
}

// TODO: Implement a method to deliver a message to all clients except the source.
void Server::Deliver(MessageOut& msg, bool save_to_recent) {
	if (save_to_recent) {
		std::lock_guard<std::mutex> lg(recent_msgs_mutex);
		this->recent_msgs.push_back(std::make_unique<MessageOut>(msg));
		while (this->recent_msgs.size() > max_recent_msgs) {
			this->recent_msgs.pop_front();
		}
	}

	std::lock_guard<std::mutex> lg(client_list_mutex);
	for (auto client : this->clients) {
		client->QueueWrite(msg);
	}
}
void Server::Deliver(MessageOut&& msg, bool save_to_recent) { Deliver(msg, save_to_recent); }
void Server::Deliver(std::shared_ptr<ClientConnection> client, MessageOut& msg) { client->QueueWrite(msg); }
void Server::Deliver(std::shared_ptr<ClientConnection> client, MessageOut&& msg) { client->QueueWrite(std::move(msg)); }

void Server::Leave(std::shared_ptr<ClientConnection> client) {
	{
		std::lock_guard lg(this->client_list_mutex);
		auto which_client = this->clients.find(client);
		if (which_client == this->clients.end()) {
			// invalid client or already called Leave()
			return;
		}
		this->clients.erase(which_client);
	}

	// setup a lua object for this event
	client_connection_info info_event;
	info_event.from_endpoint(client->GetEndpoint());

	// call lua scripts that want to know about connection going away
	this->lua_sys.CallFunctions("onClientDisconnected", &info_event);

	eid leaving_client_id = client->GetID();
	client->DoLeave(); // Send out entity destroyed events and client leave messages.

	// Notify other clients that a client left.
	for (auto _client : this->clients) {
		_client->OnClientLeave(leaving_client_id);
	}
}

void Server::Start() { this->io_context.run(); }

void Server::Stop() {
	{
		std::lock_guard<std::mutex> lg(client_list_mutex);
		this->clients.clear();
	}
	this->io_context.stop();
}

void Server::On(std::shared_ptr<EntityCreated> data) { this->entities[data->entity_id] = data->entity; }

void Server::On(std::shared_ptr<EntityDestroyed> data) { this->entities.erase(data->entity_id); }

void Server::AcceptHandler() {
	acceptor.async_accept(peer_socket, peer_endpoint, [this](std::error_code error) {
		EventQueue<EntityCreated>::ProcessEventQueue();
		EventQueue<EntityDestroyed>::ProcessEventQueue();
		if (error) {
			_log->error("Server::AcceptHandler async_accept[]: socket error: {}: {}", error.value(), error.message());
			this->AcceptHandler();
			return;
		}
		// setup a lua object for this event
		client_connection_info info_event;
		info_event.from_endpoint(peer_endpoint);
		info_event.cancel = false;
		info_event.reason = "Server policy rejected the connection"; // default reason

		// call lua scripts that want to know about connections
		this->lua_sys.CallFunctions("onClientConnected", &info_event);

		// a lua method doesn't want this client, tell them to go away
		if (info_event.cancel) {
			// let the server log know
			_log->warn(
					"Connection from [{}]:{} rejected by script: {}",
					info_event.address,
					info_event.port,
					info_event.reason);

			std::string reject_reason = info_event.reason;
			reject_reason.push_back('\n');

			auto reject_msg = MessagePool::get(); // default type is CHAT_MESSAGE
			reject_msg->SetBodyLength(reject_reason.size());
			memcpy(reject_msg->GetBodyPTR(), reject_reason.data(), reject_msg->GetBodyLength());
			reject_msg->encode_header();

			asio::write(this->peer_socket, reject_msg->buffer());
			// hopefully the client side survives long enough to get our message
			this->peer_socket.wait(asio::ip::tcp::socket::wait_write);
			this->peer_socket.close();
			this->AcceptHandler();
			return;
		}

		// promote the client to a full object
		std::shared_ptr<ClientConnection> client =
				std::make_shared<ClientConnection>(std::move(this->peer_socket), std::move(this->peer_endpoint), this);

		// write the standard greeting
		client->QueueWrite(greeting_msg);

		// this protopack contains a renderable component for others to see
		static FilePath others_protopack = FilePath::GetAssetPath("protopacks/others.json");
		proto::Entity other_entity;
		LoadProtoPack(others_protopack, other_entity);

		client->SetID(++base_id);
		client->DoJoin();

		MessageOut connecting_client_entity_msg(MessageType::ENTITY_CREATE);
		other_entity.set_id(client->GetID());
		other_entity.SerializeToZeroCopyStream(&connecting_client_entity_msg);

		for (auto other_client : clients) {
			MessageOut other_client_entity_msg(MessageType::ENTITY_CREATE);
			other_entity.set_id(other_client->GetID());
			other_entity.SerializeToZeroCopyStream(&other_client_entity_msg);

			client->QueueWrite(other_client_entity_msg);
			other_client->QueueWrite(connecting_client_entity_msg);
		}
		for (auto entity : entities) {
			MessageOut other_client_entity_msg(MessageType::ENTITY_CREATE);
			entity.second.SerializeToZeroCopyStream(&other_client_entity_msg);

			client->QueueWrite(other_client_entity_msg);
		}

		{
			std::lock_guard lg(this->client_list_mutex);
			clients.insert(client);
		}
		{
			std::lock_guard<std::mutex> lg(recent_msgs_mutex);
			for (auto& msg : this->recent_msgs) {
				client->QueueWrite(*msg);
			}
		}
		client->StartRead();

		_log->debug("Server::AcceptHandler complete");
		AcceptHandler();
	});
}
} // namespace networking
} // namespace tec
