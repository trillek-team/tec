// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "transforms.hpp"

namespace tec {
	void Position::Translate(const glm::vec3 amount) {
		this->value += amount;
	}

	void Position::Translate(const glm::vec3 amount, const glm::quat orientation) {
		this->value += orientation * amount;
	}

	void Position::Out(proto::Component* target) {
		this->Out(target->mutable_position());
	}

	void Position::Out(proto::Position* comp) {
		comp->set_x(this->value.x);
		comp->set_y(this->value.y);
		comp->set_z(this->value.z);
	}

	void Position::In(const proto::Component& source) {
		this->In(source.position());
	}

	void Position::In(const proto::Position& comp) {
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
		this->rotation = this->rotation + amount;
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
		this->Out(target->mutable_orientation());
	}

	void Orientation::Out(proto::Orientation* comp) {
		comp->set_x(this->value.x);
		comp->set_y(this->value.y);
		comp->set_z(this->value.z);
		comp->set_w(this->value.w);
	}

	void Orientation::In(const proto::Component& source) {
		this->In(source.orientation());
	}

	void Orientation::In(const proto::Orientation& comp) {
		if (comp.has_w()) {
			this->value.w = comp.w();
			this->value.x = comp.x();
			this->value.y = comp.y();
			this->value.z = comp.z();
		}
		else {
			Rotate(glm::vec3(comp.x(), comp.y(), comp.z()));
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
