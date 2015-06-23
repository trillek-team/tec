#include "controllers/fps-controller.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "os.hpp"
#include "render-system.hpp"
#include "components/transforms.hpp"
#include "physics-system.hpp"

namespace tec {
	void FPSController::Update(double delta) {
		ProcessEventQueue();
	}

	// These tell us which was pressed first.
	bool KEY_A_FIRST = false;
	bool KEY_W_FIRST = false;

	// These tell us which are pressed.
	bool KEY_W_DOWN = false;
	bool KEY_A_DOWN = false;
	bool KEY_S_DOWN = false;
	bool KEY_D_DOWN = false;

	void FPSController::On(std::shared_ptr<KeyboardEvent> data) {
		auto orientation = e.Get<Orientation>().lock();

		auto old_velocity = this->e.Get<Velocity>().lock();
		std::shared_ptr<Velocity> new_velocity;
		if (old_velocity) {
			new_velocity = std::make_shared<Velocity>(old_velocity->linear, old_velocity->angular);
		}

		switch (data->action) {
			case KeyboardEvent::KEY_DOWN:
			case KeyboardEvent::KEY_REPEAT:
				switch (data->key) {
					case GLFW_KEY_A:
						if (!KEY_D_DOWN) {
							KEY_A_FIRST = true;
						}
						KEY_A_DOWN = true;
						break;
					case GLFW_KEY_D:
						KEY_D_DOWN = true;
						break;
					case GLFW_KEY_W:
						if (!KEY_S_DOWN) {
							KEY_W_FIRST = true;
						}
						KEY_W_DOWN = true;
						break;
					case GLFW_KEY_S:
						KEY_S_DOWN = true;
						break;
				}
				break;
			case KeyboardEvent::KEY_UP:
				switch (data->key) {
					case GLFW_KEY_A:
						KEY_A_DOWN = false;
						KEY_A_FIRST = false;
						break;
					case GLFW_KEY_D:
						KEY_D_DOWN = false;
						break;
					case GLFW_KEY_W:
						KEY_W_DOWN = false;
						KEY_W_FIRST = false;
						break;
					case GLFW_KEY_S:
						KEY_S_DOWN = false;
						break;
				}
				break;
			default:
				break;
		}

		if (KEY_W_DOWN && KEY_W_FIRST) {
			new_velocity->linear = glm::vec4(orientation->value * glm::vec3(0.0, 0.0, -7.0), 1.0);
		}
		else if (KEY_S_DOWN) {
			new_velocity->linear = glm::vec4(orientation->value * glm::vec3(0.0, 0.0, 7.0), 1.0);
		}
		else {
			new_velocity->linear = glm::vec4(0.0, 0.0, 0.0, 0.0);
		}
		if (KEY_A_DOWN && KEY_A_FIRST) {
			new_velocity->angular = glm::vec4(0.0, glm::radians(30.0f), 0.0, 0.0);
		}
		else if (KEY_D_DOWN) {
			new_velocity->angular = glm::vec4(0.0, glm::radians(-30.0f), 0.0, 0.0);
		}
		else {
			new_velocity->angular = glm::vec4(0.0, 0.0, 0.0, 0.0);
		}
		this->e.Update<Velocity>(new_velocity);
	}
}
