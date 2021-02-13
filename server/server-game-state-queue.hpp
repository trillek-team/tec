#ifndef TRILLEK_SERVER_GAME_STATE_QUEUE_HPP
#define TRILLEK_SERVER_GAME_STATE_QUEUE_HPP

#include <queue>
#include <iostream>
#include <mutex>
#include <memory>

#include "event-queue.hpp"
#include "event-system.hpp"
#include "game-state.hpp"
#include "server-stats.hpp"
#include "tec-types.hpp"

namespace tec {

class ServerGameStateQueue :
	public EventQueue<EntityCreated>,
	public EventQueue<EntityDestroyed> {
public:
	ServerGameStateQueue(ServerStats& s);

	using EventQueue<EntityCreated>::On;
	using EventQueue<EntityDestroyed>::On;
	virtual void On(std::shared_ptr<EntityCreated> data);
	virtual void On(std::shared_ptr<EntityDestroyed> data);

	void ProcessEventQueue();

	GameState& GetBaseState() {
		return this->base_state;
	}

	void SetBaseState(GameState&& new_state) {
		this->base_state = std::move(new_state);
	}

private:
	static const unsigned int SERVER_STATES_ARRAY_SIZE{ 5 };

	GameState server_states_array[SERVER_STATES_ARRAY_SIZE];
	int server_state_array_index{ SERVER_STATES_ARRAY_SIZE - 1 };
	GameState predicted_states[SERVER_STATES_ARRAY_SIZE];
	int predicted_states_array_index{ 0 };

	ServerStats& stats;
	GameState base_state;
	//GameState interpolated_state;
	std::queue<GameState> server_states;
	std::mutex server_state_mutex;
	//state_id_t last_server_state_id{ 0 };
	//state_id_t command_id{ 0 };
	//double interpolation_accumulator{ 0.0 };
	//eid client_id{ 0 };
	//std::map<state_id_t, Position> predictions;
};

} // end namespace tec
#endif
