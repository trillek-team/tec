#include "resources/mesh.hpp"

namespace tec {
	std::weak_ptr<MeshGroup> Mesh::GetMeshGroup(const size_t mesh_group) {
		if (mesh_group < this->mesh_groups.size()) {
			return this->mesh_groups[mesh_group];
		}

		return std::weak_ptr<MeshGroup>();
	}

	void Mesh::AddVertex(VertexData& v, size_t mesh_group) {
		if (mesh_group >= this->mesh_groups.size()) {
			this->mesh_groups.push_back(std::make_shared<MeshGroup>());
		}
		this->mesh_groups[mesh_group]->verts.push_back(v);
	}

	void Mesh::SetVerts(std::vector<VertexData>& verts, size_t mesh_group) {
		if (mesh_group >= this->mesh_groups.size()) {
			this->mesh_groups.push_back(std::make_shared<MeshGroup>());
		}
		this->mesh_groups[mesh_group]->verts = verts;
	}

	// Returns the vertex buffer for specified mesh_group.
	const std::vector<VertexData>* Mesh::GetVertexBuffer(size_t mesh_group) {
		if (mesh_group < this->mesh_groups.size()) {
			return &this->mesh_groups[mesh_group]->verts;
		}
		return nullptr;
	}

	void Mesh::AddIndex(unsigned int i, size_t mesh_group) {
		if (mesh_group >= this->mesh_groups.size()) {
			this->mesh_groups.push_back(std::make_shared<MeshGroup>());
		}
		this->mesh_groups[mesh_group]->indicies.push_back(i);
	}

	void Mesh::SetIndicies(std::vector<unsigned int>& indicies, size_t mesh_group) {
		if (mesh_group >= this->mesh_groups.size()) {
			this->mesh_groups.push_back(std::make_shared<MeshGroup>());
		}
		this->mesh_groups[mesh_group]->indicies = indicies;
	}

	// Returns the index buffer for specified mesh_group.
	const std::vector<unsigned int>* Mesh::GetIndexBuffer(size_t mesh_group) {
		if (mesh_group < this->mesh_groups.size()) {
			return &this->mesh_groups[mesh_group]->indicies;
		}
		return nullptr;
	}
}
