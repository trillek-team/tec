#include "polygonmeshdata.hpp"

namespace vv {
	void PolygonMeshData::AddVertex(Vertex v, size_t submesh) {
		this->verts[submesh].push_back(v);
	}

	void PolygonMeshData::SetVerts(std::vector<Vertex>& verts, size_t submesh) {
		this->verts[submesh] = verts;
	}

	// Returns the vertex buffer for specified submesh.
	const std::vector<Vertex>* PolygonMeshData::GetVertexBuffer(size_t submesh) {
		if (this->verts.find(submesh) != this->verts.end()) {
			return &this->verts[submesh];
		}
		return nullptr;
	}

	void PolygonMeshData::AddIndex(unsigned int i, size_t submesh) {
		this->indicies[submesh].push_back(i);
	}

	void PolygonMeshData::SetIndicies(std::vector<unsigned int>& indicies, size_t submesh) {
		this->indicies[submesh] = indicies;
	}

	// Returns the index buffer for specified submesh.
	const std::vector<unsigned int>* PolygonMeshData::GetIndexBuffer(size_t submesh) {
		if (this->indicies.find(submesh) != this->indicies.end()) {
			return &this->indicies[submesh];
		}
		return nullptr;
	}

	// Creates a Material from the given shader MaterialMap under name.
	// return is a weak_ptr to the created Material.
	std::weak_ptr<PolygonMeshData> PolygonMeshData::Create(const std::string name) {
		auto mesh = std::make_shared<PolygonMeshData>();
		PolygonMeshMap::Set(name, mesh);
		return mesh;
	}
}
