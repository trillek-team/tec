#include "collision-body.hpp"

namespace tec {
CollisionBody::CollisionBody(CollisionBody&& other) noexcept :
		mass(other.mass), disable_deactivation(other.disable_deactivation), disable_rotation(other.disable_rotation),
		shape(std::move(other.shape)), entity_id(other.entity_id), motion_state(std::move(other.motion_state)) {}

CollisionBody& CollisionBody::operator=(CollisionBody&& other) noexcept {
	mass = other.mass;
	disable_deactivation = other.disable_deactivation;
	disable_rotation = other.disable_rotation;
	motion_state = std::move(other.motion_state);
	shape = std::move(other.shape);
	entity_id = other.entity_id;
	return *this;
}

void CollisionBody::Out(proto::Component* target) {
	proto::CollisionBody* comp = target->mutable_collision_body();
	comp->set_disable_deactivation(this->disable_deactivation);
	comp->set_disable_rotation(this->disable_rotation);
	comp->set_mass(static_cast<float>(this->mass));
	if (this->shape) {
		switch (this->shape->getShapeType()) {
		case BOX_SHAPE_PROXYTYPE:
		{
			proto::SizeVector* box = comp->mutable_box();
			btVector3 half_extents = std::static_pointer_cast<btBoxShape>(this->shape)->getHalfExtentsWithMargin();
			box->set_x(static_cast<float>(half_extents.getX()));
			box->set_y(static_cast<float>(half_extents.getY()));
			box->set_z(static_cast<float>(half_extents.getZ()));
		} break;
		case SPHERE_SHAPE_PROXYTYPE:
		{
			proto::CollisionBody::Sphere* sphere = comp->mutable_sphere();
			sphere->set_radius(static_cast<float>(std::static_pointer_cast<btSphereShape>(this->shape)->getRadius()));
		} break;
		case CAPSULE_SHAPE_PROXYTYPE:
		{
			proto::CollisionBody::Capsule* capsule = comp->mutable_capsule();
			auto capsule_shape = std::static_pointer_cast<btCapsuleShape>(this->shape);
			capsule->set_radius(static_cast<float>(capsule_shape->getRadius()));
			capsule->set_height(static_cast<float>(capsule_shape->getHalfHeight() * 2.0f));
		} break;
		}
	}
}
void CollisionBody::SetSphereShape(float radius) { this->shape.reset(new btSphereShape(radius)); }
void CollisionBody::SetCapsuleShape(float radius, float height) {
	this->shape.reset(new btCapsuleShape(radius, height));
}
void CollisionBody::SetBoxShape(float x, float y, float z) { this->shape.reset(new btBoxShape(btVector3(x, y, z))); }

void CollisionBody::In(const proto::Component& source) {
	const proto::CollisionBody& comp = source.collision_body();
	switch (comp.shape_case()) {
	case proto::CollisionBody::ShapeCase::kBox:
	{
		this->SetBoxShape(comp.box().x(), comp.box().y(), comp.box().z());
		break;
	}
	case proto::CollisionBody::ShapeCase::kSphere:
	{
		this->SetSphereShape(comp.sphere().radius());
		break;
	}
	case proto::CollisionBody::ShapeCase::kCapsule:
	{
		this->SetCapsuleShape(comp.capsule().radius(), comp.capsule().height());
		break;
	}
	case proto::CollisionBody::ShapeCase::SHAPE_NOT_SET: break;
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
} // namespace tec
