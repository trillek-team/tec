#include "components/collisionbody.hpp"
#include "physics-system.hpp"
#include "resources/mesh.hpp"
#include "entity.hpp"
#include "components/transforms.hpp"

#include <BulletCollision/Gimpact/btGImpactShape.h>

namespace tec {
	std::unique_ptr<btTriangleMesh> GenerateTriangleMesh(std::shared_ptr<Mesh> mesh_file) {
		auto mesh = std::unique_ptr<btTriangleMesh>(new btTriangleMesh());
		if (!mesh_file) {
			return nullptr;
		}
		for (size_t mesh_i = 0; mesh_i < mesh_file->GetMeshGroupCount(); ++mesh_i) {
			const auto& mesh_group = mesh_file->GetMeshGroup(mesh_i);
			const auto& temp_lock = mesh_group.lock();
			for (size_t face_i = 0; face_i < temp_lock->indicies.size(); ++face_i) {
				const VertexData& v1 = temp_lock->verts[temp_lock->indicies[face_i]];
				const VertexData& v2 = temp_lock->verts[temp_lock->indicies[++face_i]];
				const VertexData& v3 = temp_lock->verts[temp_lock->indicies[++face_i]];
				mesh->addTriangle(
					btVector3(v1.position.x, v1.position.y, v1.position.z),
					btVector3(v2.position.x, v2.position.y, v2.position.z),
					btVector3(v3.position.x, v3.position.y, v3.position.z), true);
			}
		}

		return std::move(mesh);
	}

	CollisionBody::CollisionBody(eid entity_id, COLLISION_SHAPE collision_shape, double radius,
		double height) : entity_id(entity_id), radius(radius), height(height),
		collision_shape(collision_shape), mass(1.0), disable_rotation(false),
		motion_state(new btDefaultMotionState(btTransform())) {
		switch (collision_shape) {
			case SPHERE:
				this->shape = std::move(std::unique_ptr<btCollisionShape>(new btSphereShape(this->radius)));
				break;
			case CAPSULE:
				this->shape = std::move(std::unique_ptr<btCollisionShape>(new btCapsuleShape(this->radius, this->height)));
				break;
			case STATIC_MESH:
			case DYNAMIC_MESH:
			default:
				this->shape = nullptr;
				break;
		}
	}

	CollisionBody::~CollisionBody() {
		delete this->motion_state;
	}

	bool CollisionBody::SetMesh(std::shared_ptr<Mesh> mesh) {
		this->mesh_file = mesh;
		this->mesh = GenerateTriangleMesh(this->mesh_file);
		if (!this->mesh) {
			return false;
		}
		glm::vec3 entity_scale(1.0);
		if (Entity(this->entity_id).Has<Scale>()) {
			entity_scale = Entity(this->entity_id).Get<Scale>().lock()->value;
		}
		switch (this->collision_shape) {
			case STATIC_MESH:
				{
					auto mesh_shape = std::unique_ptr<btBvhTriangleMeshShape>(
						new btBvhTriangleMeshShape(this->mesh.get(), true));
					mesh_shape->setLocalScaling(btVector3(entity_scale.x, entity_scale.y, entity_scale.z));
					this->shape = std::move(mesh_shape);
				}

				// Static BvhTriangleMehes must have a mass of 0.
				this->mass = 0;
				break;
			case DYNAMIC_MESH:
				{
					auto mesh_shape = std::unique_ptr<btGImpactMeshShape>(new btGImpactMeshShape(this->mesh.get()));
					mesh_shape->setLocalScaling(btVector3(entity_scale.x, entity_scale.y, entity_scale.z));
					mesh_shape->updateBound();
					this->shape = std::move(mesh_shape);
				}
				break;
			default:
				this->shape = nullptr;
				return false;
		}
		return true;
	}

	bool CollisionBody::SetMesh(const std::string mesh_name) {
		auto mesh = MeshMap::Get(mesh_name);
		return SetMesh(mesh);
	}
}
