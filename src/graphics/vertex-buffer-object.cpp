#include "graphics/vertex-buffer-object.hpp"

#include "resources/mesh.hpp"

namespace tec {
	VertexBufferObject::VertexBufferObject() : vao(0), vbo(0), ibo(0), vertex_count(0), index_count(0) { }

	VertexBufferObject::VertexBufferObject(std::weak_ptr<Mesh> mesh) :
		vao(0), vbo(0), ibo(0), vertex_count(0), index_count(0), source_ptr(mesh) {
		auto locked_ptr = mesh.lock();
		if (locked_ptr) {
			Load(locked_ptr);
		}
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

	GLuint VertexBufferObject::GetIndexCount() { return this->index_count; }

	bool VertexBufferObject::IsDynamic() { return !this->source_ptr.expired(); }

	void VertexBufferObject::Update() {
		std::shared_ptr<Mesh> locked_ptr = this->source_ptr.lock();
		if (locked_ptr) {
			Load(*locked_ptr->GetVertexBuffer(), *locked_ptr->GetIndexBuffer());
		}
	}

	void VertexBufferObject::Load(std::weak_ptr<Mesh> mesh) {
		std::shared_ptr<Mesh> locked_ptr = this->source_ptr.lock();
		if (locked_ptr) {
			// TODO: Iterate over all mesh groups and add them to the vertex buffer.
			Load(*locked_ptr->GetVertexBuffer(), *locked_ptr->GetIndexBuffer());
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
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
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
