#include "controllers/fps-controller.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "os.hpp"
#include "components/transforms.hpp"
#include "physics-system.hpp"

namespace tec {
	void FPSController::Update(double delta) {
		this->current_delta = delta;
		EventQueue<KeyboardEvent>::ProcessEventQueue();
		EventQueue<MouseBtnEvent>::ProcessEventQueue();
		EventQueue<MouseMoveEvent>::ProcessEventQueue();
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
		int forward = 0;
		int strafe = 0;

		if (KEY_W_DOWN && KEY_W_FIRST) {
			forward = -1;
		}
		else if (KEY_S_DOWN) {
			forward = 1;
		}
		if (KEY_A_DOWN && KEY_A_FIRST) {
			strafe = -1;
		}
		else if (KEY_D_DOWN) {
			strafe = 1;
		}
		new_velocity->linear = glm::vec4(orientation->value * glm::vec3(7.0 * strafe, 0.0, 7.0 * forward), 1.0);
		this->e.Update<Velocity>(new_velocity);
	}
	void FPSController::On(std::shared_ptr<MouseBtnEvent> data) {
		static double old_mouse_x;
		static double old_mouse_Y;

		if ((data->action == MouseBtnEvent::DOWN) && (data->button == MouseBtnEvent::RIGHT)) {
			this->mouse_look = true;
			//OS::GetMousePosition(&old_mouse_x, &old_mouse_Y);
			//OS::SetMousePosition(400, 300);
		}
		else if ((data->action == MouseBtnEvent::UP) && (data->button == MouseBtnEvent::RIGHT)) {
			this->mouse_look = false;
			//OS::SetMousePosition(old_mouse_x, old_mouse_Y);
		}

	}

	void FPSController::On(std::shared_ptr<MouseMoveEvent> data) {
		if (!this->mouse_look) {
			return;
		}
		auto old_orientation = e.Get<Orientation>().lock();
		std::shared_ptr<Orientation> new_orientation;
		if (old_orientation) {
			new_orientation = std::make_shared<Orientation>(old_orientation->value);
		}

		float change_x = data->new_x - data->old_x;
		float change_y = data->new_y - data->old_y;

		static float pitch = 0.0f;

		pitch += change_y / 5.0f;

		if (pitch > 90.0f) {
			change_y = 0.0;
			pitch = 90.0f;
		}
		if (pitch < -120.0f) {
			change_y = 0.0;
			pitch = -120.0f;
		}

		if (change_x != 0) {
			glm::quat rotX = glm::angleAxis(static_cast<float>(glm::radians(change_x * -20.0f *  this->current_delta)),
				glm::vec3(0.0, 1.0, 0.0));
			new_orientation->value = rotX * new_orientation->value;
		}
		if (change_y != 0) {
			glm::quat rotY = glm::angleAxis(static_cast<float>(glm::radians(change_y * -10.0f  * this->current_delta)),
				glm::vec3(1.0, 0.0, 0.0));
			new_orientation->value = new_orientation->value * rotY;
		}
		this->e.Update<Orientation>(new_orientation);
	}
}
