#pragma once

#include <components.pb.h>
#include <glm/mat4x4.hpp>

namespace tec {
struct View {
	View(bool active = false): active(active) {}
	glm::mat4 view_matrix{0};
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
