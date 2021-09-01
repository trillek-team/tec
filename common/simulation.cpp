#include "simulation.hpp"

#include <asio/post.hpp>
#include <future>
#include <iostream>
#include <set>
#include <thread>

#include <commands.pb.h>

#include "components/transforms.hpp"
#include "controllers/fps-controller.hpp"

namespace tec {

double UPDATE_RATE = 1.0 / 8.0; // 8 per second
double TICKS_PER_SECOND = 60.0 * UPDATE_RATE;

Simulation::Simulation() : worker_pool(1) {}

Simulation::~Simulation() {
	worker_pool.stop();
	worker_pool.join();
}

GameState Simulation::Simulate(const double delta_time, GameState& interpolated_state) {
	ProcessCommandQueue();
	EventQueue<KeyboardEvent>::ProcessEventQueue();
	EventQueue<MouseBtnEvent>::ProcessEventQueue();
	EventQueue<MouseMoveEvent>::ProcessEventQueue();
	EventQueue<MouseClickEvent>::ProcessEventQueue();
	EventQueue<ClientCommandsEvent>::ProcessEventQueue();
	EventQueue<ControllerAddedEvent>::ProcessEventQueue();
	EventQueue<ControllerRemovedEvent>::ProcessEventQueue();
	EventQueue<FocusCapturedEvent>::ProcessEventQueue();
	EventQueue<FocusBlurEvent>::ProcessEventQueue();

	std::promise<void> vcomp_promise;
	auto vcomp_future = vcomp_promise.get_future();
	asio::post(worker_pool, [&]() {
		vcomp_sys.Update(delta_time);
		vcomp_promise.set_value();
	});

	for (Controller* controller : this->controllers) {
		controller->Update(delta_time, interpolated_state, this->event_list);
	}

	this->event_list.mouse_button_events.clear();
	this->event_list.mouse_move_events.clear();
	this->event_list.keyboard_events.clear();
	this->event_list.mouse_click_events.clear();

	GameState client_state = interpolated_state;
	std::set<eid> phys_results = phys_sys.Update(delta_time, interpolated_state);

	if (phys_results.size() > 0) {
		for (eid entity_id : phys_results) {
			if (interpolated_state.positions.find(entity_id) != interpolated_state.positions.end()) {
				client_state.positions[entity_id] = this->phys_sys.GetPosition(entity_id);
			}
			if (interpolated_state.orientations.find(entity_id) != interpolated_state.orientations.end()) {
				client_state.orientations[entity_id] = this->phys_sys.GetOrientation(entity_id);
			}
			if (interpolated_state.velocities.find(entity_id) != interpolated_state.velocities.end()) {
				client_state.velocities[entity_id] = interpolated_state.velocities.at(entity_id);
			}
		}
	}
	vcomp_future.wait();

	return client_state;
}

void Simulation::AddController(Controller* controller) { this->controllers.push_back(controller); }

void Simulation::RemoveController(Controller* controller) { this->controllers.remove(controller); }

void Simulation::On(eid, std::shared_ptr<KeyboardEvent> data) {
	this->event_list.keyboard_events.push_back(*data.get());
}

void Simulation::On(eid, std::shared_ptr<MouseBtnEvent> data) {
	this->event_list.mouse_button_events.push_back(*data.get());
}

void Simulation::On(eid, std::shared_ptr<MouseMoveEvent> data) {
	this->event_list.mouse_move_events.push_back(*data.get());
}

void Simulation::On(eid, std::shared_ptr<MouseClickEvent> data) {
	this->event_list.mouse_click_events.push_back(*data.get());
}

void Simulation::On(eid, std::shared_ptr<ControllerAddedEvent> data) { AddController(data->controller.get()); }

void Simulation::On(eid, std::shared_ptr<ControllerRemovedEvent> data) { RemoveController(data->controller.get()); }

/// \brief This event is sent to indicate that focus had been captured by an entity
/// it specifies which was captured: keyboard or mouse, if either of these parameters is true,
/// then prevent normal processing of those events
/// a controller implementing this should logically AND NOT these inputs with it's current settings
void Simulation::On(eid, std::shared_ptr<FocusCapturedEvent> data) {
	for (Controller* controller : this->controllers) {
		controller->ClearFocus(data->keyboard, data->mouse);
	}
}

/// \brief This event is sent to indicate that focus had been released (blur) from an owning entity
/// it specifies which was blurred: keyboard or mouse, if either of these parameters is true,
/// then restore normal processing of those events
/// a controller implementing this should logically OR these inputs with it's current settings
void Simulation::On(eid, std::shared_ptr<FocusBlurEvent> data) {
	for (Controller* controller : this->controllers) {
		controller->SetFocus(data->keyboard, data->mouse);
	}
}

void Simulation::On(eid, std::shared_ptr<ClientCommandsEvent> data) {
	for (Controller* controller : this->controllers) {
		if (static_cast<unsigned int>(controller->entity_id) == data->client_commands.id()) {
			controller->ApplyClientCommands(data->client_commands);
		}
	}
}

} // end namespace tec
