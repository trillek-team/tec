#pragma once

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <memory>
#include <vector>
#include <string>

#include "types.hpp"
#include "entity.hpp"

namespace tec {
	class Mesh;

	enum COLLISION_SHAPE { SPHERE, CAPSULE, BOX, STATIC_MESH, DYNAMIC_MESH, NONE };

	struct CollisionBody {
		CollisionBody(COLLISION_SHAPE collision_shape = NONE);
		~CollisionBody();

		static ReflectionComponent Reflection(CollisionBody* val);

		COLLISION_SHAPE collision_shape;

		btScalar mass; // For static objects mass must be 0.
		bool disable_deactivation = false; // Whether to disable automatic deactivation.
		bool disable_rotation; // prevent rotation from physics simulation.

		btMotionState* motion_state;
		std::shared_ptr<btCollisionShape> shape;
		eid entity_id;
	};
	struct CollisionBox : public CollisionBody {
		CollisionBox(float width = 5.0f, float height = 5.0f, float depth = 5.0f) : CollisionBody(BOX), half_extents(width, height, depth) {
			this->shape = std::make_shared<btBoxShape>(this->half_extents);
		}
		btVector3 half_extents;
	};
	struct CollisionSphere : public CollisionBody {
		CollisionSphere(float radius = 1.0f) : CollisionBody(SPHERE), radius(radius) {
			this->shape = std::make_shared<btSphereShape>(this->radius);
		}
		float radius;
	};
	struct CollisionCapsule : public CollisionBody {
		CollisionCapsule(float radius = 1.0f, float height = 1.0f) : CollisionBody(CAPSULE), radius(radius), height(height) {
			this->shape = std::make_shared<btCapsuleShape>(this->radius, this->height);
		}
		float radius, height;
	};
	struct CollisionMesh : public CollisionBody {
		CollisionMesh(std::shared_ptr<Mesh> mesh, bool dynamic = false);

		std::shared_ptr<btTriangleMesh> mesh;
		std::shared_ptr<Mesh> mesh_file;
	};
}
