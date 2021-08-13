#pragma once

#include <components.pb.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace tec {
struct View {
	View(bool active = false) : active(active) {}
	glm::vec3 view_pos{0};
	glm::quat view_quat;
	bool active = false;

	void Out(proto::Component* target) {
		proto::View* comp = target->mutable_view();
		comp->set_active(this->active);
	}

	void In(const proto::Component& source) {
		const proto::View& comp = source.view();
		if (comp.has_active()) {
			this->active = comp.active();
		}
	}
};
} // namespace tec
