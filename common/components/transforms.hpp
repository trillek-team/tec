// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "components.pb.h"

namespace tec {
	static glm::vec3 FORWARD_VECTOR(0.0f, 0.0f, -1.0f);
	static glm::vec3 UP_VECTOR(0.0f, 1.0f, 0.0f);
	static glm::vec3 RIGHT_VECTOR(1.0f, 0.0f, 0.0f);

	struct Position {
		Position(glm::vec3 pos) : value(pos) { }
		Position() { }
		// Translates by amount.
		void Translate(const glm::vec3 amount);

		// Translates by amount in direction orientation.
		void Translate(const glm::vec3 amount, const glm::quat orientation);

		glm::vec3 value;
		glm::vec3 center_offset;

		void Out(proto::Component* target);
		void In(const proto::Component& source);
	};

	struct Orientation {
		Orientation(glm::vec3 rot) {
			Rotate(rot);
		}
		Orientation(glm::quat q) : value(q), rotation(glm::eulerAngles(q)) { }
		Orientation() { }
		void Rotate(const glm::vec3 amount = glm::vec3(0.0));

		// Adds the rotation amount based on the current orientation.
		void OrientedRotate(const glm::vec3 amount);

		glm::quat value;
		glm::vec3 rotation;
		glm::vec3 rotation_offset;

		void Out(proto::Component* target);
		void In(const proto::Component& source);
	};

	struct Scale {
		Scale(glm::vec3 scale) : value(scale) { }
		Scale() { }

		glm::vec3 value;

		void Out(proto::Component* target);
		void In(const proto::Component& source);
	};
}
