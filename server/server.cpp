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
	std::string message("Hello from server\n");
	this->greeting_msg = MessagePool::make_unique();
	this->greeting_msg->SetBodyLength(message.size());
	memcpy(this->greeting_msg->GetBodyPTR(), message.c_str(), this->greeting_msg->GetBodyLength());
	this->greeting_msg->encode_header();

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
void Server::Deliver(const NetMessage::ptr_type& msg, bool save_to_recent) {
	if (save_to_recent) {
		std::lock_guard<std::mutex> lg(recent_msgs_mutex);
		this->recent_msgs.push_back(std::make_unique<NetMessage>(*msg));
		while (this->recent_msgs.size() > max_recent_msgs) {
			this->recent_msgs.pop_front();
		}
	}

	std::lock_guard<std::mutex> lg(client_list_mutex);
	for (auto client : this->clients) {
		client->QueueWrite(std::make_unique<NetMessage>(*msg));
	}
}

void Server::Deliver(std::shared_ptr<ClientConnection> client, const NetMessage::ptr_type& msg) {
	client->QueueWrite(std::make_unique<NetMessage>(*msg));
}

void Server::Leave(std::shared_ptr<ClientConnection> client) {
	// setup a lua object for this event
	client_connection_info info_event;
	info_event.from_endpoint(client->GetEndpoint());

	// call lua scripts that want to know about connection going away
	this->lua_sys.CallFunctions("onClientDisconnected", &info_event);

	eid leaving_client_id = client->GetID();
	client->DoLeave(); // Send out entity destroyed events and client leave messages.

	{
		std::lock_guard lg(this->client_list_mutex);
		this->clients.erase(client);
	}

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

			auto reject_msg = MessagePool::make_unique(); // default type is CHAT_MESSAGE
			reject_msg->SetBodyLength(reject_reason.size());
			memcpy(reject_msg->GetBodyPTR(), reject_reason.data(), reject_msg->GetBodyLength());
			reject_msg->encode_header();

			asio::write(this->peer_socket, asio::buffer(reject_msg->GetDataPTR(), reject_msg->length()));
			// hopefully the client side survives long enough to get our message
			this->peer_socket.wait(asio::ip::tcp::socket::wait_write);
			this->peer_socket.close();
			this->AcceptHandler();
			return;
		}
		// write the standard greeting
		asio::write(this->peer_socket, asio::buffer(greeting_msg->GetDataPTR(), greeting_msg->length()));

		std::shared_ptr<ClientConnection> client =
				std::make_shared<ClientConnection>(std::move(this->peer_socket), std::move(this->peer_endpoint), this);

		// this protopack contains a renderable component for others to see
		static FilePath others_protopack = FilePath::GetAssetPath("protopacks/others.json");
		proto::Entity other_entity;
		LoadProtoPack(others_protopack, other_entity);

		client->SetID(++base_id);
		client->DoJoin();

		auto connecting_client_entity_msg = MessagePool::make_shared();
		other_entity.set_id(client->GetID());
		connecting_client_entity_msg->SetBodyLength(other_entity.ByteSizeLong());
		other_entity.SerializeToArray(
				connecting_client_entity_msg->GetBodyPTR(),
				static_cast<int>(connecting_client_entity_msg->GetBodyLength()));
		connecting_client_entity_msg->SetMessageType(MessageType::ENTITY_CREATE);
		connecting_client_entity_msg->encode_header();

		auto other_client_entity_msg = MessagePool::make_shared();
		for (auto other_client : clients) {
			other_entity.set_id(other_client->GetID());
			other_client_entity_msg->SetBodyLength(other_entity.ByteSizeLong());
			other_entity.SerializeToArray(
					other_client_entity_msg->GetBodyPTR(), static_cast<int>(other_client_entity_msg->GetBodyLength()));
			other_client_entity_msg->SetMessageType(MessageType::ENTITY_CREATE);
			other_client_entity_msg->encode_header();

			client->QueueWrite(other_client_entity_msg);
			other_client->QueueWrite(connecting_client_entity_msg);
		}
		for (auto entity : entities) {
			other_client_entity_msg->SetBodyLength(entity.second.ByteSizeLong());
			entity.second.SerializeToArray(
					other_client_entity_msg->GetBodyPTR(), static_cast<int>(other_client_entity_msg->GetBodyLength()));
			other_client_entity_msg->SetMessageType(MessageType::ENTITY_CREATE);
			other_client_entity_msg->encode_header();

			client->QueueWrite(other_client_entity_msg);
		}

		{
			std::lock_guard lg(this->client_list_mutex);
			clients.insert(client);
		}
		{
			std::lock_guard<std::mutex> lg(recent_msgs_mutex);
			for (auto msg : this->recent_msgs) {
				client->QueueWrite(msg);
			}
		}
		client->StartRead();

		AcceptHandler();
	});
}
} // namespace networking
} // namespace tec
