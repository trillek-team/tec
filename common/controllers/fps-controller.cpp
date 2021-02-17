#include "controllers/fps-controller.hpp"

#include <iostream>

#include <glm/gtx/quaternion.hpp>

#include "components/transforms.hpp"
#include "events.hpp"

namespace tec {
void FPSController::Update(double delta, GameState& state, EventList& commands) {
	if (!this->orientation && state.orientations.find(entity_id) != state.orientations.end()) {
		this->orientation = std::make_unique<Orientation>(state.orientations[entity_id]);
	}
	if (!this->orientation) {
		return;
	}
	this->current_delta = delta;
	if (keyboard_focus) {
		for (const KeyboardEvent& key_event : commands.keyboard_events) {
			Handle(key_event, state);
		}
	}
	for (const MouseBtnEvent& mouse_button_event : commands.mouse_button_events) {
		Handle(mouse_button_event, state);
	}
	if (mouse_focus) {
		for (const MouseMoveEvent& mouse_move_event : commands.mouse_move_events) {
			Handle(mouse_move_event, state);
		}
	}

	int forwardDirection = 0;
	int strafeDirection = 0;

	if ((this->KEY_W_DOWN && this->KEY_W_FIRST) || this->forward) {
		forwardDirection = -1;
		this->forward = true;
	}
	else if (this->KEY_S_DOWN || this->backward) {
		forwardDirection = 1;
		this->backward = true;
	}
	if ((this->KEY_A_DOWN && this->KEY_A_FIRST) || this->left_strafe) {
		strafeDirection = -1;
		this->left_strafe = true;
	}
	else if (this->KEY_D_DOWN || this->right_strafe) {
		strafeDirection = 1;
		this->right_strafe = true;
	}

	if (state.orientations.find(entity_id) != state.orientations.end()) {
		state.orientations[entity_id] = *this->orientation;
	}

	state.velocities[entity_id].linear =
		glm::vec4(this->orientation->value * glm::vec3(5.0 * strafeDirection, 0.0, 7.5 * forwardDirection), 1.0);
}

proto::ClientCommands FPSController::GetClientCommands() {
	proto::ClientCommands proto_client_commands;
	proto_client_commands.set_id(this->entity_id);
	proto::MovementCommand* movement_command = proto_client_commands.mutable_movement();
	movement_command->set_forward(false);
	movement_command->set_backward(false);
	movement_command->set_leftstrafe(false);
	movement_command->set_rightstrafe(false);
	if (this->forward) {
		movement_command->set_forward(true);
	}
	else if (this->backward) {
		movement_command->set_backward(true);
	}
	if (this->left_strafe) {
		movement_command->set_leftstrafe(true);
	}
	else if (this->right_strafe) {
		movement_command->set_rightstrafe(true);
	}

	if (this->orientation) {
		auto _orientation = proto_client_commands.mutable_orientation();
		_orientation->set_x(this->orientation->value.x);
		_orientation->set_y(this->orientation->value.y);
		_orientation->set_z(this->orientation->value.z);
		_orientation->set_w(this->orientation->value.w);
	}
	this->forward = false;
	this->backward = false;
	this->left_strafe = false;
	this->right_strafe = false;
	return proto_client_commands;
}

void FPSController::ApplyClientCommands(proto::ClientCommands proto_client_commands) {
	if (proto_client_commands.has_movement()) {
		const proto::MovementCommand& movement_commands = proto_client_commands.movement();
		this->forward = movement_commands.forward();
		this->backward = movement_commands.backward();
		this->left_strafe = movement_commands.leftstrafe();
		this->right_strafe = movement_commands.rightstrafe();
	}
	else {
		this->forward = false;
		this->backward = false;
		this->left_strafe = false;
		this->right_strafe = false;
	}
	if (this->orientation && proto_client_commands.has_orientation()) {
		const proto::OrientationCommand& orientation_command = proto_client_commands.orientation();
		this->orientation->value.x = orientation_command.x();
		this->orientation->value.y = orientation_command.y();
		this->orientation->value.z = orientation_command.z();
		this->orientation->value.w = orientation_command.w();
	}
}

void FPSController::Handle(const KeyboardEvent& data, const GameState&) {
	switch (data.action) {
	case KeyboardEvent::KEY_DOWN:
	case KeyboardEvent::KEY_REPEAT:
		switch (data.key) {
		case 'A':
			if (!this->KEY_D_DOWN) {
				this->KEY_A_FIRST = true;
			}
			this->KEY_A_DOWN = true;
			break;
		case 'D': this->KEY_D_DOWN = true; break;
		case 'W':
			if (!this->KEY_S_DOWN) {
				this->KEY_W_FIRST = true;
			}
			this->KEY_W_DOWN = true;
			break;
		case 'S': this->KEY_S_DOWN = true; break;
		}
		break;
	case KeyboardEvent::KEY_UP:
		switch (data.key) {
		case 'A':
			this->KEY_A_DOWN = false;
			this->KEY_A_FIRST = false;
			break;
		case 'D': this->KEY_D_DOWN = false; break;
		case 'W':
			this->KEY_W_DOWN = false;
			this->KEY_W_FIRST = false;
			break;
		case 'S': this->KEY_S_DOWN = false; break;
		default: break;
		}
		break;
	}
}

void FPSController::Handle(const MouseBtnEvent& data, const GameState&) {
	if ((data.action == MouseBtnEvent::DOWN) && (data.button == MouseBtnEvent::RIGHT)) {
		this->mouse_look = !this->mouse_look;
	}
}

void FPSController::Handle(const MouseMoveEvent& data, const GameState&) {
	if (!this->mouse_look) {
		return;
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
		glm::quat rotX = glm::angleAxis(
			static_cast<float>(glm::radians(change_x * -8.0f * this->current_delta)), glm::vec3(0.0, 1.0, 0.0));
		this->orientation->value = rotX * this->orientation->value;
	}
	if (change_y != 0) {
		glm::quat rotY = glm::angleAxis(
			static_cast<float>(glm::radians(change_y * -8.0f * this->current_delta)), glm::vec3(1.0, 0.0, 0.0));
		this->orientation->value = this->orientation->value * rotY;
	}
}
} // namespace tec
