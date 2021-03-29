#pragma once

#include <memory>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "multiton.hpp"
#include "tec-types.hpp"
#include "transforms.hpp"

namespace tec {
using PositionMap = Multiton<eid, Position*>;
using OrientationMap = Multiton<eid, Orientation*>;
ATTRIBUTE_ALIGNED16(struct) CollisionBody {

	BT_DECLARE_ALIGNED_ALLOCATOR();

	struct MotionState : public btMotionState {
		MotionState(eid& _entity_id) : entity_id(_entity_id) { this->transform.setIdentity(); }
		MotionState(MotionState&& other) noexcept :
				transform(std::move(other.transform)), transform_updated(other.transform_updated),
				entity_id(other.entity_id) {}

		MotionState& operator=(MotionState&& other) noexcept {
			transform_updated = other.transform_updated;
			entity_id = other.entity_id;
			return *this;
		}

		void getWorldTransform(btTransform& worldTrans) const { worldTrans = this->transform; }

		void setWorldTransform(const btTransform& worldTrans) override {
			this->transform_updated = true;
			this->transform = worldTrans;
			if (entity_id != 0) {
				if (PositionMap::Has(entity_id)) {
					auto position = PositionMap::Get(entity_id);
					btVector3 origin = transform.getOrigin();
					position->value.x = origin.x();
					position->value.y = origin.y();
					position->value.z = origin.z();
				}
				if (OrientationMap::Has(entity_id)) {
					auto orientation = OrientationMap::Get(entity_id);
					btQuaternion rotation = transform.getRotation();
					orientation->value.x = rotation.x();
					orientation->value.y = rotation.y();
					orientation->value.z = rotation.z();
					orientation->value.w = rotation.w();
					rotation.getEulerZYX(orientation->rotation.z, orientation->rotation.y, orientation->rotation.x);
				}
			}
		}

		btTransform transform;
		bool transform_updated = true;

	private:
		eid& entity_id; // Stored to use when updated transform
		friend struct CollisionBody;
	};

	CollisionBody() = default;
	CollisionBody(CollisionBody && other) noexcept;

	CollisionBody& operator=(CollisionBody&& other) noexcept;

	void SetSphereShape(float);
	void SetCapsuleShape(float radius, float height);
	void SetBoxShape(float, float, float);

	void Out(proto::Component * target);
	void In(const proto::Component& source);

	btScalar mass = 0.0f; // For static objects mass must be 0.
	bool disable_deactivation = false; // Whether to disable automatic deactivation.
	bool disable_rotation = false; // prevent rotation from physics simulation.
	bool in_world = false;
	std::shared_ptr<btCollisionShape> shape;
	eid entity_id = 0; // Stored to use when doing lookups during collision
	MotionState motion_state{this->entity_id};
};
} // namespace tec
