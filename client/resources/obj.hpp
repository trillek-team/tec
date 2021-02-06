#pragma once

#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "filesystem.hpp"
#include "resources/mesh.hpp"

namespace tec {
	class OBJ final : public MeshFile {
	public:
		// OBJ Helper struct for face vertex data indices.
		struct Face {
			unsigned int pos[3]{ 0,0,0 }; // index
			unsigned int uv[3]{ 0,0,0 }; // index
			unsigned int norm[3]{ 0,0,0 }; // index
		};

		// OBJ Helper struct for storing vertex groups
		struct OBJGroup {
			~OBJGroup() {
				for (FaceGroup* face_group : this->face_groups) {
					if (face_group) {
						delete face_group;
					}
				}
			}
			std::string name;
			struct FaceGroup {
				std::string mtl;
				std::vector<Face> faces;
			};
			std::vector<FaceGroup*> face_groups;
		};

		// MTL Helper struct describing mtl files
		struct MTL {
			glm::vec3 ka{ 0.f, 0.f, 0.f };
			glm::vec3 kd{ 0.f, 0.f, 0.f };
			glm::vec3 ks{ 0.f, 0.f, 0.f };
			float tr{ 0 };
			float hardness{ 0 };
			int illum{ 0 };
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
		std::vector<std::shared_ptr<OBJGroup>> vertexGroups;
		FilePath path; // Path to OBJ file
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::map<std::string, std::shared_ptr<MTL>> materials;
	};
}
