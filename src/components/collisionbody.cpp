#include "components/collisionbody.hpp"
#include "physics-system.hpp"
#include "resources/mesh.hpp"
#include "entity.hpp"
#include "components/transforms.hpp"

#include <BulletCollision/Gimpact/btGImpactShape.h>

namespace tec {
	std::unique_ptr<btTriangleMesh> GenerateTriangleMesh(std::shared_ptr<MeshFile> mesh_file) {
		auto btmesh = std::unique_ptr<btTriangleMesh>(new btTriangleMesh());
		if (!mesh_file) {
			return nullptr;
		}
		for (std::size_t mesh_i = 0; mesh_i < mesh_file->GetMeshCount(); ++mesh_i) {
			Mesh* mesh = mesh_file->GetMesh(mesh_i);
			for (ObjectGroup* objgroup : mesh->object_groups) {
				btmesh->preallocateVertices(mesh->verts.size());
				btmesh->preallocateIndices(objgroup->indices.size());
				for (std::size_t face_i = 0; face_i < objgroup->indices.size(); ++face_i) {
					const VertexData& v1 = mesh->verts[objgroup->indices[face_i]];
					const VertexData& v2 = mesh->verts[objgroup->indices[++face_i]];
					const VertexData& v3 = mesh->verts[objgroup->indices[++face_i]];
					btmesh->addTriangle(
						btVector3(v1.position.x, v1.position.y, v1.position.z),
						btVector3(v2.position.x, v2.position.y, v2.position.z),
						btVector3(v3.position.x, v3.position.y, v3.position.z), false);
				}
			}
		}

		return std::move(btmesh);
	}
	CollisionBody::CollisionBody(COLLISION_SHAPE collision_shape) : collision_shape(collision_shape),
		new_collision_shape(collision_shape), mass(0.0), radius(1.0f), height(1.0f), disable_deactivation(false),
		disable_rotation(false), half_extents(btVector3(1.0, 1.0, 1.0)) {
		motion_state.transform_updated = true;
	}

	CollisionBody::~CollisionBody() { }

	void CollisionBody::Out(proto::Component* target) {
		proto::CollisionBody* comp = target->mutable_collision_body();
		comp->set_disable_deactivation(this->disable_deactivation);
		comp->set_disable_rotation(this->disable_rotation);
		comp->set_mass(static_cast<float>(this->mass));
		switch (this->new_collision_shape) {
			case COLLISION_SHAPE::BOX:
				{
					proto::CollisionBody::Box* box = comp->mutable_box();
					box->set_x_extent(static_cast<float>(this->half_extents.x()));
					box->set_y_extent(static_cast<float>(this->half_extents.y()));
					box->set_z_extent(static_cast<float>(this->half_extents.z()));
				}
				break;
			case COLLISION_SHAPE::SPHERE:
				{
					proto::CollisionBody::Sphere* sphere = comp->mutable_sphere();
					sphere->set_radius(this->radius);
				}
				break;
			case COLLISION_SHAPE::CAPSULE:
				{
					proto::CollisionBody::Capsule* capsule = comp->mutable_capsule();
					capsule->set_radius(this->radius);
					capsule->set_height(this->height);
				}
				break;
		}
	}

	void CollisionBody::In(const proto::Component& source) {
		const proto::CollisionBody& comp = source.collision_body();
		switch (comp.shape_case()) {
			case proto::CollisionBody::ShapeCase::kBox:
				this->new_collision_shape = BOX;
				this->half_extents = btVector3(comp.box().x_extent(), comp.box().y_extent(), comp.box().z_extent());
				break;
			case proto::CollisionBody::ShapeCase::kSphere:
				this->new_collision_shape = SPHERE;
				this->radius = comp.sphere().radius();
				break;
			case proto::CollisionBody::ShapeCase::kCapsule:
				this->new_collision_shape = CAPSULE;
				this->radius = comp.capsule().radius();
				this->height = comp.capsule().height();
				break;

		}

		if (comp.has_disable_deactivation()) {
			this->disable_deactivation = comp.disable_deactivation();
		}
		if (comp.has_disable_rotation()) {
			this->disable_rotation = comp.disable_rotation();
		}
		if (comp.has_mass()) {
			this->mass = comp.mass();
		}
	}

	ReflectionComponent CollisionBody::Reflection(CollisionBody* val) {
		ReflectionComponent refcomp;
		Property fprop(Property::FLOAT);
		(refcomp.properties["Mass"] = fprop).Set<float>(static_cast<float>(val->mass));
		refcomp.properties["Mass"].update_func = [val] (Property& prop) { val->mass = prop.Get<float>(); };
		static std::vector<std::string> choices = {"BOX", "SPHERE", "CAPSULE"};
		std::string current_shape;
		switch (val->collision_shape) {
			case SPHERE:
				current_shape = "SPHERE";
				(refcomp.properties["radius"] = fprop).Set<float>(val->radius);
				refcomp.properties["radius"].update_func = [val] (Property& prop) {
					val->radius = prop.Get<float>();
					static_cast<btSphereShape*>(val->shape.get())->setUnscaledRadius(val->radius);
				};
				break;
			case BOX:
				current_shape = "BOX";
				(refcomp.properties["extent_x"] = fprop).Set<float>(static_cast<float>(val->half_extents.x()));
				refcomp.properties["extent_x"].update_func = [val] (Property& prop) {
					val->half_extents.setX(prop.Get<float>());
					static_cast<btBoxShape*>(val->shape.get())->setImplicitShapeDimensions(val->half_extents);
				};
				(refcomp.properties["extent_y"] = fprop).Set<float>(static_cast<float>(val->half_extents.y()));
				refcomp.properties["extent_y"].update_func = [val] (Property& prop) {
					val->half_extents.setY(prop.Get<float>());
					static_cast<btBoxShape*>(val->shape.get())->setImplicitShapeDimensions(val->half_extents);
				};
				(refcomp.properties["extent_z"] = fprop).Set<float>(static_cast<float>(val->half_extents.z()));
				refcomp.properties["extent_z"].update_func = [val] (Property& prop) {
					val->half_extents.setZ(prop.Get<float>());
					static_cast<btBoxShape*>(val->shape.get())->setImplicitShapeDimensions(val->half_extents);
				};
				break;
			case CAPSULE:
				current_shape = "CAPSULE";
				(refcomp.properties["radius"] = fprop).Set<float>(val->radius);
				refcomp.properties["radius"].update_func = [val] (Property& prop) {
					val->radius = prop.Get<float>();
					static_cast<btCapsuleShape*>(val->shape.get())->setImplicitShapeDimensions(
						btVector3(val->radius, 0.5f * val->height, val->radius));
				};
				(refcomp.properties["height"] = fprop).Set<float>(val->height);
				refcomp.properties["height"].update_func = [val] (Property& prop) {
					val->height = prop.Get<float>();
					static_cast<btCapsuleShape*>(val->shape.get())->setImplicitShapeDimensions(
						btVector3(val->radius, 0.5f * val->height, val->radius));
				};
				break;
		}
		radio_t shape_choices = std::make_pair(std::ref(choices), current_shape);
		Property rprop(Property::RADIO);
		(refcomp.properties["Shape"] = rprop).Set<radio_t>(shape_choices);
		refcomp.properties["Shape"].update_func = [val] (Property& prop) {
			radio_t shape_choices = prop.Get<radio_t>();
			if (shape_choices.second == "BOX") {
				val->new_collision_shape = BOX;
			}
			else if (shape_choices.second == "SPHERE") {
				val->new_collision_shape = SPHERE;
			}
			else if (shape_choices.second == "CAPSULE") {
				val->new_collision_shape = CAPSULE;
			}
		};
		Property prop(Property::BOOLEAN);
		(refcomp.properties["Disable Deactivation"] = prop).Set<bool>(val->disable_deactivation);
		refcomp.properties["Disable Deactivation"].update_func = [val] (Property& prop) { val->disable_deactivation = prop.Get<bool>(); };
		(refcomp.properties["Disable Rotation"] = prop).Set<bool>(val->disable_rotation);
		refcomp.properties["Disable Rotation"].update_func = [val] (Property& prop) { val->disable_rotation = prop.Get<bool>(); };
		return std::move(refcomp);
	}

	CollisionMesh::CollisionMesh(std::shared_ptr<MeshFile> mesh, bool dynamic) : CollisionBody((dynamic ? DYNAMIC_MESH : STATIC_MESH)), mesh_file(mesh) {
		if (!mesh) {
			return;
		}
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
