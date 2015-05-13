#include "components/transforms.hpp"

namespace tec {
		void Position::Translate(const glm::vec3 amount) {
			this->value += amount;
		}

		void Position::Translate(const glm::vec3 amount, const glm::quat orientation) {
			this->value += orientation * amount;
		}
		
		void Orientation::Rotate(const glm::vec3 amount) {
			this->rotation += amount;
			this->value = glm::normalize(glm::quat(this->rotation));
		}

		// Adds the rotation amount based on the current orientation.
		void Orientation::OrientedRotate(const glm::vec3 amount) {
			this->rotation += amount;

			glm::quat qX = glm::angleAxis(amount.x, this->value * RIGHT_VECTOR);
			glm::quat qY = glm::angleAxis(amount.y, this->value * UP_VECTOR);
			glm::quat qZ = glm::angleAxis(amount.z, this->value * glm::vec3(0.0, 0.0, 1.0)); // Z axis not Forward
			glm::quat change = qX * qY * qZ;

			this->value = glm::normalize(change * this->value);
		}
}
