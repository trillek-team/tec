#pragma once

#include <algorithm>

#include "graphics/renderable.hpp"
#include "resources/mesh.hpp"

namespace tec {
namespace manipulator {

const float MAX_DISTANCE = 25.0f; // Maximum distance at which to place.

/**
* \brief Manipulator used to place objects in the world. Can have a mesh assigned to provide visual feedback.
* 
*/
class Placement {
public:
	void SetRenderable(Renderable* _renderable) {
		this->renderable = _renderable;
	} // TODO: Change to retrieving renderable when setting the mesh

	/**
	* \brief Set the mesh used to represent the placement in the world.
	* 
	* \param[in] const std::shared_ptr<MeshFile> mesh MeshFile to render in the world.
	*/
	void SetMesh(const std::shared_ptr<MeshFile> mesh) {
		if (this->renderable) {
			this->ClearMesh();
			this->renderable->mesh = mesh;
		}
	}

	/**
	* \brief Clear the mesh, effectively not longer rendering the placement.
	* 
	*/
	void ClearMesh() {
		if (this->renderable->buffer) {
			this->renderable->buffer->Destroy();
			this->renderable->buffer.reset();
		}
		this->renderable->mesh.reset();
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
		auto distance = glm::distance(start, intersection);
		auto direction = glm::normalize(intersection - start);
		this->renderable->local_translation.value = start + direction * std::min<float>(distance, MAX_DISTANCE);
	}

private:
	Renderable* renderable{nullptr};
};
} // namespace manipulator
} // namespace tec
