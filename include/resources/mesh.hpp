#pragma once

#include <glm/glm.hpp>
#include <list>
#include <vector>
#include <memory>
#include "multiton.hpp"

namespace tec {
	class Mesh;
	typedef Multiton<std::string, std::shared_ptr<Mesh>> MeshMap;

	// Vertex data used for rendering or other purposes.
	struct VertexData {
		VertexData() : position(0.0f, 0.0f, 0.0f), color(0.0f, 1.0f, 0.0f, 1.0f) { }
		VertexData(float x, float y, float z, float r, float g, float b) :
			position(x, y, z), color(r, g, b, 1.0f) { }
		glm::vec3 position;
		glm::vec4 color;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec4 bone_weights; // Used for animation.
		glm::uvec4 bone_indicies; // Used for animation.
	};

	// Container for holding sub-mesh groups.
	struct MeshGroup {
		std::vector<VertexData> verts;
		std::vector<unsigned int> indicies;
		std::list<std::string> textures;
	};

	class Mesh {
	public:
		Mesh() { }
		virtual ~Mesh() { }

		/**
		 * \brief Returns a sub-mesh group.
		 *
		 * \param[in] const unsigned int& meshGroup The index of the mesh group to retrieve.
		 * \return std::weak_ptr<MeshGroup> The mesh group requested
		 */
		std::weak_ptr<MeshGroup> GetMeshGroup(const size_t mesh_group);

		/**
		 * \brief Returns the number of sub-mesh groups.
		 *
		 * \return size_t The number of sub-mesh groups.
		 */
		size_t GetMeshGroupCount() const {
			return this->mesh_groups.size();
		}

		// Add a single vertex to the vector, for the specified mesh_group.
		void AddVertex(VertexData& v, size_t mesh_group = 0);

		// Used to batch copy a vector of vertices, for the specified mesh_group.
		void SetVerts(std::vector<VertexData>& verts, size_t mesh_group = 0);

		// Returns the vertex buffer, for specified mesh_group.
		const std::vector<VertexData>* GetVertexBuffer(size_t mesh_group = 0);

		// Add a single index to the vector, for the specified mesh_group.
		void AddIndex(unsigned int i, size_t mesh_group = 0);

		// Used to batch copy a vector of indices, for the specified mesh_group.
		void SetIndicies(std::vector<unsigned int>& indicies, size_t mesh_group = 0);

		// Returns the index buffer, for specified mesh_group.
		const std::vector<unsigned int>* GetIndexBuffer(size_t mesh_group = 0);
	protected:
		std::vector<std::shared_ptr<MeshGroup>> mesh_groups;
	};

}
