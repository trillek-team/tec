#pragma once

#include <memory>
#include "entity.hpp"
#include "event-system.hpp"

namespace vv {
	class Camera {
	public:
		Camera(eid entity_id);
		~Camera();

		bool MakeActive();
	private:
		Entity e;
	};

	struct KeyboardEvent;

	// TODO: Create Controller system that calls update on all controller instances.
	struct Controller {
		virtual void Update(double delta) { }
	};
	// TODO: Remove this class as it is only for testing and should really be implemented in script.
	struct CameraMover : public Controller, public EventQueue < KeyboardEvent > {
		CameraMover(eid entity_id) : e(entity_id) { }

		void On(std::shared_ptr<KeyboardEvent> data);

		void Update(double delta);

		std::weak_ptr<Camera> cam;

		Entity e;
	};
}
