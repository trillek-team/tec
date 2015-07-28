#include "components/transforms.hpp"

namespace tec {
	void Position::Translate(const glm::vec3 amount) {
		this->value += amount;
	}

	void Position::Translate(const glm::vec3 amount, const glm::quat orientation) {
		this->value += orientation * amount;
	}

	void Position::Out(proto::Position* target) {
		target->set_x(this->value.x);
		target->set_y(this->value.y);
		target->set_z(this->value.z);
	}

	void Position::In(const proto::Position& source) {
		if (source.has_x()) {
			this->value.x = source.x();
		}
		if (source.has_y()) {
			this->value.y = source.y();
		}
		if (source.has_z()) {
			this->value.z = source.z();
		}
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

	void Orientation::Out(proto::Orientation* target) {
		target->set_x(this->value.x);
		target->set_y(this->value.y);
		target->set_z(this->value.z);
		target->set_w(this->value.w);
	}

	void Orientation::In(const proto::Orientation& source) {
		this->value.x = source.x();
		this->value.y = source.y();
		this->value.z = source.z();
		this->value.w = source.w();
	}
}
