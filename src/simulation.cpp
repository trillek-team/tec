#include "simulation.hpp"

#include <thread>
#include <future>
#include <set>

#include "components/transforms.hpp"
#include "physics-system.hpp"
#include "controllers/fps-controller.hpp"

#include "components/collisionbody.hpp"
#include <glm/gtx/compatibility.hpp>

namespace tec {
	void Simulation::PopulateBaseState() {
		for (auto itr = Multiton<eid, std::shared_ptr<Position>>::Begin(); itr != Multiton<eid, std::shared_ptr<Position>>::End(); ++itr) {
			this->interpolation_state.positions[itr->first] = *itr->second.get();
		}
		for (auto itr = Multiton<eid, std::shared_ptr<Orientation>>::Begin(); itr != Multiton<eid, std::shared_ptr<Orientation>>::End(); ++itr) {
			this->interpolation_state.orientations[itr->first] = *itr->second.get();
		}
	}

	void Simulation::Simulate(const double delta_time) {
		EventQueue<KeyboardEvent>::ProcessEventQueue();
		EventQueue<MouseBtnEvent>::ProcessEventQueue();
		EventQueue<MouseMoveEvent>::ProcessEventQueue();
		EventQueue<MouseClickEvent>::ProcessEventQueue();

		/*auto vcomp_future = std::async(std::launch::async, [&] () {
			vcomp_sys.Update(delta_time);
			});*/

		for (Controller* controller : this->controllers) {
			controller->Update(delta_time, this->interpolation_state, this->current_command_list);
		}
		this->current_command_list.mouse_button_events.clear();
		this->current_command_list.mouse_move_events.clear();
		this->current_command_list.keyboard_events.clear();
		this->current_command_list.mouse_click_events.clear();
		std::future<std::set<eid>> phys_future = std::async(std::launch::async, [=] () -> std::set < eid > {
			return std::move(phys_sys.Update(delta_time, this->interpolation_state));
		});

		std::set<eid> current_results = phys_future.get();

		if (current_results.size() > 0) {
			for (eid entity_id : current_results) {
				client_state.positions[entity_id] = this->phys_sys.GetPosition(entity_id);
				client_state.orientations[entity_id] = this->phys_sys.GetOrientation(entity_id);
				if (this->interpolation_state.velocties.find(entity_id) != this->interpolation_state.velocties.end()) {
					client_state.velocties[entity_id] = this->interpolation_state.velocties[entity_id];
				}
			}
		}
		//vcomp_future.get();
	}

	void Simulation::Interpolate(const double delta_time) {
			// If we don't have a server update we will just use the last result of the local simulation.
			for (const auto& position : this->client_state.positions) {
				this->interpolation_state.positions[position.first] = position.second;
			}
			for (const auto& orientation : this->client_state.orientations) {
				this->interpolation_state.orientations[orientation.first] = orientation.second;
			}
			for (const auto& velocity : this->client_state.velocties) {
				this->interpolation_state.velocties[velocity.first] = velocity.second;
			}
	}

	void Simulation::AddController(Controller* controller) {
		this->controllers.push_back(controller);
	}


	const GameState& Simulation::GetClientState() {
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
