#include "placement.hpp"

#include <algorithm>

#include "game.hpp"
#include "multiton.hpp"

#include "graphics/renderable.hpp"
#include "resources/mesh.hpp"

namespace tec {
using RenderableMap = Multiton<eid, Renderable*>;

Renderable* GetRenderable() { return RenderableMap::Get(ENGINE_ENTITIES::MANIPULATOR); }

namespace manipulator {

void Placement::SetMaxDistance(float _max_distance) { this->max_distance = _max_distance; }

void Placement::SetMesh(const std::shared_ptr<MeshFile> _mesh) {
	auto renderable = GetRenderable();
	if (renderable) {
		renderable->mesh = _mesh;
		this->mesh = _mesh;
	}
}

void Placement::SetMesh(const std::string mesh_name) {
	if (!MeshMap::Has(mesh_name)) {
		std::string ext = mesh_name.substr(mesh_name.find_last_of(".") + 1);
		if (file_factories.find(ext) != file_factories.end()) {
			file_factories[ext](mesh_name);
		}
	}
	SetMesh(MeshMap::Get(mesh_name));
}

void Placement::ClearMesh() {
	auto renderable = GetRenderable();
	if (renderable) {
		renderable->mesh.reset();
		this->mesh.reset();
	}
}

void Placement::SetRayIntersectionPoint(const glm::vec3 start, const glm::vec3 intersection) {
	auto renderable = GetRenderable();
	float distance = glm::distance(start, intersection);
	glm::vec3 direction = glm::normalize(intersection - start);
	renderable->local_translation = start + direction * std::min<float>(distance, this->max_distance);
}

void Placement::PlaceEntityInWorld(glm::vec3 _position) {
	if (this->mesh) {

		static eid starting_entity_id = 20000;
		proto::Entity entity;
		entity.set_id(starting_entity_id++);

		Renderable renderable;
		renderable.mesh_name = this->mesh->GetName();
		renderable.Out(entity.add_components());

		Position position(_position);
		position.Out(entity.add_components()->mutable_position());

		EventSystem<EntityCreated>::Get()->Emit(std::make_shared<EntityCreated>(EntityCreated{entity}));

		this->ClearMesh();
	}
}

} // namespace manipulator
} // namespace tec
