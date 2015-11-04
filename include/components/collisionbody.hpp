#pragma once

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <memory>
#include <vector>
#include <string>

#include "types.hpp"
#include "entity.hpp"

namespace tec {
	class MeshFile;

	enum COLLISION_SHAPE { SPHERE, CAPSULE, BOX, STATIC_MESH, DYNAMIC_MESH, NONE };

	struct CollisionBody : public btMotionState {
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

		std::shared_ptr<btCollisionShape> shape;
		eid entity_id;

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
	struct CollisionMesh : public CollisionBody {
		CollisionMesh(std::shared_ptr<MeshFile> mesh, bool dynamic = false);

		std::shared_ptr<btTriangleMesh> mesh;
		std::shared_ptr<MeshFile> mesh_file;
	};
}
