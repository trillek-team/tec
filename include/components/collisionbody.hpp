#pragma once

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <memory>
#include <vector>
#include <string>

#include "types.hpp"

namespace tec {
	enum COLLISION_SHAPE { SPHERE, CAPSULE, BOX, NONE };

	struct CollisionBody {
		struct MotionState : public btMotionState {
			btTransform transform;

			bool transform_updated;

			void getWorldTransform(btTransform& worldTrans) const {
				worldTrans = this->transform;
			}

			void setWorldTransform(const btTransform& worldTrans) {
				this->transform_updated = true;
				this->transform = worldTrans;
			}
		};
		CollisionBody(COLLISION_SHAPE collision_shape = NONE);
		~CollisionBody();

		void Out(proto::Component* target);
		void In(const proto::Component& source);

		COLLISION_SHAPE collision_shape;
		COLLISION_SHAPE new_collision_shape;

		btScalar mass; // For static objects mass must be 0.
		bool disable_deactivation = false; // Whether to disable automatic deactivation.
		bool disable_rotation; // prevent rotation from physics simulation.

		btVector3 half_extents; // For BOX shapes.
		float radius; // For SPHERE and CAPSULE shapes.
		float height; // For CAPSULE shapes.

		std::shared_ptr<btCollisionShape> shape;
		eid entity_id;
		MotionState motion_state;
	};
}
