// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <glm/mat4x4.hpp>

#include "components.pb.h"

namespace tec {
	struct View {
		View(bool active = false) : active(active) { }
		glm::mat4 view_matrix;
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
}
