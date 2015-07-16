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
	CollisionBody::CollisionBody(eid entity_id, COLLISION_SHAPE collision_shape) :
		entity_id(entity_id), collision_shape(collision_shape), mass(1.0), shape(nullptr),
		disable_rotation(false), motion_state(new btDefaultMotionState(btTransform())) { }

	CollisionBody::~CollisionBody() {
		delete this->motion_state;
	}
	ReflectionComponent CollisionBody::Reflection(CollisionBody* val) {
		ReflectionComponent refcomp;
		switch (val->collision_shape) {
			case SPHERE:
				{
					refcomp.properties["shape"] = "SPHERE";
					auto colspehre = static_cast<CollisionSphere*>(val);
					refcomp.properties["radius"] = std::to_string(colspehre->radius);
				}
				break;
			case BOX:
				{
					refcomp.properties["shape"] = "BOX";
					auto colbox = static_cast<CollisionBox*>(val);
					refcomp.properties["extent_x"] = std::to_string(colbox->half_extents.x());
					refcomp.properties["extent_y"] = std::to_string(colbox->half_extents.y());
					refcomp.properties["extent_z"] = std::to_string(colbox->half_extents.z());
				}
				break;
			case CAPSULE:
				{
					refcomp.properties["shape"] = "CAPSULE";
					auto colcapsule = static_cast<CollisionCapsule*>(val);
					refcomp.properties["radius"] = std::to_string(colcapsule->radius);
					refcomp.properties["height"] = std::to_string(colcapsule->height);
				}
				break;
		}
		refcomp.properties["disable_deactivation"] = std::to_string(val->disable_deactivation);
		refcomp.properties["disable_rotation"] = std::to_string(val->disable_rotation);
		return std::move(refcomp);
	}

	CollisionMesh::CollisionMesh(eid entity_id, std::shared_ptr<Mesh> mesh, bool dynamic) :
		CollisionBody(entity_id, (dynamic ? DYNAMIC_MESH : STATIC_MESH)), mesh_file(mesh) {
		this->mesh_file = mesh;
		this->mesh = GenerateTriangleMesh(this->mesh_file);
		if (!this->mesh) {
			return;
		}
		glm::vec3 entity_scale(1.0);
		if (Entity(this->entity_id).Has<Scale>()) {
			entity_scale = Entity(this->entity_id).Get<Scale>().lock()->value;
		}
		switch (this->collision_shape) {
			case STATIC_MESH:
				{
					auto mesh_shape = std::make_shared<btBvhTriangleMeshShape>(this->mesh.get(), true);
					mesh_shape->setLocalScaling(btVector3(entity_scale.x, entity_scale.y, entity_scale.z));
					this->shape = mesh_shape;
				}

				// Static BvhTriangleMehes must have a mass of 0.
				this->mass = 0;
				break;
			case DYNAMIC_MESH:
				{
					auto mesh_shape = std::make_shared<btGImpactMeshShape>(this->mesh.get());
					mesh_shape->setLocalScaling(btVector3(entity_scale.x, entity_scale.y, entity_scale.z));
					mesh_shape->updateBound();
					this->shape = mesh_shape;
				}
				break;
		}
	}
}
