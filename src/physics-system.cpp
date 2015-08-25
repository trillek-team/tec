#include "physics-system.hpp"

#include "components/collisionbody.hpp"
#include "components/transforms.hpp"
#include "component-update-system.hpp"
#include "entity.hpp"
#include "events.hpp"

#include "physics/physics-debug-drawer.hpp"
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <glm/gtc/matrix_transform.hpp>

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

		debug_drawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);
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
				itr->second->motion_state = new btDefaultMotionState(transform);
				if (CreateRigiedBody(entity_id, itr->second)) {
					this->dynamicsWorld->addRigidBody(this->bodies[entity_id]);
				}
			}
			btRigidBody* body = this->bodies[entity_id];
			this->dynamicsWorld->removeRigidBody(body);
			if (itr->second->new_collision_shape != itr->second->collision_shape) {
				this->bodies.erase(entity_id);
				itr->second->shape.reset();
				itr->second->collision_shape = itr->second->new_collision_shape;
				if (CreateRigiedBody(entity_id, itr->second)) {
					body = this->bodies[entity_id];
					body->setWorldTransform(transform);
					break;
				}
			}
			if (itr->second->mass != body->getInvMass()) {
				btVector3 fallInertia(0, 0, 0);
				itr->second->shape->calculateLocalInertia(itr->second->mass, fallInertia);
				body->setMassProps(itr->second->mass, fallInertia);
				body->updateInertiaTensor();
				body->clearForces();
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

			body->setWorldTransform(transform);
			this->dynamicsWorld->addRigidBody(this->bodies[entity_id]);
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
		for (int i = 0; i < obj_array.size(); ++i) {
			const CollisionBody* colbody = static_cast<const CollisionBody*>(obj_array.at(i)->getUserPointer());
			eid entity_id = colbody->entity_id;
			if (this->bodies.find(entity_id) == this->bodies.end()) {
				this->dynamicsWorld->removeCollisionObject(obj_array.at(i));
			}
		}

		this->dynamicsWorld->stepSimulation(delta, 10);

		for (auto body : this->bodies) {
			auto entity_id = body.first;
			if (body.second->getActivationState() == DISABLE_SIMULATION) {
				continue;
			}
			Entity e(entity_id);
			auto transform = body.second->getWorldTransform();
			if (e.Has<Position>()) {
				std::shared_ptr<Position> old_position = e.Get<Position>().lock();
				auto pos = transform.getOrigin();
				auto position = std::make_shared<Position>(*old_position);
				position->value = glm::vec3(pos.x(), pos.y(), pos.z());
				e.Update(position);
			}
			if (e.Has<Orientation>()) {
				std::shared_ptr<Orientation> old_orientation = e.Get<Orientation>().lock();
				auto rot = transform.getRotation();
				auto orientation = std::make_shared<Orientation>(*old_orientation);
				orientation->value = glm::highp_dquat(rot.w(), rot.x(), rot.y(), rot.z());
				e.Update(orientation);
			}
		}
	}

	glm::vec3 GetRayDirection(float mouse_x, float mouse_y, float screen_width, float screen_height, glm::mat4 view, glm::mat4 projection) {
		glm::vec4 lRayStart_NDC((mouse_x / screen_width - 0.5f) * 2.0f, (mouse_y / screen_height - 0.5f) * -2.0f, -1.0, 1.0f);
		glm::vec4 lRayEnd_NDC((mouse_x / screen_width - 0.5f) * 2.0f, (mouse_y / screen_height - 0.5f) * -2.0f, 0.0, 1.0f);

		glm::mat4 inverted_viewprojection = glm::inverse(projection * view);
		glm::vec4 lRayStart_world = inverted_viewprojection * lRayStart_NDC; lRayStart_world /= lRayStart_world.w;
		glm::vec4 lRayEnd_world = inverted_viewprojection * lRayEnd_NDC; lRayEnd_world /= lRayEnd_world.w;
		glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
		lRayDir_world = glm::normalize(lRayDir_world);
		return lRayDir_world;
	}

	eid PhysicsSystem::RayCastMousePick(eid source_entity, float mouse_x, float mouse_y, float screen_width, float screen_height) {
		this->last_rayvalid = false;
		glm::vec3 position;
		if (Entity(source_entity).Has<Position>()) {
			position = (Entity(source_entity).Get<Position>().lock())->value;
		}
		glm::quat orientation;
		if (Entity(source_entity).Has<Orientation>()) {
			orientation = (Entity(source_entity).Get<Orientation>().lock())->value;
		}

		// TODO: This could be pulled from something but it seems unlikely to change.
		static glm::mat4 projection = glm::perspective(
			glm::radians(45.0f),
			screen_width / screen_height,
			-1.0f,
			300.0f
			);
		glm::mat4 view = glm::inverse(glm::translate(glm::mat4(1.0), position) * glm::mat4_cast(orientation));

		glm::vec3 world_direction = position - GetRayDirection(mouse_x, mouse_y,
			screen_width, screen_height, view, projection) * 100.0f;

		btVector3 from(position.x, position.y, position.z), to(world_direction.x, world_direction.y, world_direction.z);
		this->last_rayfrom = from;
		btDynamicsWorld::AllHitsRayResultCallback ray_result_callback(from, to);
		this->dynamicsWorld->rayTest(from, to, ray_result_callback);
		if (ray_result_callback.hasHit()) {
			int collision_object_count = ray_result_callback.m_collisionObjects.size();
			double lastfrac = 1.1;
			int hit_entity_index = collision_object_count;
			eid hit_entity = 0;
			for (int i = 0; i < collision_object_count; i++) {
				eid entity = 0;
				double frc = ray_result_callback.m_hitFractions.at(i);
				const CollisionBody* colbody = (const CollisionBody*)ray_result_callback.m_collisionObjects.at(i)->getUserPointer();
				if (!colbody) {
					continue;
				}
				entity = colbody->entity_id;
				if (entity && entity != source_entity) {
					if (frc < lastfrac) {
						hit_entity = entity;
						hit_entity_index = i;
						lastfrac = frc;
					}
				}
			}
			if (hit_entity_index < collision_object_count) {
				this->last_raypos = ray_result_callback.m_hitPointWorld.at(hit_entity_index);
				this->last_raynorm = ray_result_callback.m_hitNormalWorld.at(hit_entity_index);
				this->last_raydist = last_rayfrom.distance(this->last_raypos);
				this->last_rayvalid = true;
				return hit_entity;
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
