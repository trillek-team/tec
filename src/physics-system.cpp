#include "physics-system.hpp"

#include "components/collisionbody.hpp"
#include "entity.hpp"
#include "components/transforms.hpp"
#include "component-update-system.hpp"

#include "physics/physics-debug-drawer.hpp"
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

namespace tec {
	PhysicsDebugDrawer debug_drawer;

	PhysicsSystem::PhysicsSystem() {
		last_rayvalid = false;
		this->collisionConfiguration = new btDefaultCollisionConfiguration();
		this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
		this->broadphase = new btDbvtBroadphase();
		this->solver = new btSequentialImpulseConstraintSolver();
		this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->collisionConfiguration);
		this->dynamicsWorld->setGravity(btVector3(0, -7.0, 0));

		// Register the collision dispatcher with the GImpact algorithm for dynamic meshes.
		btCollisionDispatcher * dispatcher = static_cast<btCollisionDispatcher *>(this->dynamicsWorld->getDispatcher());
		btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);

		debug_drawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
		this->dynamicsWorld->setDebugDrawer(&debug_drawer);
	}

	PhysicsSystem::~PhysicsSystem() {
		if (this->dynamicsWorld != nullptr) {
			delete this->dynamicsWorld;
		}
		if (this->solver != nullptr) {
			delete this->solver;
		}
		if (this->collisionConfiguration != nullptr) {
			delete this->collisionConfiguration;
		}
		if (this->dispatcher != nullptr) {
			delete this->dispatcher;
		}
		if (this->broadphase != nullptr) {
			delete this->broadphase;
		}
	}

	void PhysicsSystem::Update(const double delta) {
		ProcessCommandQueue();

		for (auto itr = CollisionBodyMap::Begin(); itr != CollisionBodyMap::End(); ++itr) {
			auto entity_id = itr->first;
			glm::vec3 position(0.0);
			if (Entity(entity_id).Has<Position>()) {
				position = (Entity(entity_id).Get<Position>().lock())->value;
			}
			glm::quat orientation;
			if (Entity(entity_id).Has<Orientation>()) {
				orientation = (Entity(entity_id).Get<Orientation>().lock())->value;
			}
			btTransform transform(
				btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w),
				btVector3(position.x, position.y, position.z));

			if (this->bodies.find(entity_id) == this->bodies.end()) {
				if (CreateRigiedBody(entity_id, itr->second)) {
					this->bodies[entity_id]->setWorldTransform(transform);
					this->dynamicsWorld->addRigidBody(this->bodies[entity_id]);
				}
			}
			else {
				btRigidBody* body = this->bodies[entity_id];
				if (itr->second->new_collision_shape != itr->second->collision_shape) {
					this->dynamicsWorld->removeRigidBody(body);
					this->bodies.erase(entity_id);
					itr->second->shape.reset();
					itr->second->collision_shape = itr->second->new_collision_shape;
					if (CreateRigiedBody(entity_id, itr->second)) {
						body = this->bodies[entity_id];
						body->setWorldTransform(transform);
						this->dynamicsWorld->addRigidBody(this->bodies[entity_id]);
						break;
					}
				}
				body->setWorldTransform(transform);
				if (itr->second->mass != body->getInvMass()) {
					this->dynamicsWorld->removeRigidBody(body);
					btVector3 fallInertia(0, 0, 0);
					itr->second->shape->calculateLocalInertia(itr->second->mass, fallInertia);
					body->setMassProps(itr->second->mass, fallInertia);
					this->dynamicsWorld->addRigidBody(body);
				}

				int state = body->getActivationState();
				if (itr->second->disable_deactivation) {
					body->forceActivationState(DISABLE_DEACTIVATION);
				}
				else if (state == DISABLE_DEACTIVATION) {
					body->forceActivationState(ACTIVE_TAG);
				}

				if (itr->second->disable_rotation) {
					body->setAngularFactor(btVector3(0.0, 0, 0.0));
				}
			}
		}

		for (auto itr = VelocityMap::Begin(); itr != VelocityMap::End(); ++itr) {
			auto entity_id = itr->first;
			if (this->bodies.find(entity_id) != this->bodies.end()) {
				auto body = this->bodies.at(entity_id);
				body->setLinearVelocity(itr->second->GetLinear() + body->getGravity());
				body->setAngularVelocity(itr->second->GetAngular());
			}
		}

		btCollisionObjectArray& obj_array = this->dynamicsWorld->getCollisionObjectArray();
		for (std::size_t i = 0; i < obj_array.size(); ++i) {
			const CollisionBody* colbody = static_cast<const CollisionBody*>(obj_array.at(i)->getUserPointer());
			eid entity_id = colbody->entity_id;
			if (this->bodies.find(entity_id) == this->bodies.end()) {
				this->dynamicsWorld->removeCollisionObject(obj_array.at(i));
			}
		}

		this->dynamicsWorld->stepSimulation(delta, 10);

		for (auto body : this->bodies) {
			auto entity_id = body.first;
			Entity e(entity_id);
			auto transform = body.second->getWorldTransform();
			if (e.Has<Position>()) {
				auto pos = transform.getOrigin();
				auto position = std::make_shared<Position>(glm::vec3(pos.x(), pos.y(), pos.z()));
				e.Update(position);
			}
			if (e.Has<Orientation>()) {
				auto rot = transform.getRotation();
				auto orientation = std::make_shared<Orientation>(glm::highp_dquat(rot.w(), rot.x(), rot.y(), rot.z()));
				e.Update(orientation);
			}
		}
	}

	eid PhysicsSystem::RayCast(eid source_entity) {
		last_rayvalid = false;
		glm::vec3 position;
		if (Entity(source_entity).Has<Position>()) {
			position = (Entity(source_entity).Get<Position>().lock())->value;
		}
		glm::quat orientation;
		if (Entity(source_entity).Has<Orientation>()) {
			orientation = (Entity(source_entity).Get<Orientation>().lock())->value;
		}
		auto fv = position + glm::rotate(orientation, FORWARD_VECTOR * 300.f);
		btVector3 from(position.x, position.y, position.z), to(fv.x, fv.y, fv.z);
		last_rayfrom = from;
		btDynamicsWorld::AllHitsRayResultCallback cr(from, to);
		this->dynamicsWorld->rayTest(from, to, cr);
		if (cr.hasHit()) {
			int mx = cr.m_collisionObjects.size();
			double lastfrac = 1.1;
			int hc = mx;
			eid entity_hit = 0;
			for (int i = 0; i < mx; i++) {
				eid entity = 0;
				double frc = cr.m_hitFractions.at(i);
				const CollisionBody* coll = (const CollisionBody*)cr.m_collisionObjects.at(i)->getUserPointer();
				if (!coll) continue;
				entity = coll->entity_id;
				if (entity && entity != source_entity) {
					if (frc < lastfrac) {
						entity_hit = entity;
						hc = i;
						lastfrac = frc;
					}
				}
			}
			if (hc < mx) {
				last_raypos = cr.m_hitPointWorld.at(hc);
				last_raynorm = cr.m_hitNormalWorld.at(hc);
				last_raydist = last_rayfrom.distance(last_raypos);
				last_rayvalid = true;
				return entity_hit;
			}
		}
		return 0;
	}

	eid PhysicsSystem::RayCastIgnore(eid ign) {
		eid cam = 1; // TODO: This hardcoded number should be the active camera id.
		last_rayvalid = false;
		glm::vec3 position;
		if (Entity(cam).Has<Position>()) {
			position = (Entity(cam).Get<Position>().lock())->value;
		}
		glm::quat orientation;
		if (Entity(cam).Has<Orientation>()) {
			orientation = (Entity(cam).Get<Orientation>().lock())->value;
		}
		auto fv = position + glm::rotate(orientation, FORWARD_VECTOR * 300.f);
		btVector3 from(position.x, position.y, position.z), to(fv.x, fv.y, fv.z);
		last_rayfrom = from;
		btDynamicsWorld::AllHitsRayResultCallback cr(from, to);
		this->dynamicsWorld->rayTest(from, to, cr);
		if (cr.hasHit()) {
			int mx = cr.m_collisionObjects.size();
			double lastfrac = 1.1;
			int hc = mx;
			eid entity_hit = 0;
			for (int i = 0; i < mx; i++) {
				eid entity = 0;
				double frc = cr.m_hitFractions.at(i);
				const CollisionBody* coll = (const CollisionBody*)cr.m_collisionObjects.at(i)->getUserPointer();
				if (!coll) continue;
				entity = coll->entity_id;
				if (entity && entity != cam && entity != ign) {
					if (frc < lastfrac) {
						entity_hit = entity;
						hc = i;
						lastfrac = frc;
					}
				}
			}
			if (hc < mx) {
				last_raypos = cr.m_hitPointWorld.at(hc);
				last_raynorm = cr.m_hitNormalWorld.at(hc);
				last_raydist = last_rayfrom.distance(last_raypos);
				last_rayvalid = true;
				return entity_hit;
			}
		}
		return 0;
	}

	void PhysicsSystem::DebugDraw() {
		this->dynamicsWorld->debugDrawWorld();
		debug_drawer.UpdateVertexBuffer();
	}

	void PhysicsSystem::SetGravity(const unsigned int entity_id, const btVector3& f) {
		if (this->bodies.find(entity_id) != this->bodies.end()) {
			this->bodies[entity_id]->setGravity(f);
		}
	}

	void PhysicsSystem::SetNormalGravity(const unsigned int entity_id) {
		if (this->bodies.find(entity_id) != this->bodies.end()) {
			this->bodies[entity_id]->setGravity(this->dynamicsWorld->getGravity());
		}
	}

	bool PhysicsSystem::CreateRigiedBody(eid entity_id, std::shared_ptr<CollisionBody> collision_body) {
		btVector3 fallInertia(0, 0, 0);
		if (!collision_body->shape) {
			switch (collision_body->collision_shape) {
				case SPHERE:
					collision_body->shape = std::make_shared<btSphereShape>(collision_body->radius);
					break;
				case BOX:
					collision_body->shape = std::make_shared<btBoxShape>(collision_body->half_extents);
					break;
				case CAPSULE:
					collision_body->shape = std::make_shared<btCapsuleShape>(collision_body->radius, collision_body->height);
					break;
			}
		}
		if (collision_body->mass > 0.0) {
			if (collision_body->shape) {
				collision_body->shape->calculateLocalInertia(collision_body->mass, fallInertia);
			}
		}
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(collision_body->mass,
			collision_body->motion_state, collision_body->shape.get(), fallInertia);
		auto body = new btRigidBody(fallRigidBodyCI);

		if (!body) {
			return false;
		}
		collision_body->entity_id = entity_id;
		this->bodies[entity_id] = body;

		body->setUserPointer(collision_body.get());

		// Check if we want to disable automatic deactivation for this body.
		if (collision_body->disable_deactivation) {
			body->forceActivationState(DISABLE_DEACTIVATION);
		}

		// Prevent objects from rotating from physics system.
		if (collision_body->disable_rotation) {
			body->setAngularFactor(btVector3(0.0, 0, 0.0));
		}

		return true;
	}
}
