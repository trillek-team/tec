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
	CollisionBody::CollisionBody(COLLISION_SHAPE collision_shape) : collision_shape(collision_shape), mass(0.0), shape(nullptr),
		disable_rotation(false), motion_state(new btDefaultMotionState(btTransform())) { }

	CollisionBody::~CollisionBody() {
		delete this->motion_state;
	}

	ReflectionComponent CollisionBody::Reflection(CollisionBody* val) {
		ReflectionComponent refcomp;
		switch (val->collision_shape) {
			case SPHERE:
				{
					//refcomp.properties["shape"] = "SPHERE";
					auto colspehre = static_cast<CollisionSphere*>(val);
					Property prop(Property::FLOAT);
					(refcomp.properties["radius"] = prop).Set<float>(colspehre->radius);
					refcomp.properties["radius"].update_func = [colspehre] (Property& prop) {
						colspehre->radius = prop.Get<float>();
						static_cast<btSphereShape*>(colspehre->shape.get())->setUnscaledRadius(colspehre->radius);
					};
				}
				break;
			case BOX:
				{
					//refcomp.properties["shape"] = "BOX";
					auto colbox = static_cast<CollisionBox*>(val);
					Property prop(Property::FLOAT);
					(refcomp.properties["extent_x"] = prop).Set<float>(colbox->half_extents.x());
					refcomp.properties["extent_x"].update_func = [colbox] (Property& prop) {
						colbox->half_extents.setX(prop.Get<float>());
						static_cast<btBoxShape*>(colbox->shape.get())->setImplicitShapeDimensions(colbox->half_extents);
					};
					(refcomp.properties["extent_y"] = prop).Set<float>(colbox->half_extents.y());
					refcomp.properties["extent_y"].update_func = [colbox] (Property& prop) {
						colbox->half_extents.setY(prop.Get<float>());
						static_cast<btBoxShape*>(colbox->shape.get())->setImplicitShapeDimensions(colbox->half_extents);
					};
					(refcomp.properties["extent_z"] = prop).Set<float>(colbox->half_extents.z());
					refcomp.properties["extent_z"].update_func = [colbox] (Property& prop) {
						colbox->half_extents.setZ(prop.Get<float>());
						static_cast<btBoxShape*>(colbox->shape.get())->setImplicitShapeDimensions(colbox->half_extents);
					};
				}
				break;
			case CAPSULE:
				{
					//refcomp.properties["shape"] = "CAPSULE";
					auto colcapsule = static_cast<CollisionCapsule*>(val);
					Property prop(Property::FLOAT);
					(refcomp.properties["radius"] = prop).Set<float>(colcapsule->radius);
					refcomp.properties["radius"].update_func = [colcapsule] (Property& prop) {
						colcapsule->radius = prop.Get<float>();
						static_cast<btCapsuleShape*>(colcapsule->shape.get())->setImplicitShapeDimensions(
							btVector3(colcapsule->radius, 0.5f * colcapsule->height, colcapsule->radius));
					};
					(refcomp.properties["height"] = prop).Set<float>(colcapsule->height);
					refcomp.properties["height"].update_func = [colcapsule] (Property& prop) {
						colcapsule->height = prop.Get<float>();
						static_cast<btCapsuleShape*>(colcapsule->shape.get())->setImplicitShapeDimensions(
							btVector3(colcapsule->radius, 0.5f * colcapsule->height, colcapsule->radius));
					};

				}
				break;
		}
		Property prop(Property::BOOLEAN);
		(refcomp.properties["Disable Deactivation"] = prop).Set<bool>(val->disable_deactivation);
		refcomp.properties["Disable Deactivation"].update_func = [val] (Property& prop) { val->disable_deactivation = prop.Get<bool>(); };
		(refcomp.properties["Disable Rotation"] = prop).Set<bool>(val->disable_rotation);
		refcomp.properties["Disable Rotation"].update_func = [val] (Property& prop) { val->disable_rotation = prop.Get<bool>(); };
		Property mass_prop(Property::FLOAT);
		(refcomp.properties["Mass"] = mass_prop).Set<float>(val->mass);
		refcomp.properties["Mass"].update_func = [val] (Property& prop) { val->mass = prop.Get<float>(); };
		return std::move(refcomp);
	}

	CollisionMesh::CollisionMesh(std::shared_ptr<Mesh> mesh, bool dynamic) : CollisionBody((dynamic ? DYNAMIC_MESH : STATIC_MESH)), mesh_file(mesh) {
		this->mesh_file = mesh;
		this->mesh = GenerateTriangleMesh(this->mesh_file);
		if (!this->mesh) {
			return;
		}
		glm::vec3 entity_scale(1.0);
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
