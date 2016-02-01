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

	std::set<eid> PhysicsSystem::Update(const double delta, const GameState& state) {
		std::set<eid> updated_entities;
		ProcessCommandQueue();
		EventQueue<MouseBtnEvent>::ProcessEventQueue();
		EventQueue<EntityCreated>::ProcessEventQueue();
		EventQueue<EntityDestroyed>::ProcessEventQueue();

		for (auto itr = collidables.begin(); itr != collidables.end(); ++itr) {
			eid entity_id = itr->first;
			CollisionBody& collidable = itr->second;
			glm::vec3 position(0.0);
			if (state.positions.find(entity_id) != state.positions.end()) {
				auto pos = state.positions.at(entity_id).value;
				if (std::isfinite(pos.x) && std::isfinite(pos.x) && std::isfinite(pos.x)) {
					position = pos;
				}
			}
			glm::quat orientation;
			if (state.orientations.find(entity_id) != state.orientations.end()) {
				auto ori = state.orientations.at(entity_id).value;
				if (std::isfinite(ori.x) && std::isfinite(ori.x) && std::isfinite(ori.x) && std::isfinite(ori.w)) {
					orientation = ori;
				}
			}
			btTransform transform(
				btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w),
				btVector3(position.x, position.y, position.z));

			collidable.motion_state.transform = transform;
			if (collidable.new_collision_shape != collidable.collision_shape) {
				if (!UpdateCollisionShape(entity_id)) {
					continue;
				}
			}
			btRigidBody* body = this->bodies[entity_id];
			this->dynamicsWorld->removeRigidBody(body);
			if (collidable.mass != body->getInvMass()) {
				btVector3 fallInertia(0, 0, 0);
				collidable.shape->calculateLocalInertia(collidable.mass, fallInertia);
				body->setMassProps(collidable.mass, fallInertia);
				body->updateInertiaTensor();
				body->clearForces();
			}

			int state = body->getActivationState();
			if (collidable.disable_deactivation) {
				body->forceActivationState(DISABLE_DEACTIVATION);
			}
			else if (state == DISABLE_DEACTIVATION) {
				body->forceActivationState(ACTIVE_TAG);
			}

			if (collidable.disable_rotation) {
				body->setAngularFactor(btVector3(0.0, 0, 0.0));
			}
			body->setWorldTransform(collidable.motion_state.transform);
			this->dynamicsWorld->addRigidBody(body);
		}

		for (auto itr = VelocityMap::Begin(); itr != VelocityMap::End(); ++itr) {
			auto entity_id = itr->first;
			if (this->bodies.find(entity_id) != this->bodies.end()) {
				auto body = this->bodies.at(entity_id);
				if (state.velocties.find(entity_id) != state.velocties.end()) {
					const Velocity& vel = state.velocties.at(entity_id);
					if (std::isfinite(vel.linear.x) && std::isfinite(vel.linear.x) && std::isfinite(vel.linear.x)) {
						body->setLinearVelocity(vel.GetLinear() + body->getGravity());
					}
					if (std::isfinite(vel.angular.x) && std::isfinite(vel.angular.x) && std::isfinite(vel.angular.x)) {
						body->setAngularVelocity(vel.GetAngular());
					}
				}
			}
		}

		this->dynamicsWorld->stepSimulation(delta, 10);

		for (auto itr = collidables.begin(); itr != collidables.end(); ++itr) {
			auto entity_id = itr->first;
			if (itr->second.motion_state.transform_updated) {
				updated_entities.insert(entity_id);
				itr->second.motion_state.transform_updated = false;
			}
		}
		return std::move(updated_entities);
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

	eid PhysicsSystem::RayCastMousePick(eid source_entity, double mouse_x, double mouse_y, float screen_width, float screen_height) {
		if (source_entity == 0) {
			return 0;
		}
		this->last_rayvalid = false;
		this->last_entity_hit = 0;
		glm::vec3 position;
		if (Entity(source_entity).Has<Position>()) {
			position = (Entity(source_entity).Get<Position>().lock())->value;
		}
		glm::quat orientation;
		if (Entity(source_entity).Has<Orientation>()) {
			orientation = (Entity(source_entity).Get<Orientation>().lock())->value;
		}

		if (screen_height == 0.0f) {
			return 0;
		}
		// TODO: This could be pulled from something but it seems unlikely to change.
		static glm::mat4 projection = glm::perspective(
			glm::radians(45.0f),
			screen_width / screen_height,
			-1.0f,
			300.0f
			);
		glm::mat4 view = glm::inverse(glm::translate(glm::mat4(1.0), position) * glm::mat4_cast(orientation));

		glm::vec3 world_direction = position - GetRayDirection(static_cast<float>(mouse_x),
			static_cast<float>(mouse_y), screen_width, screen_height, view, projection) * 100.0f;

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
			this->last_entity_hit = hit_entity;
			if (hit_entity_index < collision_object_count) {
				this->last_raypos = ray_result_callback.m_hitPointWorld.at(hit_entity_index);
				this->last_raynorm = ray_result_callback.m_hitNormalWorld.at(hit_entity_index);
				this->last_raydist = last_rayfrom.distance(this->last_raypos);
				this->last_rayvalid = true;
			}
		}
		return this->last_entity_hit;
	}

	eid PhysicsSystem::RayCastIgnore(eid ign) {
		eid cam = 1; // TODO: This hard-coded number should be the active camera id.
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

	bool PhysicsSystem::CreateRigiedBody(eid entity_id, CollisionBody&& collidable) {
		// If it already exists erase it before creating a new instance.
		if (this->collidables.find(entity_id) != this->collidables.end()) {
			if (this->bodies.find(entity_id) != this->bodies.end()) {
				this->dynamicsWorld->removeRigidBody(this->bodies.at(entity_id));
				delete this->bodies.at(entity_id);
				this->bodies.erase(entity_id);
			}
			this->collidables.erase(entity_id);
		}
		collidable.entity_id = entity_id;
		this->collidables.emplace(entity_id, std::move(collidable));

		return UpdateCollisionShape(entity_id);
	}

	bool PhysicsSystem::UpdateCollisionShape(eid entity_id) {
		CollisionBody& collidable = this->collidables.at(entity_id);
		if (collidable.collision_shape != collidable.new_collision_shape) {
			if (this->bodies.find(entity_id) != this->bodies.end()) {
				this->dynamicsWorld->removeRigidBody(this->bodies.at(entity_id));
				delete this->bodies.at(entity_id);
			}

			switch (collidable.new_collision_shape) {
				case SPHERE:
					collidable.shape = std::make_shared<btSphereShape>(collidable.radius);
					break;
				case BOX:
					collidable.shape = std::make_shared<btBoxShape>(collidable.half_extents);
					break;
				case CAPSULE:
					collidable.shape = std::make_shared<btCapsuleShape>(collidable.radius, collidable.height);
					break;
			}
			collidable.collision_shape = collidable.new_collision_shape;
			btVector3 fallInertia(0, 0, 0);
			if (collidable.mass > 0.0) {
				if (collidable.shape) {
					collidable.shape->calculateLocalInertia(collidable.mass, fallInertia);
				}
			}
			btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(collidable.mass,
				&collidable.motion_state, collidable.shape.get(), fallInertia);
			auto body = new btRigidBody(fallRigidBodyCI);

			if (!body) {
				return false;
			}

			this->bodies[entity_id] = body;

			body->setUserPointer(&collidable);
			if (collidable.disable_deactivation) {
				body->forceActivationState(DISABLE_DEACTIVATION);
			}

			if (collidable.disable_rotation) {
				body->setAngularFactor(btVector3(0.0, 0, 0.0));
			}
		}
		return true;
	}

	void PhysicsSystem::On(std::shared_ptr<MouseBtnEvent> data) {
		if (data->action == MouseBtnEvent::DOWN) {
			if (this->last_entity_hit) {
				std::shared_ptr<MouseClickEvent> mce_event = std::make_shared<MouseClickEvent>();
				mce_event->button = data->button;
				mce_event->ray_distance = this->last_raydist;
				mce_event->ray_hit_piont_world = glm::vec3(this->last_raypos.getX(),
					this->last_raypos.getY(), this->last_raypos.getZ());
				EventSystem<MouseClickEvent>::Get()->Emit(this->last_entity_hit, mce_event);
			}
		}
	}


	void PhysicsSystem::On(std::shared_ptr<EntityCreated> data) {
		for (int i = 0; i < data->entity.components_size(); ++i) {
			const proto::Component& comp = data->entity.components(i);
			switch (comp.component_case()) {
				case proto::Component::kCollisionBody:
					CollisionBody colbody;
					colbody.In(comp);
					CreateRigiedBody(data->entity.id(), std::move(colbody));
					break;
			}
		}
	}

	void PhysicsSystem::On(std::shared_ptr<EntityDestroyed> data) {
		if (this->collidables.find(data->entity_id) != this->collidables.end()) {
			if (this->bodies.find(data->entity_id) != this->bodies.end()) {
				this->dynamicsWorld->removeRigidBody(this->bodies.at(data->entity_id));
				delete this->bodies.at(data->entity_id);
				this->bodies.erase(data->entity_id);
			}
			this->collidables.erase(data->entity_id);
		}
	}

	Position PhysicsSystem::GetPosition(eid entity_id) {
		auto pos = collidables.at(entity_id).motion_state.transform.getOrigin();
		Position position(glm::vec3(pos.x(), pos.y(), pos.z()));

		// TODO: remove this once center_offset is in renderable
		if (Entity(entity_id).Has<Position>()) {
			position.center_offset = Entity(entity_id).Get<Position>().lock()->center_offset;
		}
		return std::move(position);
	}

	Orientation PhysicsSystem::GetOrientation(eid entity_id) {
		auto rot = collidables.at(entity_id).motion_state.transform.getRotation();
		Orientation orientation(glm::quat(rot.w(), rot.x(), rot.y(), rot.z()));

		// TODO: remove this once rotation_offset is in renderable
		if (Entity(entity_id).Has<Orientation>()) {
			orientation.rotation_offset = Entity(entity_id).Get<Orientation>().lock()->rotation_offset;
		}
		return std::move(orientation);
	}
}
