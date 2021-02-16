#pragma once

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

public:
	ServerStats& stats;

private:
	GameState base_state;
};

} // end namespace tec
