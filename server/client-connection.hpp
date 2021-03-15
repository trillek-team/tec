#pragma once

#include <asio.hpp>
#include <deque>
#include <map>
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

	void QueueWrite(MessagePool::ptr_type msg);
	void QueueWrite(MessageOut& msg);
	void QueueWrite(MessageOut&& msg);

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

	MessageOut PrepareGameStateUpdateMessage(state_id_t current_state_id, uint64_t current_timestamp);

	size_t GetPartialMessageCount() const { return read_messages.size(); }

private:
	void read_header();

	void read_body();

	void process_message(MessageIn&);

	void do_write();

	// peer connection
	tcp::socket socket;
	// address of peer
	tcp::endpoint endpoint;
	// message fragment being read
	MessagePool::ptr_type current_read_msg;
	// message fragments in-progress or waiting to be written
	std::deque<MessagePool::ptr_type> write_msg_queue;
	// we must assure that this stream performs only a single write operation at a time
	std::mutex write_msg_mutex;
	// composite messages currently being read
	std::map<uint32_t, std::unique_ptr<MessageIn>> read_messages;

	Server* server;
	eid id{0};
	proto::Entity entity;

	std::shared_ptr<FPSController> controller;

	state_id_t last_confirmed_state_id{0}; // That last state_id the client confirmed it received.
	state_id_t last_recv_command_id{0};
	GameState state_changes_since_confirmed; // That state changes that happened since last_confirmed_state_id.
};
} // end namespace networking
} // end namespace tec
