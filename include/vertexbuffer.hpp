#pragma once

#include <vector>
#include <memory>
#include <cstring>

#include "multiton.hpp"

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

namespace vv {
	struct VertexBuffer;
	typedef Multiton<eid, std::shared_ptr<VertexBuffer>> VertexBufferMap;

	// TODO: Move to Mesh class file
	struct Vertex {
		Vertex(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f,
			const float r = 0.0f, const float g = 0.0f, const float b = 0.0f) {
			this->position[0] = x; this->position[1] = y; this->position[2] = z;
			this->color[0] = r; this->color[1] = g; this->color[2] = b;
		}
		float position[3];
		float color[3];
	};

	// Holds vertex and index buffer "names".
	struct VertexBuffer {
		VertexBuffer() : vao(0), vbo(0), ibo(0), vertex_count(0), index_count(0) { }
		void Buffer(const std::vector<Vertex>& verts, const std::vector<GLuint>& indicies) {
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
					sizeof(Vertex), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
				if (buffer) {
					memcpy(buffer, &verts[0], verts.size() * sizeof(Vertex));
					glUnmapBuffer(GL_ARRAY_BUFFER);
				}
				else {
					//std::err << "glMapBufferRange() failed" << __LINE__ << __FILE__ << std::endl;
				}
			}
			else {
				glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);
				this->vertex_count = verts.size();
			}

			glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(GLvoid*)offsetof(Vertex, position));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(GLvoid*)offsetof(Vertex, color));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
			if (this->index_count >= indicies.size()) {
				auto* buffer = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, indicies.size() *
					sizeof(GLuint), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
				if (buffer) {
					memcpy(buffer, &indicies[0], indicies.size() * sizeof(GLuint));
					glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
				}
				else {
					//std::err << "glMapBufferRange() failed" << __LINE__ << __FILE__ << std::endl;
				}
			}
			else {
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLuint), &indicies[0], GL_STATIC_DRAW);
				this->index_count = indicies.size();
			}
			glBindVertexArray(0);
		}
		GLuint vao, vbo, ibo;
		size_t vertex_count;
		size_t index_count;
	};
}
