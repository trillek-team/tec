#pragma once

#include <memory>
#include <map>
#include <set>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "types.hpp"
#include "entity.hpp"
#include "multiton.hpp"
#include "command-queue.hpp"
#include "event-system.hpp"
#include "components/velocity.hpp"

namespace tec {
	struct CollisionBody;
	struct MouseBtnEvent;

	class PhysicsSystem : public CommandQueue < PhysicsSystem >, EventQueue < MouseBtnEvent > {
	public:
		PhysicsSystem();
		~PhysicsSystem();

		/** \brief
		*
		* This function is called once every frame. It is the only
		* function that can write data. This function is in the critical
		* path, job done here must be simple.
		*/
		std::set<eid> Update(const double delta);

		eid RayCastMousePick(eid source_entity, double mouse_x = 0.0f, double mouse_y = 0.0f,
			float screen_width = 1.0f, float screen_height = 1.0f);
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
		void On(std::shared_ptr<MouseBtnEvent> data);

		std::shared_ptr<Position> GetPosition(eid entity_id);
		std::shared_ptr<Orientation> GetOrientation(eid entity_id);
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
		bool CreateRigiedBody(eid entity_id, std::shared_ptr<CollisionBody> collision_body);

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
		eid last_entity_hit;
	};
}
