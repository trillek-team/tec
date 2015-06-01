#include "graphics/vertex-buffer-object.hpp"

#include "resources/mesh.hpp"

namespace tec {
	VertexBufferObject::VertexBufferObject() : vao(0), vbo(0), ibo(0), vertex_count(0), index_count(0) { }

	VertexBufferObject::VertexBufferObject(std::shared_ptr<Mesh> mesh) :
		vao(0), vbo(0), ibo(0), vertex_count(0), index_count(0), source_mesh(mesh) {
		Load(mesh);
	}

	VertexBufferObject::~VertexBufferObject() {
		Destroy();
	}

	void VertexBufferObject::Destroy() {
		glDeleteBuffers(1, &this->vbo);
		glDeleteBuffers(1, &this->vao);
		glDeleteBuffers(1, &this->ibo);
	}

	GLuint VertexBufferObject::GetVAO() { return this->vao; }

	GLuint VertexBufferObject::GetIBO() { return this->ibo; }

	VertexBufferObject::VertexGroup* VertexBufferObject::GetVertexGroup(const size_t vertex_group_number) {
		if (vertex_group_number < this->vertex_groups.size()) {
			return &this->vertex_groups[vertex_group_number];
		}
		return nullptr;
	}

	size_t VertexBufferObject::GetVertexGroupCount() const {
		return this->vertex_groups.size();
	}

	bool VertexBufferObject::IsDynamic() { return !this->source_mesh.expired(); }

	void VertexBufferObject::Update() {
		std::shared_ptr<Mesh> locked_ptr = this->source_mesh.lock();
		if (locked_ptr) {
			Load(*locked_ptr->GetVertexBuffer(), *locked_ptr->GetIndexBuffer());
		}
	}

	void VertexBufferObject::Load(std::shared_ptr<Mesh> mesh) {
		if (mesh) {
			this->source_mesh = mesh;
			// TODO: Make a load method that takes offset and count to sub_buffer.
			size_t vertex_goup_count = mesh->GetMeshGroupCount();
			size_t total_verts = 0;
			size_t total_indices = 0;
			for (size_t i = 0; i < vertex_goup_count; ++i) {
				auto submesh = mesh->GetMeshGroup(i).lock();
				if (submesh) {
					VertexGroup group;
					group.index_count = submesh->indicies.size();
					group.starting_offset = total_indices;
					group.mesh_group_number = i;
					this->vertex_groups.push_back(std::move(group));
					total_verts += submesh->verts.size();
					total_indices += submesh->indicies.size();
				}
			}
			std::vector<VertexData> all_verts;
			all_verts.reserve(total_verts);
			std::vector<GLuint> all_indices;
			all_indices.reserve(total_indices);
			size_t vert_offset = 0;
			for (size_t i = 0; i < vertex_goup_count; ++i) {
				auto submesh = mesh->GetMeshGroup(i).lock();
				if (submesh) {
					vert_offset = all_verts.size();
					all_verts.insert(all_verts.end(), submesh->verts.begin(), submesh->verts.end());
					for (auto index : submesh->indicies) {
						all_indices.push_back(index + vert_offset);
					}
				}
			}
			Load(all_verts, all_indices);
		}
	}

	void VertexBufferObject::Load(const std::vector<VertexData>& verts, const std::vector<GLuint>& indices) {
		if (!this->vao) {
			glGenVertexArrays(1, &this->vao);
		}
		if (!this->vbo) {
			glGenBuffers(1, &this->vbo);
		}
		if (!this->ibo) {
			glGenBuffers(1, &this->ibo);
		}

		glBindVertexArray(this->vao);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		// If the size hasn't changed we can call update
		if (this->vertex_count >= verts.size()) {
			auto* buffer = glMapBufferRange(GL_ARRAY_BUFFER, 0, verts.size() *
				sizeof(VertexData), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
			if (buffer) {
				memcpy(buffer, &verts[0], verts.size() * sizeof(VertexData));
				glUnmapBuffer(GL_ARRAY_BUFFER);
			}
			else {
				//std::err << "glMapBufferRange() failed" << __LINE__ << __FILE__ << std::endl;
			}
		}
		else {
			glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(VertexData), &verts[0], GL_STATIC_DRAW);
			this->vertex_count = verts.size();
		}

		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
			(GLvoid*)offsetof(VertexData, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData),
			(GLvoid*)offsetof(VertexData, color));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
		if (this->index_count >= indices.size()) {
			auto* buffer = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() *
				sizeof(GLuint), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
			if (buffer) {
				memcpy(buffer, &indices[0], indices.size() * sizeof(GLuint));
				glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
			}
			else {
				//std::err << "glMapBufferRange() failed" << __LINE__ << __FILE__ << std::endl;
			}
		}
		else {
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
			this->index_count = indices.size();
		}
		glBindVertexArray(0);
	}
}
