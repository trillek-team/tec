#pragma once

#include <glm/mat4x4.hpp>
#include "../proto/components.pb.h"

namespace tec {
	struct View {
		glm::mat4 view_matrix;
		bool active = false;

		void Out(proto::View* target) {
			target->set_active(this->active);
		}

		void In(const proto::View& source) {
			if (source.has_active()) {
				this->active = source.active();
			}
		}
	};
}
