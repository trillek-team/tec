#pragma once

#include <memory>
#include <functional>

#include "types.hpp"

namespace tec {
	struct Shape {
		float half_extents[3];
		float radius, height;
		proto::Shape::ShapeCase shape_case;

		void Out(proto::Component* target) {
			proto::Shape* comp = target->mutable_shape();
			switch (this->shape_case) {
			case proto::Shape::ShapeCase::kBox:
			{
				proto::Shape::Box* box = comp->mutable_box();
				box->set_x_extent(this->half_extents[0]);
				box->set_y_extent(this->half_extents[1]);
				box->set_z_extent(this->half_extents[2]);
			}
			break;
			case proto::Shape::ShapeCase::kSphere:
			{
				proto::Shape::Sphere* sphere = comp->mutable_sphere();
				sphere->set_radius(this->radius);
			}
			break;
			case proto::Shape::ShapeCase::kCapsule:
			{
				proto::Shape::Capsule* capsule = comp->mutable_capsule();
				capsule->set_radius(this->radius);
				capsule->set_height(this->height);
			}
			break;
			}
		}
		void In(const proto::Component& source) {
			const proto::Shape& comp = source.shape();
			shape_case = comp.shape_case();
			switch (shape_case) {
			case proto::Shape::ShapeCase::kBox:
			{
				this->half_extents[0] = comp.box().x_extent();
				this->half_extents[1] = comp.box().y_extent();
				this->half_extents[2] = comp.box().z_extent();
			}
			break;
			case proto::Shape::ShapeCase::kSphere:
			{
				this->radius = comp.sphere().radius();
			}
			break;
			case proto::Shape::ShapeCase::kCapsule:
			{
				this->radius = comp.capsule().radius();
				this->height = comp.capsule().height();
			}
			break;
			}
		}
	};
}
