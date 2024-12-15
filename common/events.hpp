#pragma once

#include <commands.pb.h>
#include <components.pb.h>
#include <glm/glm.hpp>
#include <vector>

#include "tec-types.hpp"

namespace tec {
struct MouseHover {
	glm::vec3 ray_hit_point_world;
};

struct KeyboardEvent {
	enum KEY_ACTION { KEY_DOWN, KEY_UP, KEY_REPEAT, KEY_CHAR };
	int key{0};
	int scancode{0};
	KEY_ACTION action;
	int mods{0};

	bool operator==(const KeyboardEvent& other) const {
		return key == other.key && scancode == other.scancode && action == other.action && mods == other.mods;
	}
};

struct MouseBtnEvent {
	enum MOUSE_BTN_ACTION { DOWN, UP };
	enum MOUSE_BTN { LEFT, RIGHT, MIDDLE };
	MOUSE_BTN_ACTION action;
	MOUSE_BTN button;

	bool operator==(const MouseBtnEvent& other) const { return action == other.action && button == other.button; }
};

/** Mouse change of position event */
struct MouseMoveEvent {
	double norm_x{0.0}, norm_y{0.0}; /// Resolution independent new x, y (0-1) from upper-left to lower-right.
	int old_x{0}, old_y{0}; /// Client space old x, y.
	int new_x{0}, new_y{0}; /// Client space new x, y.

	bool operator==(const MouseMoveEvent& other) const {
		return norm_x == other.norm_x && norm_y == other.norm_y && old_x == other.old_x && old_y == other.old_y
			   && new_x == other.new_x && new_y == other.new_y;
	}
};

/** Mouse wheel event */
struct MouseScrollEvent {
	double x_offset{0.0}, y_offset{0.0}; /// Delta x, y of mouse wheel.
};

struct MouseClickEvent {
	MouseBtnEvent::MOUSE_BTN button;
	glm::vec3 ray_hit_point_world{0.f, 0.f, 0.f};
	double ray_distance;

	bool operator==(const MouseClickEvent& other) const {
		return button == other.button && ray_hit_point_world == other.ray_hit_point_world
			   && ray_distance == other.ray_distance;
	}
};

struct WindowResizedEvent {
	int old_width{0}, old_height{0}; // Client space old width, height.
	int new_width{0}, new_height{0}; // Client space new width, height.
};

struct FileDropEvent {
	std::vector<std::string> filenames;
};

struct EntityCreated {
	proto::Entity entity;
};

struct EntityDestroyed {};

struct ClientCommandsEvent {
	proto::ClientCommands client_commands;
};

struct Controller;
struct ControllerAddedEvent {
	std::shared_ptr<Controller> controller{nullptr};
};
struct ControllerRemovedEvent {
	std::shared_ptr<Controller> controller{nullptr};
};
struct FocusCapturedEvent {
	bool keyboard;
	bool mouse;
};
struct FocusBlurEvent {
	bool keyboard;
	bool mouse;
};
struct ChatCommandEvent {
	std::string command;
	std::vector<std::string> args;
	ChatCommandEvent() = default;
	ChatCommandEvent(proto::ChatCommand chat_command) { this->In(chat_command); }
	void In(proto::ChatCommand chat_command) {
		this->command = chat_command.command();
		auto arguments = chat_command.arguments();
		this->args.reserve(arguments.size());
		for (auto argument : arguments) {
			this->args.push_back(argument);
		}
	}
	void Out(proto::ChatCommand& chat_command) {
		chat_command.set_command(this->command);
		auto arguments = chat_command.mutable_arguments();
		for (const std::string& argument : this->args) {
			arguments->Add(std::string(argument));
		}
	}
	proto::ChatCommand Out() {
		proto::ChatCommand chat_command;
		this->Out(chat_command);
		return chat_command;
	}
};

struct UserLoginEvent {
	std::string username;
	std::string password;
	UserLoginEvent() = default;
	UserLoginEvent(proto::UserLogin user_login) { this->In(user_login); }
	void In(proto::UserLogin user_login) {
		this->username = user_login.username();
		this->password = user_login.password();
	}
	void Out(proto::UserLogin& user_login) {
		user_login.set_username(this->username);
		user_login.set_password(this->password);
	}
	proto::UserLogin Out() {
		proto::UserLogin user_login;
		this->Out(user_login);
		return user_login;
	}
};
} // namespace tec
