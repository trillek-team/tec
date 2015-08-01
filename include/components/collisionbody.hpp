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
		
		void Out(proto::Component* target);
		void In(const proto::Component& source);

		static ReflectionComponent Reflection(CollisionBody* val);

		COLLISION_SHAPE collision_shape;
		COLLISION_SHAPE new_collision_shape;

		btScalar mass; // For static objects mass must be 0.
		bool disable_deactivation = false; // Whether to disable automatic deactivation.
		bool disable_rotation; // prevent rotation from physics simulation.

		btVector3 half_extents; // For BOX shapes.
		float radius; // For SPHERE and CAPSULE shapes.
		float height; // For CAPSULE shapes.

		btMotionState* motion_state;
		std::shared_ptr<btCollisionShape> shape;
		eid entity_id;
	};
	struct CollisionMesh : public CollisionBody {
		CollisionMesh(std::shared_ptr<Mesh> mesh, bool dynamic = false);

		std::shared_ptr<btTriangleMesh> mesh;
		std::shared_ptr<Mesh> mesh_file;
	};
}
