#pragma once

#include <queue>
#include <iostream>
#include <mutex>
#include <memory>

#include "event-queue.hpp"
#include "event-system.hpp"
#include "game-state.hpp"
#include "types.hpp"

namespace tec {
	class GameStateQueue : public EventQueue<EntityCreated>,
		public EventQueue<EntityDestroyed>, public EventQueue<NewGameStateEvent> {
	public:
		GameStateQueue() : last_server_state_id(0) { }
		~GameStateQueue() { }

		void Interpolate(const double delta_time);

		void QueueServerState(GameState&& new_state);

		void ProcessEventQueue();

		void On(std::shared_ptr<EntityCreated> data);
		void On(std::shared_ptr<EntityDestroyed> data);
		void On(std::shared_ptr<NewGameStateEvent> data);

		const GameState& GetInterpolatedState() {
			return this->interpolated_state;
		}

		const GameState& GetBaseState() {
			return this->base_state;
		}

		void SetBaseState(GameState&& new_state) {
			this->base_state = std::move(new_state);
		}
	private:

		void SetInitialEntityState(const proto::Entity& entity);
		void RemoveEntity(eid entity_id);

		GameState base_state;
		GameState interpolated_state;
		std::queue<GameState> server_states;
		std::mutex server_state_mutex;
		state_id_t last_server_state_id;
		double interpolation_accumulator = 0.0;
	};
}