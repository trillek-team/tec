#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "types.hpp"
#include "reflection.hpp"

namespace tec {
	struct Velocity {
		Velocity() : linear(0, 0, 0, 0), angular(0, 0, 0, 0) { };

		Velocity(glm::vec4 linear, glm::vec4 angular)
			: linear(linear), angular(angular) { };

		glm::vec4 linear;
		glm::vec4 angular;
		btVector3 GetLinear() const {
			return btVector3(linear.x, linear.y, linear.z);
		}
		btVector3 GetAngular() const {
			return btVector3(angular.x, angular.y, angular.z);
		}

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

		static ReflectionComponent Reflection(Velocity* val) {
			ReflectionComponent refcomp;
			Property prop(Property::FLOAT);
			(refcomp.properties["Linear X"] = prop).Set<float>(val->linear.x);
			refcomp.properties["Linear X"].update_func = [val] (Property& prop) { val->linear.x = prop.Get<float>(); };
			(refcomp.properties["Linear Y"] = prop).Set<float>(val->linear.y);
			refcomp.properties["Linear Y"].update_func = [val] (Property& prop) { val->linear.y = prop.Get<float>(); };
			(refcomp.properties["Linear Z"] = prop).Set<float>(val->linear.z);
			refcomp.properties["Linear Z"].update_func = [val] (Property& prop) { val->linear.z = prop.Get<float>(); };
			(refcomp.properties["Angular X"] = prop).Set<float>(val->angular.x);
			refcomp.properties["Angular X"].update_func = [val] (Property& prop) { val->angular.x = prop.Get<float>(); };
			(refcomp.properties["Angular Y"] = prop).Set<float>(val->angular.y);
			refcomp.properties["Angular Y"].update_func = [val] (Property& prop) { val->angular.y = prop.Get<float>(); };
			(refcomp.properties["Angular Z"] = prop).Set<float>(val->angular.z);
			refcomp.properties["Angular Z"].update_func = [val] (Property& prop) { val->angular.z = prop.Get<float>(); };

			return std::move(refcomp);
		}
	};
}
