#pragma once

#include <map>
#include <memory>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "command-queue.hpp"
#include "event-system.hpp"
#include "game-state.hpp"
#include "tec-types.hpp"

namespace tec {
struct CollisionBody;
struct MouseBtnEvent;
struct EntityCreated;
struct EntityDestroyed;

class PhysicsSystem :
	public CommandQueue<PhysicsSystem>,
	EventQueue<MouseBtnEvent>,
	EventQueue<EntityCreated>,
	EventQueue<EntityDestroyed> {
public:
	PhysicsSystem();
	~PhysicsSystem();

	// sets a different substep limit, if zero, then update delta must be a constant
	void SetSubstepping(int substep) { simulation_substeps = substep; }

	std::set<eid> Update(const double delta, const GameState& state);

	eid RayCastMousePick(eid source_entity,
		double mouse_x = 0.0f,
		double mouse_y = 0.0f,
		float screen_width = 1.0f,
		float screen_height = 1.0f);
	eid RayCastIgnore(eid source_entity, eid ignore_entity);
	glm::vec3 GetLastRayPos() const { return glm::vec3(last_raypos.getX(), last_raypos.getY(), last_raypos.getZ()); }
	double GetLastRayDistance() const { return last_raydist; }
	void RaySetInvalid() { last_rayvalid = false; }

	void DebugDraw();

	using EventQueue<MouseBtnEvent>::On;
	using EventQueue<EntityCreated>::On;
	using EventQueue<EntityDestroyed>::On;
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
	bool AddRigidBody(CollisionBody* collision_body);
	void RemoveRigidBody(eid entity_id);

	btBroadphaseInterface* broadphase;
	btCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDynamicsWorld* dynamicsWorld;
	int simulation_substeps = 10;

	std::map<eid, btRigidBody*> bodies;

	btVector3 last_rayfrom;
	double last_raydist{0.0};
	btVector3 last_raypos;
	btVector3 last_raynorm;
	bool last_rayvalid{false};
	eid last_entity_hit{0};
};
} // end namespace tec
