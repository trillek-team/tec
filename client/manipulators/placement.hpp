#pragma once

#include <algorithm>

#include "engine-entities.hpp"
#include "graphics/renderable.hpp"
#include "resources/mesh.hpp"

namespace tec {
using RenderableMap = Multiton<eid, Renderable*>;
namespace manipulator {

const float MAX_DISTANCE = 25.0f; // Maximum distance at which to place.

/**
* \brief Manipulator used to place objects in the world. Can have a mesh assigned to provide visual feedback.
* 
*/
class Placement {
public:
	/**
	* \brief Set the mesh used to represent the placement in the world.
	* 
	* \param[in] const std::shared_ptr<MeshFile> mesh MeshFile to render in the world.
	*/
	void SetMesh(const std::shared_ptr<MeshFile> _mesh) {
		auto renderable = GetRenderable();
		if (renderable) {
			this->ClearMesh();
			renderable->mesh = _mesh;
			this->mesh = _mesh;
		}
	}

	/**
	* \brief Clear the mesh, effectively not longer rendering the placement.
	* 
	*/
	void ClearMesh() {
		auto renderable = GetRenderable();
		if (renderable) {
			if (renderable->buffer) {
				renderable->buffer->Destroy();
				renderable->buffer.reset();
			}
			renderable->mesh.reset();
			this->mesh.reset();
		}
	}

	/**
	* \brief Sets the ray used to render the placement.
	* 
	* Sets the position in the world of the placement mesh based on the ray's start point and going to either
	* the intersection point or the point along the ray that is MAX_DISTANCE towards the intersection point.
	*
	* \param[in] const glm::vec3 start Starting point for the ray, usually the center of the screen.
	* \param[in] const glm::vec3 intersection Intersection point of the ray with the physics world.
	*/
	void SetRayIntersectionPoint(const glm::vec3 start, const glm::vec3 intersection) {
		auto renderable = GetRenderable();
		float distance = glm::distance(start, intersection);
		glm::vec3 direction = glm::normalize(intersection - start);
		renderable->local_translation.value = start + direction * std::min<float>(distance, MAX_DISTANCE);
	}

	void PlaceEntityInWorld(glm::vec3 _position) {
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

private:
	Renderable* GetRenderable() { return RenderableMap::Get(ENGINE_ENTITIES::MANIPULATOR); }
	std::shared_ptr<MeshFile> mesh;
};
} // namespace manipulator
} // namespace tec
