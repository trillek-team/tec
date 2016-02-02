#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../proto/components.pb.h"
#include "reflection.hpp"

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

		static ReflectionComponent Reflection(Position* val) {
			ReflectionComponent refcomp;
			Property prop(Property::FLOAT);
			(refcomp.properties["X"] = prop).Set(val->value.x);
			refcomp.properties["X"].update_func = [val] (Property& prop) {
				val->value.x = prop.Get<float>();
			};
			(refcomp.properties["Y"] = prop).Set(val->value.y);
			refcomp.properties["Y"].update_func = [val] (Property& prop) {
				val->value.y = prop.Get<float>();
			};
			(refcomp.properties["Z"] = prop).Set(val->value.z);
			refcomp.properties["Z"].update_func = [val] (Property& prop) {
				val->value.z = prop.Get<float>();
			};
			(refcomp.properties["Offset X"] = prop).Set(val->center_offset.x);
			refcomp.properties["Offset X"].update_func = [val] (Property& prop) {
				val->center_offset.x = prop.Get<float>();
			};
			(refcomp.properties["Offset Y"] = prop).Set(val->center_offset.y);
			refcomp.properties["Offset Y"].update_func = [val] (Property& prop) {
				val->center_offset.y = prop.Get<float>();
			};
			(refcomp.properties["Offset Z"] = prop).Set(val->center_offset.z);
			refcomp.properties["Offset Z"].update_func = [val] (Property& prop) {
				val->center_offset.z = prop.Get<float>();
			};
			return std::move(refcomp);
		}
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

		static ReflectionComponent Reflection(Orientation* val) {
			ReflectionComponent refcomp;
			Property prop(Property::FLOAT);
			(refcomp.properties["Rotation X"] = prop).Set(glm::degrees(val->rotation.x));
			refcomp.properties["Rotation X"].update_func = [val] (Property& prop) {
				val->rotation.x = glm::radians(prop.Get<float>());
				val->value = glm::normalize(glm::quat(val->rotation));
			};
			(refcomp.properties["Rotation Y"] = prop).Set(glm::degrees(val->rotation.y));
			refcomp.properties["Rotation Y"].update_func = [val] (Property& prop) {
				val->rotation.y = glm::radians(prop.Get<float>());
				val->value = glm::normalize(glm::quat(val->rotation));
			};
			(refcomp.properties["Rotation Z"] = prop).Set(glm::degrees(val->rotation.z));
			refcomp.properties["Rotation Z"].update_func = [val] (Property& prop) {
				val->rotation.z = glm::radians(prop.Get<float>());
				val->value = glm::normalize(glm::quat(val->rotation));
			};
			(refcomp.properties["Offset X"] = prop).Set(glm::degrees(val->rotation_offset.x));
			refcomp.properties["Offset X"].update_func = [val] (Property& prop) {
				val->rotation_offset.x = glm::radians(prop.Get<float>());
			};
			(refcomp.properties["Offset Y"] = prop).Set(glm::degrees(val->rotation_offset.y));
			refcomp.properties["Offset Y"].update_func = [val] (Property& prop) {
				val->rotation_offset.y = glm::radians(prop.Get<float>());
			};
			(refcomp.properties["Offset Z"] = prop).Set(glm::degrees(val->rotation_offset.z));
			refcomp.properties["Offset Z"].update_func = [val] (Property& prop) {
				val->rotation_offset.z = glm::radians(prop.Get<float>());
			};
			return std::move(refcomp);
		}
	};

	struct Scale {
		Scale(glm::vec3 scale) : value(scale) { }
		Scale() { }

		glm::vec3 value;

		void Out(proto::Component* target);
		void In(const proto::Component& source);

		static ReflectionComponent Reflection(Scale* val) {
			ReflectionComponent refcomp;
			Property prop(Property::FLOAT);
			(refcomp.properties["x"] = prop).Set(val->value.x);
			refcomp.properties["x"].update_func = [val] (Property& prop) {
				val->value.x = prop.Get<float>();
			};
			(refcomp.properties["y"] = prop).Set(val->value.y);
			refcomp.properties["y"].update_func = [val] (Property& prop) {
				val->value.y = prop.Get<float>();
			};
			(refcomp.properties["z"] = prop).Set(val->value.z);
			refcomp.properties["z"].update_func = [val] (Property& prop) {
				val->value.z = prop.Get<float>();
			};
			return std::move(refcomp);
		}
	};
}
