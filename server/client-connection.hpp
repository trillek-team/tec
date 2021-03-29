#pragma once

#include <asio.hpp>
#include <deque>
#include <map>
#include <memory>
#include <mutex>

#include "game-state.hpp"
#include "net-message.hpp"
#include "tec-types.hpp"
#include "user/user.hpp"

using asio::ip::tcp;

namespace tec {
using namespace user;

namespace networking {
class Server;

// Used to represent a client connection to the server.
class ClientConnection : public std::enable_shared_from_this<ClientConnection> {
public:
	ClientConnection(tcp::socket _socket, tcp::endpoint _endpoint, Server* server);

	~ClientConnection() { this->socket.close(); }

	void StartRead();

	void QueueWrite(MessagePool::ptr_type msg);
	void QueueWrite(MessageOut& msg);
	void QueueWrite(MessageOut&& msg);

	eid GetID() { return this->user ? this->user->GetEntityId() : 0; }

	tcp::endpoint GetEndpoint() { return this->endpoint; }

	// Called when a client is entering the world. Such as after the have logged in.
	void OnJoinWorld();

	// Called when a client is leaving the world. Such as after the have logged out.
	void OnLeaveWorld();

	// Called when another client leaves.
	void OnOtherLeaveWorld(eid entity_id);

	void ConfirmStateID(state_id_t state_id) { this->last_confirmed_state_id = state_id; }

	state_id_t GetLastConfirmedStateID() { return this->last_confirmed_state_id; }

	void UpdateGameState(const GameState& full_state);

	MessageOut PrepareGameStateUpdateMessage(state_id_t current_state_id, uint64_t current_timestamp);

	size_t GetPartialMessageCount() const { return read_messages.size(); }

	bool ReadyToReceive() const { return this->ready_to_recv_states; }

	static void RegisterLuaType(sol::state&);

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

	User* user{nullptr};

	state_id_t last_confirmed_state_id{0}; // That last state_id the client confirmed it received.
	state_id_t last_recv_command_id{0};
	GameState state_changes_since_confirmed; // That state changes that happened since last_confirmed_state_id.

	bool ready_to_recv_states{false};
};
} // end namespace networking
} // end namespace tec
