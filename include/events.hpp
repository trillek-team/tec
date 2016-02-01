#pragma once

#include <glm/glm.hpp>
#include "proto/components.pb.h"
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
		proto::Entity entity;
	};
	
	struct EntityDestroyed {
		eid entity_id;
	};
}
