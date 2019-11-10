// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <glm/glm.hpp>
#include "components.pb.h"
#include "commands.pb.h"
#include "types.hpp"

namespace tec {
	struct MouseHover {
		glm::vec3 ray_hit_piont_world;
	};

	struct KeyboardEvent {
		enum KEY_ACTION { KEY_DOWN, KEY_UP, KEY_REPEAT, KEY_CHAR };
		int key;
		int scancode;
		KEY_ACTION action;
		int mods;
	};

	struct MouseBtnEvent {
		enum MOUSE_BTN_ACTION { DOWN, UP };
		enum MOUSE_BTN { LEFT, RIGHT, MIDDLE };
		MOUSE_BTN_ACTION action;
		MOUSE_BTN button;
	};

	/** Mouse change of position event */
	struct MouseMoveEvent {
		double norm_x, norm_y; /// Resolution independent new x, y (0-1) from upper-left to lower-right.
		int old_x, old_y; /// Client space old x, y.
		int new_x, new_y; /// Client space new x, y.
	};

	/** Mouse wheel event */
	struct MouseScrollEvent {
		double x_offset, y_offset; /// Delta x, y of mouse wheel.
	};

	struct MouseClickEvent {
		eid entity_id;
		MouseBtnEvent::MOUSE_BTN button;
		glm::vec3 ray_hit_piont_world;
		double ray_distance;
	};

	struct WindowResizedEvent {
		int old_width, old_height; // Client space old width, height.
		int new_width, new_height; // Client space new width, height.
	};

	struct FileDropEvent {
		std::vector<std::string> filenames;
	};

	struct EntityCreated {
		eid entity_id;
		proto::Entity entity;
	};

	struct EntityDestroyed {
		eid entity_id;
	};

	struct ClientCommandsEvent {
		proto::ClientCommands client_commands;
	};

	struct Controller;
	struct ControllerAddedEvent {
		Controller* controller;
	};
	struct ControllerRemovedEvent {
		Controller* controller;
	};

}
