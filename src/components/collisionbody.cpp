#include "components/collisionbody.hpp"
#include "physics-system.hpp"
#include "resources/mesh.hpp"
#include "entity.hpp"
#include "components/transforms.hpp"

#include <BulletCollision/Gimpact/btGImpactShape.h>

namespace tec {
	CollisionBody::CollisionBody(COLLISION_SHAPE collision_shape) : collision_shape(collision_shape),
		new_collision_shape(collision_shape), mass(0.0), radius(1.0f), height(1.0f), disable_deactivation(false),
		disable_rotation(false), half_extents(btVector3(1.0, 1.0, 1.0)) {
		motion_state.transform_updated = true;
	}

	CollisionBody::~CollisionBody() { }

	void CollisionBody::Out(proto::Component* target) {
		proto::CollisionBody* comp = target->mutable_collision_body();
		comp->set_disable_deactivation(this->disable_deactivation);
		comp->set_disable_rotation(this->disable_rotation);
		comp->set_mass(static_cast<float>(this->mass));
		switch (this->new_collision_shape) {
			case COLLISION_SHAPE::BOX:
				{
					proto::CollisionBody::Box* box = comp->mutable_box();
					box->set_x_extent(static_cast<float>(this->half_extents.x()));
					box->set_y_extent(static_cast<float>(this->half_extents.y()));
					box->set_z_extent(static_cast<float>(this->half_extents.z()));
				}
				break;
			case COLLISION_SHAPE::SPHERE:
				{
					proto::CollisionBody::Sphere* sphere = comp->mutable_sphere();
					sphere->set_radius(this->radius);
				}
				break;
			case COLLISION_SHAPE::CAPSULE:
				{
					proto::CollisionBody::Capsule* capsule = comp->mutable_capsule();
					capsule->set_radius(this->radius);
					capsule->set_height(this->height);
				}
				break;
		}
	}

	void CollisionBody::In(const proto::Component& source) {
		const proto::CollisionBody& comp = source.collision_body();
		switch (comp.shape_case()) {
			case proto::CollisionBody::ShapeCase::kBox:
				this->new_collision_shape = BOX;
				this->half_extents = btVector3(comp.box().x_extent(), comp.box().y_extent(), comp.box().z_extent());
				break;
			case proto::CollisionBody::ShapeCase::kSphere:
				this->new_collision_shape = SPHERE;
				this->radius = comp.sphere().radius();
				break;
			case proto::CollisionBody::ShapeCase::kCapsule:
				this->new_collision_shape = CAPSULE;
				this->radius = comp.capsule().radius();
				this->height = comp.capsule().height();
				break;

		}

		if (comp.has_disable_deactivation()) {
			this->disable_deactivation = comp.disable_deactivation();
		}
		if (comp.has_disable_rotation()) {
			this->disable_rotation = comp.disable_rotation();
		}
		if (comp.has_mass()) {
			this->mass = comp.mass();
		}
	}
}
