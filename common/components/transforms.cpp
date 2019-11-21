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
		proto::Position* comp = target->mutable_position();
		comp->set_x(this->value.x);
		comp->set_y(this->value.y);
		comp->set_z(this->value.z);
		proto::Position::CenterOffset* offset = comp->mutable_offset();
		offset->set_x(this->center_offset.x);
		offset->set_y(this->center_offset.y);
		offset->set_z(this->center_offset.z);
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
		if (comp.has_offset()) {
			const proto::Position::CenterOffset& offset = comp.offset();
			if (offset.has_x()) {
				this->center_offset.x = offset.x();
			}
			if (offset.has_y()) {
				this->center_offset.y = offset.y();
			}
			if (offset.has_z()) {
				this->center_offset.z = offset.z();
			}
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
		proto::Orientation* comp = target->mutable_orientation();
		comp->set_x(this->value.x);
		comp->set_y(this->value.y);
		comp->set_z(this->value.z);
		comp->set_w(this->value.w);
		proto::Orientation::RotationOffset* offset = comp->mutable_offset();
		offset->set_x(this->rotation_offset.x);
		offset->set_y(this->rotation_offset.y);
		offset->set_z(this->rotation_offset.z);
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
		this->rotation = glm::eulerAngles(this->value);
		if (comp.has_offset()) {
			const proto::Orientation::RotationOffset& offset = comp.offset();
			if (offset.has_x()) {
				this->rotation_offset.x = offset.x();
			}
			if (offset.has_y()) {
				this->rotation_offset.y = offset.y();
			}
			if (offset.has_z()) {
				this->rotation_offset.z = offset.z();
			}
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
