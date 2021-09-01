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

class ServerGameStateQueue : public EventQueue<EntityCreated>, public EventQueue<EntityDestroyed> {
public:
	ServerGameStateQueue(ServerStats& s);

	virtual void On(eid, std::shared_ptr<EntityCreated> data) override;
	virtual void On(eid, std::shared_ptr<EntityDestroyed> data) override;

	void ProcessEventQueue();

	GameState& GetBaseState() { return this->base_state; }

	void SetBaseState(GameState&& new_state) { this->base_state = std::move(new_state); }

public:
	ServerStats& stats;

private:
	GameState base_state;
};

} // end namespace tec
