#pragma once

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <memory>
#include <vector>
#include <string>

#include "types.hpp"

namespace tec {
	class Mesh;

	enum COLLISION_SHAPE { SPHERE, CAPSULE, BOX, STATIC_MESH, DYNAMIC_MESH };

	struct CollisionBody {
		CollisionBody(eid entity_id, COLLISION_SHAPE collision_shape);
		~CollisionBody();

		COLLISION_SHAPE collision_shape;

		double height; // Used for capsule shape collidable.
		btScalar mass; // For static objects mass must be 0.
		bool disable_deactivation; // Whether to disable automatic deactivation.
		bool disable_rotation; // prevent rotation from physics simulation.
		eid entity_id; // entity id that raycasts resolve to (this)

		btMotionState* motion_state;
		std::shared_ptr<btCollisionShape> shape;
	};
	struct CollisionBox : public CollisionBody {
		CollisionBox(eid entity_id, float width, float height, float depth) :
			CollisionBody(entity_id, BOX), half_extents(width, height, depth) {
			this->shape = std::make_shared<btBoxShape>(this->half_extents);
		}
		btVector3 half_extents;
	};
	struct CollisionSphere : public CollisionBody {
		CollisionSphere(eid entity_id, float radius) :
			CollisionBody(entity_id, SPHERE), radius(radius) {
			this->shape = std::make_shared<btSphereShape>(this->radius);
		}
		float radius;
	};
	struct CollisionCapsule : public CollisionBody {
		CollisionCapsule(eid entity_id, float radius, float height) :
			CollisionBody(entity_id, CAPSULE), radius(radius), height(height) {
			this->shape = std::make_shared<btCapsuleShape>(this->radius, this->height);
		}
		float radius, height;
	};
	struct CollisionMesh : public CollisionBody {
		CollisionMesh(eid entity_id, std::shared_ptr<Mesh> mesh, bool dynamic = false);

		std::shared_ptr<btTriangleMesh> mesh;
		std::shared_ptr<Mesh> mesh_file;
	};
}
