#include "client-game-state-queue.hpp"
#include "simulation.hpp"

#include <glm/gtx/compatibility.hpp>

#include "components/transforms.hpp"

namespace tec {

ClientGameStateQueue::ClientGameStateQueue(ServerStats& s) : stats(s) {}

void ClientGameStateQueue::Interpolate(const double delta_time) {
	std::lock_guard<std::mutex> lg(this->server_state_mutex);
	stats.server_state_count = this->server_states.size();
	if (this->server_states.size() > 10) { // so many!
		std::cout << "getting flooded by state updates" << std::endl;
	}
	if (this->server_states.size() < 1) { // no states means nothing to do here
		return;
	}
	// this should be the rate at which we receive updates from the server
	// used to estimate how far we are before the next update gets received
	const double INTERPOLATION_RATE = UPDATE_RATE;
	interpolation_accumulator += delta_time;
	const GameState& to_state = this->server_states.front();
	stats.last_state_time = to_state.timestamp;

	// the interpolation_accumulator could be thought of as where we are between two states
	// if the incoming states are timestamped, it would probably be better to use that
	// instead of guessing like this
	if (interpolation_accumulator >= INTERPOLATION_RATE) {
		// remove a state from the queue and make it the base state
		// also initialize the interpolated state with it
		if (this->client_id != 0) {
			this->predictions.emplace(std::make_pair(this->command_id, this->interpolated_state.positions[this->client_id]));
		}
		for (auto position : to_state.positions) {
			this->base_state.positions[position.first] = position.second;
			this->interpolated_state.positions[position.first] = position.second;
		}
		if (this->client_id != 0) {
			this->base_state.positions[this->client_id] = this->predictions.at(this->command_id);
			this->interpolated_state.positions[this->client_id] = this->predictions.at(this->command_id);
		}
		for (auto velocity : to_state.velocities) {
			this->base_state.velocities[velocity.first] = velocity.second;
			this->interpolated_state.velocities[velocity.first] = velocity.second;
		}
		for (auto orientation : to_state.orientations) {
			this->base_state.orientations[orientation.first] = orientation.second;
			this->interpolated_state.orientations[orientation.first] = orientation.second;
		}
		// when too many states, discard the rest! what could possibly go wrong!?
		// we just interpolate between them anyways
		while (interpolation_accumulator >= INTERPOLATION_RATE) {
			interpolation_accumulator -= INTERPOLATION_RATE;
			this->base_state.state_id = to_state.state_id;
			this->server_states.pop();
		}
	}
	else {
		// figure out how many states we are behind
		// this should likely use timestamps instead...
		float lerp_percent = static_cast<float>(interpolation_accumulator / (INTERPOLATION_RATE * (to_state.state_id - this->base_state.state_id)));
		// if this is zero, then we don't write to the interpolated state?
		// that seem weird
		if (lerp_percent > 0.0) {
			// in each of these sections we iterate through all the items in the new state
			// then we Lerp from the base state to the new state
			for (auto position : to_state.positions) {
				auto base_position_iter = this->base_state.positions.find(position.first);
				if (base_position_iter != this->base_state.positions.end()) {
					this->interpolated_state.positions[position.first].value =
						glm::lerp(base_position_iter->second.value, position.second.value, lerp_percent);
				}
				else {
					this->interpolated_state.positions[position.first] = position.second;
				}
			}
			for (auto velocity : to_state.velocities) {
				auto base_velocity_iter = this->base_state.velocities.find(velocity.first);
				if (base_velocity_iter != this->base_state.velocities.end()) {
					this->interpolated_state.velocities[velocity.first].linear =
						glm::lerp(base_velocity_iter->second.linear, velocity.second.linear, lerp_percent);
					this->interpolated_state.velocities[velocity.first].angular =
						glm::lerp(base_velocity_iter->second.angular, velocity.second.angular, lerp_percent);
				}
				else {
					this->interpolated_state.velocities[velocity.first].linear = velocity.second.linear;
					this->interpolated_state.velocities[velocity.first].angular = velocity.second.angular;
				}
			}
			// Quaternions, we use a slerp here instead
			for (auto orientation : to_state.orientations) {
				auto base_orientation_iter = this->base_state.orientations.find(orientation.first);
				if (base_orientation_iter != this->base_state.orientations.end()) {
					this->interpolated_state.orientations[orientation.first].value =
						glm::slerp(base_orientation_iter->second.value, orientation.second.value, lerp_percent);
				}
				else {
					this->interpolated_state.orientations[orientation.first] = orientation.second;
				}
			}
		}
	}
}

void ClientGameStateQueue::ProcessEventQueue() {
	EventQueue<EntityCreated>::ProcessEventQueue();
	EventQueue<EntityDestroyed>::ProcessEventQueue();
	EventQueue<NewGameStateEvent>::ProcessEventQueue();
}

void ClientGameStateQueue::On(std::shared_ptr<NewGameStateEvent> data) {
	QueueServerState(std::move(data->new_state));
}

void ClientGameStateQueue::QueueServerState(GameState&& new_state) {
	if (new_state.state_id > this->last_server_state_id) {
		this->server_states_array[server_state_array_index % SERVER_STATES_ARRAY_SIZE] = new_state;
		server_state_array_index++;
		this->last_server_state_id = new_state.state_id;
		std::lock_guard<std::mutex> lg(this->server_state_mutex);
		CheckPredictionResult(new_state);
		this->server_states.emplace(std::move(new_state));
	}
}

void ClientGameStateQueue::CheckPredictionResult(GameState& new_state) {
	if (this->client_id != 0) {
		//std::cout << "Client command id: " << this->command_id << " server ack: " << new_state.command_id << std::endl;
		this->stats.current_command_id = this->command_id;
		this->stats.current_acked_id = new_state.command_id;
		for (auto itr = this->predictions.begin(); itr != this->predictions.end(); ) {
			if ((itr->first + 1) < new_state.command_id) {
				itr = this->predictions.erase(itr);
			}
			else {
				++itr;
			}
		}
		if (this->predictions.find(new_state.command_id) != this->predictions.end()) {
			if (new_state.positions.find(this->client_id) != new_state.positions.end()) {
				stats.client_entity_position = new_state.positions[this->client_id].value;
				auto dif = new_state.positions[this->client_id].value - this->predictions[new_state.command_id].value;
				//std::cout << "Diff (" << dif.x << ", " << dif.y << ", " << dif.z << ") <> Predictions size = " << this->predictions.size() << std::endl;
			}
			this->predictions.erase(new_state.command_id);
		}
	}
}

void ClientGameStateQueue::On(std::shared_ptr<EntityCreated> data) {
	const proto::Entity& entity = data->entity;
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
			break;
		}
		case proto::Component::kOrientation:
		{
			Orientation orientation;
			orientation.In(comp);
			this->interpolated_state.orientations[entity_id] = orientation;
			this->base_state.orientations[entity_id] = orientation;
			break;
		}
		case proto::Component::kVelocity:
		{
			Velocity vel;
			vel.In(comp);
			this->interpolated_state.velocities[entity_id] = vel;
			this->base_state.velocities[entity_id] = vel;
			break;
		}
		default:
			break;
		}
	}
}

void ClientGameStateQueue::On(std::shared_ptr<EntityDestroyed> data) {
	const eid entity_id = data->entity_id;
	this->interpolated_state.positions.erase(entity_id);
	this->base_state.positions.erase(entity_id);
	this->interpolated_state.orientations.erase(entity_id);
	this->base_state.orientations.erase(entity_id);
	this->interpolated_state.velocities.erase(entity_id);
	this->base_state.velocities.erase(entity_id);
}

} // end namespace tec
