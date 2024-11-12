#pragma once

#include <memory>

#include "tec-types.hpp"
#include <glm/glm.hpp>

namespace tec {
class MeshFile;
namespace manipulator {

const float DEFAULT_MAX_PLACEMENT_DISTANCE = 25.0f;

/**
* \brief Manipulator used to place objects in the world. Can have a mesh assigned to provide visual feedback.
*
*/
class Placement {
public:
	void SetMaxDistance(float _max_distance = DEFAULT_MAX_PLACEMENT_DISTANCE);
	/**
	* \brief Set the mesh used to represent the placement in the world.
	*
	* \param[in] const std::shared_ptr<MeshFile> mesh MeshFile to render in the world.
	*/
	void SetMesh(const std::shared_ptr<MeshFile>&);

	/**
	* \brief Set the mesh used to represent the placement in the world.
	*
	* This version takes the mesh_name and locators the corresponding mesh_file and then
	* calls the appropriate SetMesh;
	*
	* \param[in] const std::string mesh_name Name of the mesh file to load.
	*/
	void SetMesh(const std::string&);

	/**
	* \brief Clear the mesh, effectively not longer rendering the placement.
	*
	*/
	void ClearMesh();

	/**
	* \brief Sets the ray used to render the placement.
	*
	* Sets the position in the world of the placement mesh based on the ray's start point and going to either
	* the intersection point or the point along the ray that is MAX_DISTANCE towards the intersection point.
	*
	* \param[in] const glm::vec3 start Starting point for the ray, usually the center of the screen.
	* \param[in] const glm::vec3 intersection Intersection point of the ray with the physics world.
	*/
	void SetRayIntersectionPoint(const glm::vec3, const glm::vec3);

	void PlaceEntityInWorld(glm::vec3);

	static void RegisterLuaType(sol::state&);

private:
	std::shared_ptr<MeshFile> mesh;

	float max_distance = DEFAULT_MAX_PLACEMENT_DISTANCE; // Maximum distance at which to place.
};
} // namespace manipulator
} // namespace tec
