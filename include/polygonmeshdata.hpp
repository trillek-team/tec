#pragma once

#include <vector>
#include <memory>
#include <map>
#include "vertexbuffer.hpp"

namespace vv {
	class PolygonMeshData;
	typedef Multiton<std::string, std::shared_ptr<PolygonMeshData>> PolygonMeshMap;

	class PolygonMeshData {
	public:
		// Add a single vertex to the vector, for the specified submesh.
		void AddVertex(Vertex v, size_t submesh = 0);

		// Used to batch copy a vector of vertices, for the specified submesh.
		void SetVerts(std::vector<Vertex>& verts, size_t submesh = 0);

		// Returns the vertex buffer, for specified submesh.
		const std::vector<Vertex>* GetVertexBuffer(size_t submesh = 0);

		// Add a single index to the vector, for the specified submesh.
		void AddIndex(unsigned int i, size_t submesh = 0);

		// Used to batch copy a vector of indices, for the specified submesh.
		void SetIndicies(std::vector<unsigned int>& indicies, size_t submesh = 0);

		// Returns the index buffer, for specified submesh.
		const std::vector<unsigned int>* GetIndexBuffer(size_t submesh = 0);

		// Creates a Material from the given shader MaterialMap under name.
		// return is a weak_ptr to the created Material.
		static std::weak_ptr<PolygonMeshData> Create(const std::string name);
	private:
		std::map<size_t, std::vector<Vertex>> verts;
		std::map<size_t, std::vector<unsigned int>> indicies;
	};
}
