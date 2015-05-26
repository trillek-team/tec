#pragma once

#include <memory>
#include <vector>

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

namespace tec {
	class Mesh;
	struct VertexData;

	class VertexBufferObject {
	public:
		VertexBufferObject();

		VertexBufferObject(std::weak_ptr<Mesh> mesh);

		~VertexBufferObject();

		/**
		 * \brief Delete the underlaying GL buffers
		 */
		void Destroy();

		/**
		 * \brief Get the ID of the vertex array object.
		 * note: this method is not const, since GL can modify the ID
		 * \return GLuint the GL texture ID
		 */
		GLuint GetVAO();

		/**
		 * \brief Get the ID of the index buffer object.
		 * note: this method is not const, since GL can modify the ID
		 * \return GLuint the GL texture ID
		 */
		GLuint GetIBO();

		/**
		 * \brief Gets the count of indices in the index buffer object.
		 * \return size_t The number of indices.
		 */
		size_t GetIndexCount();

		/**
		 * \return true If the vertex buffer object was created dynamic
		 */
		bool IsDynamic();

		/**
		 * Called to update dynamic vertex buffer object.
		 */
		void Update();

		/**
		 * Loads a mesh into a vertex buffer object.
		 * note: calls Load(const std::vector<VertexData>& verts, const std::vector<GLuint>& indices).
		 */
		void Load(std::weak_ptr<Mesh> mesh);

		/**
		 * Loads a set of vertex and index data into a vertex buffer object.
		 */
		void Load(const std::vector<VertexData>& verts, const std::vector<GLuint>& indices);
	private:
		GLuint vao, vbo, ibo;
		size_t vertex_count;
		size_t index_count;
		std::weak_ptr<Mesh> source_ptr;
	};
}
