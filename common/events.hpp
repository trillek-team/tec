#pragma once

#include <commands.pb.h>
#include <components.pb.h>
#include <glm/glm.hpp>

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
};

struct MouseBtnEvent {
	enum MOUSE_BTN_ACTION { DOWN, UP };
	enum MOUSE_BTN { LEFT, RIGHT, MIDDLE };
	MOUSE_BTN_ACTION action;
	MOUSE_BTN button;
};

/** Mouse change of position event */
struct MouseMoveEvent {
	double norm_x{0.0}, norm_y{0.0}; /// Resolution independent new x, y (0-1) from upper-left to lower-right.
	int old_x{0}, old_y{0}; /// Client space old x, y.
	int new_x{0}, new_y{0}; /// Client space new x, y.
};

/** Mouse wheel event */
struct MouseScrollEvent {
	double x_offset{0.0}, y_offset{0.0}; /// Delta x, y of mouse wheel.
};

struct MouseClickEvent {
	eid entity_id{0};
	MouseBtnEvent::MOUSE_BTN button;
	glm::vec3 ray_hit_point_world{0.f, 0.f, 0.f};
	double ray_distance;
};

struct WindowResizedEvent {
	int old_width{0}, old_height{0}; // Client space old width, height.
	int new_width{0}, new_height{0}; // Client space new width, height.
};

struct FileDropEvent {
	std::vector<std::string> filenames;
};

struct EntityCreated {
	eid entity_id{0};
	proto::Entity entity;
};

struct EntityDestroyed {
	eid entity_id{0};
};

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
	eid entity_id;
	bool keyboard;
	bool mouse;
};
struct FocusBlurEvent {
	eid entity_id;
	bool keyboard;
	bool mouse;
};
} // namespace tec
