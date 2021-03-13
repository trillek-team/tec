#pragma once

#include <asio.hpp>
#include <deque>
#include <memory>
#include <mutex>

#include "game-state.hpp"
#include "net-message.hpp"
#include "tec-types.hpp"

using asio::ip::tcp;

namespace tec {
struct FPSController;

namespace networking {
class Server;

// Used to represent a client connection to the server.
class ClientConnection : public std::enable_shared_from_this<ClientConnection> {
public:
	ClientConnection(tcp::socket _socket, tcp::endpoint _endpoint, Server* server);

	~ClientConnection();

	void StartRead();

	void QueueWrite(NetMessage::shared_type msg);

	eid GetID() { return this->id; }

	tcp::endpoint GetEndpoint() { return this->endpoint; }

	// Sets the client id and sends it to this client.
	void SetID(eid id);

	proto::Entity& GetEntity() { return this->entity; }

	void DoJoin(); // Emits an EntityCreated event and ENTITY_CREATE message to this client.

	void DoLeave(); // Emits an EntityDestroyed event.

	// Called when another client leaves.
	void OnClientLeave(eid entity_id);

	void ConfirmStateID(state_id_t state_id) { this->last_confirmed_state_id = state_id; }

	state_id_t GetLastConfirmedStateID() { return this->last_confirmed_state_id; }

	void UpdateGameState(const GameState& full_state);

	NetMessage::ptr_type PrepareGameStateUpdateMessage(state_id_t current_state_id, uint64_t current_timestamp);

private:
	void read_header();

	void read_body();

	void process_message();

	void do_write();

	tcp::socket socket;
	tcp::endpoint endpoint;
	NetMessage::ptr_type current_read_msg;
	std::deque<NetMessage::shared_type> write_msgs_;
	Server* server;
	eid id{0};
	proto::Entity entity;
	std::mutex write_msg_mutex;

	std::shared_ptr<FPSController> controller;

	state_id_t last_confirmed_state_id{0}; // That last state_id the client confirmed it received.
	state_id_t last_recv_command_id{0};
	GameState state_changes_since_confirmed; // That state changes that happened since last_confirmed_state_id.
};
} // end namespace networking
} // end namespace tec
