// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <memory>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "types.hpp"

namespace tec {
	ATTRIBUTE_ALIGNED16(struct) CollisionBody {

		BT_DECLARE_ALIGNED_ALLOCATOR();

		struct MotionState : public btMotionState {
			MotionState() {
				this->transform.setIdentity();
			}
			MotionState(MotionState&& other) noexcept : transform(std::move(other.transform)),
				transform_updated(other.transform_updated) {}

			MotionState& operator=(MotionState&& other) noexcept {
				transform_updated = other.transform_updated;
				return *this;
			}

			void getWorldTransform(btTransform& worldTrans) const {
				worldTrans = this->transform;
			}

			void setWorldTransform(const btTransform& worldTrans) {
				this->transform_updated = true;
				this->transform = worldTrans;
			}

			btTransform transform;
			bool transform_updated = true;
		};

		CollisionBody() = default;
		CollisionBody(CollisionBody && other) noexcept;

		CollisionBody& operator=(CollisionBody && other) noexcept;

		void Out(proto::Component * target);
		void In(const proto::Component & source);

		btScalar mass = 0.0f;              // For static objects mass must be 0.
		bool disable_deactivation = false; // Whether to disable automatic deactivation.
		bool disable_rotation = false;     // prevent rotation from physics simulation.
		std::shared_ptr<btCollisionShape> shape;
		eid entity_id = 0; // Stored to use when doing lookups during collision
		MotionState motion_state;
	};
}
