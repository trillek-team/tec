// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <memory>

#include "types.hpp"

namespace tec {
	struct CollisionBody {
		struct MotionState : public btMotionState {
			MotionState() { }
			MotionState(MotionState&& other) : transform(std::move(other.transform)),
				transform_updated(other.transform_updated) {
			}
			
			MotionState& operator=(MotionState&& other) {
				transform_updated = other.transform_updated;
				transform = std::move(transform);
				return *this;
			}
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
		CollisionBody();
		CollisionBody(CollisionBody&& other);
		~CollisionBody();

		CollisionBody& operator=(CollisionBody&& other);

		void Out(proto::Component* target);
		void In(const proto::Component& source);

		btScalar mass; // For static objects mass must be 0.
		bool disable_deactivation = false; // Whether to disable automatic deactivation.
		bool disable_rotation; // prevent rotation from physics simulation.

		std::shared_ptr<btCollisionShape> shape;
		eid entity_id; // Stored to use when doing lookups during collision
		MotionState motion_state;
	};
}
