#pragma once

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <memory>
#include <vector>

#include "types.hpp"

namespace tec {
	class Mesh;

	enum COLLISION_SHAPE { SPHERE, CAPSULE, STATIC_MESH, DYNAMIC_MESH };

	struct CollisionBody {
		CollisionBody(eid entity_id, COLLISION_SHAPE collision_shape,
			double radius = 1.0, double height = 1.0);
		~CollisionBody();

		bool SetMesh(const std::string mesh_name);

		bool SetMesh(std::shared_ptr<Mesh> mesh);

		COLLISION_SHAPE collision_shape;

		double radius; // Used for sphere and capsule shape collidable.
		double height; // Used for capsule shape collidable.
		btScalar mass; // For static objects mass must be 0.
		bool disable_deactivation; // Whether to disable automatic deactivation.
		bool disable_rotation; // prevent rotation from physics simulation.
		eid entity_id; // entity id that raycasts resolve to (this)

		std::shared_ptr<btTriangleMesh> mesh; // Used for mesh shape collidable.
		std::shared_ptr<Mesh> mesh_file; // Used for mesh shape collidable.

		btMotionState* motion_state;
		std::shared_ptr<btCollisionShape> shape;

	};
}
