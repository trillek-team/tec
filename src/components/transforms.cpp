#include "components/transforms.hpp"

namespace tec {
	void Position::Translate(const glm::vec3 amount) {
		this->value += amount;
	}

	void Position::Translate(const glm::vec3 amount, const glm::quat orientation) {
		this->value += orientation * amount;
	}

	void Position::Out(proto::Component* target) {
		proto::Position* comp = target->mutable_position();
		comp->set_x(this->value.x);
		comp->set_y(this->value.y);
		comp->set_z(this->value.z);
	}

	void Position::In(const proto::Component& source) {
		const proto::Position& comp = source.position();
		if (comp.has_x()) {
			this->value.x = comp.x();
		}
		if (comp.has_y()) {
			this->value.y = comp.y();
		}
		if (comp.has_z()) {
			this->value.z = comp.z();
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

	void Orientation::Out(proto::Component* target) {
		proto::Orientation* comp = target->mutable_orientation();
		comp->set_x(this->value.x);
		comp->set_y(this->value.y);
		comp->set_z(this->value.z);
		comp->set_w(this->value.w);
	}

	void Orientation::In(const proto::Component& source) {
		const proto::Orientation& comp = source.orientation();
		if (comp.has_x()) {
			this->value.x = comp.x();
		}
		if (comp.has_y()) {
			this->value.y = comp.y();
		}
		if (comp.has_z()) {
			this->value.z = comp.z();
		}
		if (comp.has_w()) {
			this->value.w = comp.w();
		}
	}

	void Scale::Out(proto::Component* target) {
		proto::Scale* comp = target->mutable_scale();
		comp->set_x(this->value.x);
		comp->set_y(this->value.y);
		comp->set_z(this->value.z);
	}

	void Scale::In(const proto::Component& source) {
		const proto::Scale& comp = source.scale();
		if (comp.has_x()) {
			this->value.x = comp.x();
		}
		if (comp.has_y()) {
			this->value.y = comp.y();
		}
		if (comp.has_z()) {
			this->value.z = comp.z();
		}
	}
}
