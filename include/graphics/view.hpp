#pragma once

#include <glm/mat4x4.hpp>
#include "entity.hpp"

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

		static ReflectionComponent Reflection(View* val) {
			ReflectionComponent refcomp;
			Property prop(Property::BOOLEAN);
			(refcomp.properties2["Active"] = prop).Set<bool>(val->active);
			refcomp.properties2["Active"].update_func = [val] (Property& prop) { val->active = prop.Get<bool>(); };
			return std::move(refcomp);
		}
	};
}
