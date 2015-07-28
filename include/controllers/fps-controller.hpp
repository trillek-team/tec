#pragma once

#include <memory>
#include "entity.hpp"
#include "event-system.hpp"

namespace tec {
	struct KeyboardEvent;
	struct MouseBtnEvent;
	struct MouseMoveEvent;

	// TODO: Create Controller system that calls update on all controller instances.
	struct Controller {
		virtual void Update(double delta) { }
	};
	// TODO: Remove this class as it is only for testing and should really be implemented in script.
	struct FPSController : public Controller, public EventQueue < KeyboardEvent >,
		public EventQueue < MouseMoveEvent >, public EventQueue < MouseBtnEvent > {
		FPSController(eid entity_id) : e(entity_id), mouse_look(false) { }

		void On(std::shared_ptr<KeyboardEvent> data);
		void On(std::shared_ptr<MouseBtnEvent> data);
		void On(std::shared_ptr<MouseMoveEvent> data);

		void Update(double delta);

		Entity e;
		double current_delta;
		bool mouse_look;
	};
}
