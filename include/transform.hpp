#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

namespace vv {
	static glm::vec3 FORWARD_VECTOR(0.0f, 0.0f, -1.0f);
	static glm::vec3 UP_VECTOR(0.0f, 1.0f, 0.0f);
	static glm::vec3 RIGHT_VECTOR(1.0f, 0.0f, 0.0f);

	struct Position {
		void Translate(const glm::vec3 amount) {
			this->value += amount;
		}

		void OrientedTranslate(glm::quat orientation, const glm::vec3 amount) {
			this->value += orientation * amount;
		}

		glm::vec3 value;
	};

	struct Orientation {
		void Rotate(const glm::vec3 amount) {
			this->rotation += amount;
			this->value = glm::normalize(glm::quat(this->rotation));
		}
		// Adds the rotation amount based on the current orientation.
		void OrientedRotate(const glm::vec3 amount) {
			this->rotation += amount;

			glm::quat qX = glm::angleAxis(amount.x, this->value * RIGHT_VECTOR);
			glm::quat qY = glm::angleAxis(amount.y, this->value * UP_VECTOR);
			glm::quat qZ = glm::angleAxis(amount.z, this->value * glm::vec3(0.0, 0.0, 1.0)); // Z axis not Forward
			glm::quat change = qX * qY * qZ;

			this->value = glm::normalize(change * this->value);
		}

		glm::quat value;
		glm::vec3 rotation;
	};
}
