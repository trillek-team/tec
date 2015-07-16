#pragma once

#include <memory>
#include <map>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "types.hpp"
#include "entity.hpp"
#include "multiton.hpp"
#include "command-queue.hpp"

namespace tec {
	struct CollisionBody;

	struct Velocity {
		Velocity() : linear(0, 0, 0, 0), angular(0, 0, 0, 0) { };

		Velocity(glm::vec4 linear, glm::vec4 angular)
			: linear(linear), angular(angular) { };

		glm::vec4 linear;
		glm::vec4 angular;
		btVector3 GetLinear() const {
			return btVector3(linear.x, linear.y, linear.z);
		}
		btVector3 GetAngular() const {
			return btVector3(angular.x, angular.y, angular.z);
		}

		static ReflectionComponent Reflection(Velocity* val) {
			ReflectionComponent refcomp;
			refcomp.properties["linear_x"] = std::to_string(val->linear.x);
			refcomp.properties["linear_y"] = std::to_string(val->linear.y);
			refcomp.properties["linear_z"] = std::to_string(val->linear.z);
			refcomp.properties["angular_x"] = std::to_string(val->angular.x);
			refcomp.properties["angular_y"] = std::to_string(val->angular.y);
			refcomp.properties["angular_z"] = std::to_string(val->angular.z);
			return std::move(refcomp);
		}
	};

	class PhysicsSystem : public CommandQueue < PhysicsSystem > {
	public:
		PhysicsSystem();
		~PhysicsSystem();

		/** \brief
		*
		* This function is called once every frame. It is the only
		* function that can write data. This function is in the critical
		* path, job done here must be simple.
		*/
		void Update(const double delta);

		eid RayCast();
		eid RayCastIgnore(eid);
		glm::vec3 GetLastRayPos() const {
			btVector3 tmp = last_raypos; // grab a copy
			return glm::vec3(tmp.getX(), tmp.getY(), tmp.getZ());
		}
		double GetLastRayDistance() const {
			return last_raydist;
		}
		void RaySetInvalid() {
			last_rayvalid = false;
		}

		void DebugDraw();
	protected:
		/** \brief Set a rigid body's gravity.
		 *
		 * \param const unsigned int entity_id The entity ID of the rigid body.
		 * \param btVector3 f The rigid body's new gravity.
		 */
		void SetGravity(const unsigned int entity_id, const btVector3& f);

		/** \brief Set a rigid body's gravity to the world's gravity.
		 *
		 * \param const unsigned int entity_id The entity ID of the rigid body.
		 */
		void SetNormalGravity(const unsigned int entity_id);
	private:
		bool CreateRigiedBody(std::shared_ptr<CollisionBody> collision_body);

		typedef Multiton<eid, std::shared_ptr<CollisionBody>> CollisionBodyMap;
		typedef Multiton<eid, std::shared_ptr<Velocity>> VelocityMap;

		btBroadphaseInterface* broadphase;
		btCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDynamicsWorld* dynamicsWorld;

		std::map<eid, btRigidBody*> bodies;

		btVector3 last_rayfrom;
		double last_raydist;
		btVector3 last_raypos;
		btVector3 last_raynorm;
		bool last_rayvalid;
	};
}
