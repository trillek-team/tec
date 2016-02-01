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
#include "game-state.hpp"
#include "components/velocity.hpp"

namespace tec {
	struct CollisionBody;
	struct MouseBtnEvent;

	class PhysicsSystem : public CommandQueue < PhysicsSystem >,
		EventQueue < MouseBtnEvent >, EventQueue < EntityCreated >,
		EventQueue < EntityDestroyed > {
	public:
		PhysicsSystem();
		~PhysicsSystem();

		std::set<eid> Update(const double delta, const GameState& state);

		eid RayCastMousePick(eid source_entity, double mouse_x = 0.0f, double mouse_y = 0.0f,
			float screen_width = 1.0f, float screen_height = 1.0f);
		eid RayCastIgnore(eid);
		glm::vec3 GetLastRayPos() const {
			return glm::vec3(last_raypos.getX(), last_raypos.getY(), last_raypos.getZ());
		}
		double GetLastRayDistance() const {
			return last_raydist;
		}
		void RaySetInvalid() {
			last_rayvalid = false;
		}

		void DebugDraw();
		void On(std::shared_ptr<MouseBtnEvent> data);
		void On(std::shared_ptr<EntityCreated> data);
		void On(std::shared_ptr<EntityDestroyed> data);

		Position GetPosition(eid entity_id);
		Orientation GetOrientation(eid entity_id);
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
		bool CreateRigiedBody(eid entity_id, CollisionBody&& collision_body);

		bool UpdateCollisionShape(eid entity_id);

		typedef Multiton<eid, std::shared_ptr<Velocity>> VelocityMap;

		btBroadphaseInterface* broadphase;
		btCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDynamicsWorld* dynamicsWorld;
		
		std::map<eid, btRigidBody*> bodies;
		std::map<eid, CollisionBody> collidables;

		btVector3 last_rayfrom;
		double last_raydist;
		btVector3 last_raypos;
		btVector3 last_raynorm;
		bool last_rayvalid;
		eid last_entity_hit;
	};
}
