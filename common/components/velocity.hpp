#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "tec-types.hpp"

namespace tec {
struct Velocity {
	Velocity() : linear(0, 0, 0), angular(0, 0, 0){};

	Velocity(glm::vec3 linear, glm::vec3 angular) : linear(linear), angular(angular){};

	glm::vec3 linear;
	glm::vec3 angular;
	btVector3 GetLinear() const { return btVector3(linear.x, linear.y, linear.z); }
	btVector3 GetAngular() const { return btVector3(angular.x, angular.y, angular.z); }

	void Out(proto::Component* target) {
		proto::Velocity* comp = target->mutable_velocity();
		comp->set_linear_x(this->linear.x);
		comp->set_linear_y(this->linear.y);
		comp->set_linear_z(this->linear.z);
		comp->set_angular_x(this->angular.x);
		comp->set_angular_y(this->angular.y);
		comp->set_angular_z(this->angular.z);
	}

	void In(const proto::Component& source) {
		const proto::Velocity& comp = source.velocity();
		if (comp.has_linear_x()) {
			this->linear.x = comp.linear_x();
		}
		if (comp.has_linear_y()) {
			this->linear.y = comp.linear_y();
		}
		if (comp.has_linear_z()) {
			this->linear.z = comp.linear_z();
		}
		if (comp.has_angular_x()) {
			this->angular.x = comp.angular_x();
		}
		if (comp.has_angular_y()) {
			this->angular.y = comp.angular_y();
		}
		if (comp.has_angular_z()) {
			this->angular.z = comp.angular_z();
		}
	}
};
} // namespace tec
