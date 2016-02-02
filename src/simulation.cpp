#include "simulation.hpp"

#include <thread>
#include <future>
#include <set>

#include "components/transforms.hpp"
#include "components/collisionbody.hpp"
#include "physics-system.hpp"
#include "component-update-system.hpp"
#include "controllers/fps-controller.hpp"
#include <glm/gtx/compatibility.hpp>

namespace tec {
	std::set<eid> Simulation::Simulate(const double delta_time) {
		ProcessCommandQueue();
		EventQueue<KeyboardEvent>::ProcessEventQueue();
		EventQueue<MouseBtnEvent>::ProcessEventQueue();
		EventQueue<MouseMoveEvent>::ProcessEventQueue();
		EventQueue<MouseClickEvent>::ProcessEventQueue();
		EventQueue<EntityCreated>::ProcessEventQueue();
		EventQueue<EntityUpdated>::ProcessEventQueue();
		EventQueue<EntityDestroyed>::ProcessEventQueue();

		/*auto vcomp_future = std::async(std::launch::async, [&] () {
			vcomp_sys.Update(delta_time);
			});*/

		for (Controller* controller : this->controllers) {
			controller->Update(delta_time, this->client_state, this->current_command_list);
		}
		this->current_command_list.mouse_button_events.clear();
		this->current_command_list.mouse_move_events.clear();
		this->current_command_list.keyboard_events.clear();
		this->current_command_list.mouse_click_events.clear();
		std::future<std::set<eid>> phys_future = std::async(std::launch::async, [=] () -> std::set < eid > {
			return std::move(phys_sys.Update(delta_time, this->client_state));
		});

		std::set<eid> current_results = phys_future.get();

		if (current_results.size() > 0) {
			for (eid entity_id : current_results) {
				client_state.positions[entity_id] = this->phys_sys.GetPosition(entity_id);
				client_state.orientations[entity_id] = this->phys_sys.GetOrientation(entity_id);
				if (this->client_state.velocties.find(entity_id) != this->client_state.velocties.end()) {
					client_state.velocties[entity_id] = this->client_state.velocties[entity_id];
				}
				// Update reflection
				for (auto pos : client_state.positions) {
					ComponentUpdateSystem<Position>::SubmitUpdate(pos.first, std::make_shared<Position>(pos.second), 0);
					if (client_state.orientations.find(pos.first) != client_state.orientations.end()) {
						tec::Orientation ori = client_state.orientations.at(pos.first);
						ComponentUpdateSystem<Orientation>::SubmitUpdate(pos.first, std::make_shared<Orientation>(ori), 0);
					}
					if (client_state.velocties.find(pos.first) != client_state.velocties.end()) {
						tec::Velocity vel = client_state.velocties.at(pos.first);
						ComponentUpdateSystem<Velocity>::SubmitUpdate(pos.first, std::make_shared<Velocity>(vel), 0);
					}
				}
			}
		}
		//vcomp_future.get();

		return std::move(current_results);
	}


	void Simulation::Interpolate(const double delta_time) {
		static const double INTERPOLATION_RATE = 1.0 / 100.0; // TODO: Make this configurable via a run-time property.

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
				for (auto velocity : to_state.velocties) {
					this->base_state.velocties[velocity.first] = velocity.second;
				}
				for (auto orientation : to_state.orientations) {
					this->base_state.orientations[orientation.first] = orientation.second;
				}
				interpolation_accumulator -= INTERPOLATION_RATE;
				this->base_state.state_id = to_state.state_id;
				std::lock_guard<std::mutex> lock(this->server_state_mutex);
				this->server_states.pop();
			}
			const GameState& to_state = this->server_states.front();
			float lerp_percent = static_cast<float>(interpolation_accumulator / (INTERPOLATION_RATE * (to_state.state_id - this->base_state.state_id)));
			if (lerp_percent > 0.0) {
				for (auto position : to_state.positions) {
					if (this->base_state.positions.find(position.first) != this->base_state.positions.end()) {
						this->client_state.positions[position.first].value = glm::lerp(
							base_state.positions.at(position.first).value, position.second.value, lerp_percent);
					}
					else {
						this->client_state.positions[position.first] = position.second;
					}
				}
				for (auto velocity : to_state.velocties) {
					if (this->base_state.velocties.find(velocity.first) != this->base_state.velocties.end()) {
						this->client_state.velocties[velocity.first].linear = glm::lerp(
							base_state.velocties.at(velocity.first).linear, velocity.second.linear, lerp_percent);
						this->client_state.velocties[velocity.first].angular = glm::lerp(
							base_state.velocties.at(velocity.first).angular, velocity.second.angular, lerp_percent);
					}
					else {
						this->client_state.velocties[velocity.first].linear = velocity.second.linear;
						this->client_state.velocties[velocity.first].angular = velocity.second.angular;
					}
				}
				for (auto orientation : to_state.orientations) {
					if (this->base_state.orientations.find(orientation.first) != this->base_state.orientations.end()) {
						this->client_state.orientations[orientation.first].value = glm::slerp(
							base_state.orientations.at(orientation.first).value, orientation.second.value, lerp_percent);
					}
					else {
						this->client_state.orientations[orientation.first] = orientation.second;
					}
				}
			}
		}
	}

	void Simulation::AddController(Controller* controller) {
		this->controllers.push_back(controller);
	}

	void Simulation::onServerStateUpdate(GameState&& new_state) {
		if (new_state.state_id > this->last_server_state_id) {
			this->last_server_state_id = new_state.state_id;
			std::lock_guard<std::mutex> lock(this->server_state_mutex);
			this->server_states.emplace(std::move(new_state));
		}
	}

	void Simulation::SetEntityState(proto::Entity& entity) {
		QueueCommand(std::move([=] (Simulation* sim) {
			sim->onSetEntityState(entity);
		}));
	}

	void Simulation::On(std::shared_ptr<EntityCreated> data) {
		onSetEntityState(data->entity);
	}

	void Simulation::On(std::shared_ptr<EntityUpdated> data) {
		const proto::Entity& entity = data->entity;
		eid entity_id = entity.id();
		for (int i = 0; i < entity.components_size(); ++i) {
			const proto::Component& comp = entity.components(i);
			switch (comp.component_case()) {
				case proto::Component::kPosition:
					{
						Position pos;
						pos.In(comp);
						this->client_state.positions[entity_id] = pos;
						this->base_state.positions[entity_id] = pos;
					}
					break;
				case proto::Component::kOrientation:
					{
						Orientation orientation;
						orientation.In(comp);
						this->client_state.orientations[entity_id] = orientation;
						this->base_state.orientations[entity_id] = orientation;
					}
					break;
				case proto::Component::kVelocity:
					{
						Velocity vel;
						vel.In(comp);
						this->client_state.velocties[entity_id] = vel;
						this->base_state.velocties[entity_id] = vel;
					}
					break;
			}
		}
	}

	void Simulation::On(std::shared_ptr<EntityDestroyed> data) {
		if (this->client_state.positions.find(data->entity_id) != this->client_state.positions.end()) {
			this->client_state.positions.erase(data->entity_id);
			this->base_state.positions.erase(data->entity_id);
		}
		if (this->client_state.orientations.find(data->entity_id) != this->client_state.orientations.end()) {
			this->client_state.orientations.erase(data->entity_id);
			this->base_state.orientations.erase(data->entity_id);
		}
		if (this->client_state.velocties.find(data->entity_id) != this->client_state.velocties.end()) {
			this->client_state.velocties.erase(data->entity_id);
			this->base_state.velocties.erase(data->entity_id);
		}
	}

	void Simulation::onSetEntityState(const proto::Entity& entity) {
		eid entity_id = entity.id();
		for (int i = 0; i < entity.components_size(); ++i) {
			const proto::Component& comp = entity.components(i);
			switch (comp.component_case()) {
				case proto::Component::kPosition:
					{
						Position pos;
						pos.In(comp);
						this->client_state.positions[entity_id] = pos;
						this->base_state.positions[entity_id] = pos;
					}
					break;
				case proto::Component::kOrientation:
					{
						Orientation orientation;
						orientation.In(comp);
						this->client_state.orientations[entity_id] = orientation;
						this->base_state.orientations[entity_id] = orientation;
					}
					break;
				case proto::Component::kVelocity:
					{
						Velocity vel;
						vel.In(comp);
						this->client_state.velocties[entity_id] = vel;
						this->base_state.velocties[entity_id] = vel;
					}
					break;
			}
			if (in_functors.find(comp.component_case()) != in_functors.end()) {
				in_functors[comp.component_case()](entity, comp);
			}
		}
	}

	void Simulation::onRemoveEntity(const eid entity_id) {

	}

	GameState& Simulation::GetClientState() {
		return this->client_state;
	}

	void Simulation::On(std::shared_ptr<KeyboardEvent> data) {
		this->current_command_list.keyboard_events.push_back(*data.get());
	}

	void Simulation::On(std::shared_ptr<MouseBtnEvent> data) {
		this->current_command_list.mouse_button_events.push_back(*data.get());
	}

	void Simulation::On(std::shared_ptr<MouseMoveEvent> data) {
		this->current_command_list.mouse_move_events.push_back(*data.get());
	}

	void Simulation::On(std::shared_ptr<MouseClickEvent> data) {
		this->current_command_list.mouse_click_events.push_back(*data.get());
	}
}
