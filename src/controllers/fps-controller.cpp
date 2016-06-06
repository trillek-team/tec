// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "controllers/fps-controller.hpp"

#include <glm/gtx/quaternion.hpp>
#include "os.hpp"
#include "components/transforms.hpp"
#include "events.hpp"

namespace tec {
	void FPSController::Update(double delta, GameState& state, const CommandList& commands) {
		this->current_delta = delta;
		for (const KeyboardEvent& key_event : commands.keyboard_events) {
			Handle(key_event, state);
		}
		for (const MouseBtnEvent& mouse_button_event : commands.mouse_button_events) {
			Handle(mouse_button_event, state);
		}
		for (const MouseMoveEvent& mouse_move_event : commands.mouse_move_events) {
			Handle(mouse_move_event, state);
		}
		
		int forward = 0;
		int strafe = 0;

		if (this->KEY_W_DOWN && this->KEY_W_FIRST) {
			forward = -1;
		}
		else if (this->KEY_S_DOWN) {
			forward = 1;
		}
		if (this->KEY_A_DOWN && this->KEY_A_FIRST) {
			strafe = -1;
		}
		else if (this->KEY_D_DOWN) {
			strafe = 1;
		}

		glm::quat orientation;
		if (state.orientations.find(entity_id) != state.orientations.end()) {
			orientation = state.orientations.at(entity_id).value;
		}

		state.velocities[entity_id].linear = glm::vec4(orientation * glm::vec3(7.0 * strafe, 0.0, 7.0 * forward), 1.0);
	}

	void FPSController::Handle(const KeyboardEvent& data, GameState& state) {
		switch (data.action) {
			case KeyboardEvent::KEY_DOWN:
			case KeyboardEvent::KEY_REPEAT:
				switch (data.key) {
					case GLFW_KEY_A:
						if (!this->KEY_D_DOWN) {
							this->KEY_A_FIRST = true;
						}
						this->KEY_A_DOWN = true;
						break;
					case GLFW_KEY_D:
						this->KEY_D_DOWN = true;
						break;
					case GLFW_KEY_W:
						if (!this->KEY_S_DOWN) {
							this->KEY_W_FIRST = true;
						}
						this->KEY_W_DOWN = true;
						break;
					case GLFW_KEY_S:
						this->KEY_S_DOWN = true;
						break;
				}
				break;
			case KeyboardEvent::KEY_UP:
				switch (data.key) {
					case GLFW_KEY_A:
						this->KEY_A_DOWN = false;
						this->KEY_A_FIRST = false;
						break;
					case GLFW_KEY_D:
						this->KEY_D_DOWN = false;
						break;
					case GLFW_KEY_W:
						this->KEY_W_DOWN = false;
						this->KEY_W_FIRST = false;
						break;
					case GLFW_KEY_S:
						this->KEY_S_DOWN = false;
						break;
				}
				break;
			default:
				break;
		}
	}

	void FPSController::Handle(const MouseBtnEvent& data, GameState& state) {
		static double old_mouse_x;
		static double old_mouse_Y;

		if ((data.action == MouseBtnEvent::DOWN) && (data.button == MouseBtnEvent::RIGHT)) {
			this->mouse_look = true;
		}
		else if ((data.action == MouseBtnEvent::UP) && (data.button == MouseBtnEvent::RIGHT)) {
			this->mouse_look = false;
		}
	}

	void FPSController::Handle(const MouseMoveEvent& data, GameState& state) {
		if (!this->mouse_look) {
			return;
		}
		glm::quat orientation;
		if (state.orientations.find(entity_id) != state.orientations.end()) {
			orientation = state.orientations.at(entity_id).value;
		}

		float change_x = static_cast<float>(data.new_x - data.old_x);
		float change_y = static_cast<float>(data.new_y - data.old_y);

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
			glm::quat rotX = glm::angleAxis(static_cast<float>(glm::radians(change_x * -50.0f *  this->current_delta)),
				glm::vec3(0.0, 1.0, 0.0));
			orientation = rotX * orientation;
		}
		if (change_y != 0) {
			glm::quat rotY = glm::angleAxis(static_cast<float>(glm::radians(change_y * -10.0f  * this->current_delta)),
				glm::vec3(1.0, 0.0, 0.0));
			orientation = orientation * rotY;
		}

		if (state.orientations.find(entity_id) != state.orientations.end()) {
			state.orientations[entity_id].value = orientation;
		}
	}
}
