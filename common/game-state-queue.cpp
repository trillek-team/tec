#include "game-state-queue.hpp"

#include <glm/gtx/compatibility.hpp>
#include "components/transforms.hpp"

namespace tec {
	// TODO: Move ProcessEventQueue into ProcessEvents method.
	void GameStateQueue::Interpolate(const double delta_time) {
		EventQueue<EntityCreated>::ProcessEventQueue();
		EventQueue<EntityDestroyed>::ProcessEventQueue();
		EventQueue<NewGameStateEvent>::ProcessEventQueue();

		std::lock_guard<std::mutex> lock(this->server_state_mutex);
		static const double INTERPOLATION_RATE = 10.0 / 60.0;

		if (this->server_states.size() > 5) {
			std::cout << "getting flooded by state updates" << std::endl;
		}
		if (this->server_states.size() > 2) {
			interpolation_accumulator += delta_time;
			if (interpolation_accumulator > INTERPOLATION_RATE) {
				const GameState& to_state = this->server_states.front();
				for (auto position : to_state.positions) {
					this->base_state.positions[position.first] = position.second;
				}
				for (auto velocity : to_state.velocities) {
					this->base_state.velocities[velocity.first] = velocity.second;
				}
				for (auto orientation : to_state.orientations) {
					this->base_state.orientations[orientation.first] = orientation.second;
				}
				interpolation_accumulator -= INTERPOLATION_RATE;
				this->base_state.state_id = to_state.state_id;
				this->server_states.pop();
			}
			const GameState& to_state = this->server_states.front();
			float lerp_percent = static_cast<float>(interpolation_accumulator / (INTERPOLATION_RATE * (to_state.state_id - this->base_state.state_id)));
			if (lerp_percent > 0.0) {
				for (auto position : to_state.positions) {
					if (this->base_state.positions.find(position.first) != this->base_state.positions.end()) {
						this->interpolated_state.positions[position.first].value = glm::lerp(
							base_state.positions.at(position.first).value, position.second.value, lerp_percent);
					}
					else {
						this->interpolated_state.positions[position.first] = position.second;
					}
				}
				for (auto velocity : to_state.velocities) {
					if (this->base_state.velocities.find(velocity.first) != this->base_state.velocities.end()) {
						this->interpolated_state.velocities[velocity.first].linear = glm::lerp(
							base_state.velocities.at(velocity.first).linear, velocity.second.linear, lerp_percent);
						this->interpolated_state.velocities[velocity.first].angular = glm::lerp(
							base_state.velocities.at(velocity.first).angular, velocity.second.angular, lerp_percent);
					}
					else {
						this->interpolated_state.velocities[velocity.first].linear = velocity.second.linear;
						this->interpolated_state.velocities[velocity.first].angular = velocity.second.angular;
					}
				}
				for (auto orientation : to_state.orientations) {
					if (this->base_state.orientations.find(orientation.first) != this->base_state.orientations.end()) {
						this->interpolated_state.orientations[orientation.first].value = glm::slerp(
							base_state.orientations.at(orientation.first).value, orientation.second.value, lerp_percent);
					}
					else {
						this->interpolated_state.orientations[orientation.first] = orientation.second;
					}
				}
			}
		}
	}


	void GameStateQueue::On(std::shared_ptr<NewGameStateEvent> data) {
		QueueServerState(std::move(data->new_state));
	}

	void GameStateQueue::QueueServerState(GameState&& new_state) {
		if (new_state.state_id > this->last_server_state_id) {
			this->last_server_state_id = new_state.state_id;
			std::lock_guard<std::mutex> lock(this->server_state_mutex);
			this->server_states.emplace(std::move(new_state));
		}
	}

	void GameStateQueue::RemoveEntity(eid entity_id) {
		if (this->interpolated_state.positions.find(entity_id) != this->interpolated_state.positions.end()) {
			this->interpolated_state.positions.erase(entity_id);
			this->base_state.positions.erase(entity_id);
		}
		if (this->interpolated_state.orientations.find(entity_id) != this->interpolated_state.orientations.end()) {
			this->interpolated_state.orientations.erase(entity_id);
			this->base_state.orientations.erase(entity_id);
		}
		if (this->interpolated_state.velocities.find(entity_id) != this->interpolated_state.velocities.end()) {
			this->interpolated_state.velocities.erase(entity_id);
			this->base_state.velocities.erase(entity_id);
		}
	}

	void GameStateQueue::On(std::shared_ptr<EntityCreated> data) {
		SetInitialEntityState(data->entity);
	}

	void GameStateQueue::SetInitialEntityState(const proto::Entity& entity) {
		eid entity_id = entity.id();
		for (int i = 0; i < entity.components_size(); ++i) {
			const proto::Component& comp = entity.components(i);
			switch (comp.component_case()) {
			case proto::Component::kPosition:
			{
				Position pos;
				pos.In(comp);
				this->interpolated_state.positions[entity_id] = pos;
				this->base_state.positions[entity_id] = pos;
			}
			break;
			case proto::Component::kOrientation:
			{
				Orientation orientation;
				orientation.In(comp);
				this->interpolated_state.orientations[entity_id] = orientation;
				this->base_state.orientations[entity_id] = orientation;
			}
			break;
			case proto::Component::kVelocity:
			{
				Velocity vel;
				vel.In(comp);
				this->interpolated_state.velocities[entity_id] = vel;
				this->base_state.velocities[entity_id] = vel;
			}
			break;
			}
		}
	}

	void GameStateQueue::On(std::shared_ptr<EntityDestroyed> data) {
		RemoveEntity(data->entity_id);
	}
}