#pragma once

#include <components.pb.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace tec {
static const glm::vec3 FORWARD_VECTOR = {0.0f, 0.0f, -1.0f};
static const glm::vec3 UP_VECTOR = {0.0f, 1.0f, 0.0f};
static const glm::vec3 RIGHT_VECTOR = {1.0f, 0.0f, 0.0f};

struct Position {
	Position(glm::vec3 pos) : value(pos) {}
	Position() = default;
	// Translates by amount.
	void Translate(const glm::vec3 amount);

	// Translates by amount in direction orientation.
	void Translate(const glm::vec3 amount, const glm::quat orientation);

	glm::vec3 value = {0.0f, 0.0f, 0.0f};

	void Out(proto::Component* target) const;
	void Out(proto::Position* comp) const; // Can be used to save into a specific component
	void In(const proto::Component& source);
	void In(const proto::Position& comp); // Can be used to load in a specific component
};

struct Orientation {
	Orientation(glm::vec3 rot) { Rotate(rot); }
	Orientation(glm::quat q) : value(q), rotation(glm::eulerAngles(q)) {}
	Orientation() = default;
	void Rotate(const glm::vec3 amount = glm::vec3(0.0f));

	// Adds the rotation amount based on the current orientation.
	void OrientedRotate(const glm::vec3 amount);

	glm::quat value{1.f, 0.f, 0.f, 0.f}; // w first
	glm::vec3 rotation{0.f, 0.f, 0.f};

	void Out(proto::Component* target) const;
	void Out(proto::Quaternion* comp) const; // Can be used to save into a specific component
	void In(const proto::Component& source);
	void In(const proto::Quaternion& comp); // Can be used to load in a specific component
};

struct Scale {
	Scale(glm::vec3 scale) : value(scale) {}
	Scale() = default;

	glm::vec3 value{0, 0, 0};

	void Out(proto::Component* target);
	void In(const proto::Component& source);
};
} // namespace tec
