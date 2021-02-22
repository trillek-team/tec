#include "server-game-state-queue.hpp"

namespace tec {

ServerGameStateQueue::ServerGameStateQueue(ServerStats& s): stats(s) {}

void ServerGameStateQueue::ProcessEventQueue() {
	EventQueue<EntityCreated>::ProcessEventQueue();
	EventQueue<EntityDestroyed>::ProcessEventQueue();
}

void ServerGameStateQueue::On(std::shared_ptr<EntityCreated> data) {
	const proto::Entity& entity = data->entity;
	eid entity_id = entity.id();
	for (int i = 0; i < entity.components_size(); ++i) {
		const proto::Component& comp = entity.components(i);
		switch (comp.component_case()) {
		case proto::Component::kPosition:
		{
			Position pos;
			pos.In(comp);
			this->base_state.positions[entity_id] = pos;
			break;
		}
		case proto::Component::kOrientation:
		{
			Orientation orientation;
			orientation.In(comp);
			this->base_state.orientations[entity_id] = orientation;
			break;
		}
		case proto::Component::kVelocity:
		{
			Velocity vel;
			vel.In(comp);
			this->base_state.velocities[entity_id] = vel;
			break;
		}
		default: break;
		}
	}
}

void ServerGameStateQueue::On(std::shared_ptr<EntityDestroyed> data) {
	const eid entity_id = data->entity_id;
	this->base_state.positions.erase(entity_id);
	this->base_state.orientations.erase(entity_id);
	this->base_state.velocities.erase(entity_id);
}

} // end namespace tec
