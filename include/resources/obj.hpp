#pragma once

#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "filesystem.hpp"
#include "mesh.hpp"

namespace tec {
	class OBJ final : public Mesh {
	public:
		OBJ() { }
		~OBJ() { }

		// OBJ Helper struct for face vertex data indicies.
		struct Face {
			Face() {
				this->pos[0] = 0; this->pos[1] = 0; this->pos[2] = 0;
				this->uv[0] = 0; this->uv[1] = 0; this->uv[2] = 0;
				this->norm[0] = 0; this->norm[1] = 0; this->norm[2] = 0;
			}
			unsigned int pos[3]; // index
			unsigned int uv[3]; // index
			unsigned int norm[3]; // index
		};

		// OBJ Helper struct for storing vertex groups
		struct VertexGroup {
			std::string name;
			std::string mtl;
			std::vector<Face> faces;
		};

		// MTL Helper struct describing mtl files
		struct MTL {
			glm::vec3 ka;
			glm::vec3 kd;
			glm::vec3 ks;
			float tr;
			float hardness;
			int illum;
			std::string ambientMap;
			std::string diffuseMap;
			std::string specularMap;
			std::string normalMap;
		};

		/**
		 * \brief Returns a resource with the specified name.
		 *
		 * The only used initialization property is "filename".
		 * \param[in] const std::vector<Property>& properties The creation properties for the resource.
		 * \return std::shared_ptr<OBJ> The created OBJ resource.
		 */
		static std::shared_ptr<OBJ> Create(const FilePath& fname);

		/**
		 * \brief Loads the OBJ file from disk and parses it.
		 *
		 * \return bool If the mesh was valid and loaded correctly.
		 */
		bool Parse();

		/**
		 * \brief Loads the MTL file from disk and parses it.
		 *
		 * \return bool If the material was valid and loaded correctly.
		 */
		bool ParseMTL(const FilePath& fname);

		/**
		 * \brief Calculates the final vertex positions based on the bind-pose skeleton.
		 *
		 * There isn't a return as the processing will just do nothing if the
		 * parse data was default objects.
		 * \return void
		 */
		void PopulateMeshGroups();

		/**
		 * \brief Sets the mesh filename.
		 *
		 * This is just a shorthand function that can be called directly via script API.
		 * \param[in] const std::string& fname The mesh filename.
		 * \return bool True if initialization finished with no errors.
		 */
		void SetFileName(const FilePath& fname) {
			this->path = fname;
		}
	private:
		FilePath path; // Path to OBJ file
		std::vector<std::shared_ptr<VertexGroup>> vertexGroups;

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::map<std::string, std::shared_ptr<MTL>> materials;
	};
}
