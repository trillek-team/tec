#pragma once

#include <glm/mat4x4.hpp>
#include "entity.hpp"

#include "../proto/components.pb.h"

namespace tec {
	struct View {
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

		static ReflectionComponent Reflection(View* val) {
			ReflectionComponent refcomp;
			Property prop(Property::BOOLEAN);
			(refcomp.properties["Active"] = prop).Set<bool>(val->active);
			refcomp.properties["Active"].update_func = [val] (Property& prop) { val->active = prop.Get<bool>(); };
			return std::move(refcomp);
		}
	};
}
