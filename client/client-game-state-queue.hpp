#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

#include "event-queue.hpp"
#include "event-system.hpp"
#include "game-state.hpp"
#include "server-stats.hpp"
#include "tec-types.hpp"

namespace tec {

class ClientGameStateQueue:
		public EventQueue<EntityCreated>,
		public EventQueue<EntityDestroyed>,
		public EventQueue<NewGameStateEvent> {
public:
	ClientGameStateQueue(ServerStats& s);

	void Interpolate(const double delta_time);

	void QueueServerState(GameState&& new_state);

	void UpdatePredictions(GameState& new_state);

	void CheckPredictionResult(GameState& new_state);

	void ProcessEventQueue();

	void SetClientID(eid _client_id) { this->client_id = _client_id; }

	void SetCommandID(state_id_t _command_id) { this->command_id = _command_id; }

	using EventQueue<EntityCreated>::On;
	using EventQueue<EntityDestroyed>::On;
	using EventQueue<NewGameStateEvent>::On;
	virtual void On(std::shared_ptr<EntityCreated> data);
	virtual void On(std::shared_ptr<EntityDestroyed> data);
	virtual void On(std::shared_ptr<NewGameStateEvent> data);

	GameState& GetInterpolatedState() { return this->interpolated_state; }

	GameState& GetBaseState() { return this->base_state; }

	void SetBaseState(GameState&& new_state) { this->base_state = std::move(new_state); }

	GameState* GetGameState(int offset) {
		return &this->server_states_array[(server_state_array_index - offset) % SERVER_STATES_ARRAY_SIZE];
	}

private:
	static const unsigned int SERVER_STATES_ARRAY_SIZE{5};

	GameState server_states_array[SERVER_STATES_ARRAY_SIZE];
	int server_state_array_index{SERVER_STATES_ARRAY_SIZE - 1};

	ServerStats& stats;
	GameState base_state;
	GameState interpolated_state;
	std::queue<GameState> server_states;
	std::mutex server_state_mutex;
	state_id_t last_server_state_id{0};
	state_id_t command_id{0};
	double interpolation_accumulator{0.0};
	eid client_id{0};
	std::map<state_id_t, GameState> predictions;
};

} // end namespace tec
