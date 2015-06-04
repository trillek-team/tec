#pragma once

#include <memory>
#include <vector>

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

namespace tec {
	class Material;
	class Shader;

	class Mesh;
	struct VertexData;

	struct VertexGroup {
		size_t mesh_group_number;
		size_t index_count;
		size_t starting_offset;
		std::shared_ptr<Material> material;
	};

	class VertexBufferObject {
	public:
		VertexBufferObject();

		VertexBufferObject(std::shared_ptr<Mesh> mesh);

		~VertexBufferObject();

		/**
		 * \brief Delete the underlying GL buffers
		 */
		void Destroy();

		/**
		 * \brief Get the ID of the vertex array object.
		 * note: this method is not const, since GL can modify the ID
		 * \return GLuint the GL texture ID.
		 */
		const GLuint GetVAO();

		/**
		 * \brief Get the ID of the index buffer object.
		 * note: this method is not const, since GL can modify the ID
		 * \return GLuint the GL texture ID.
		 */
		const GLuint GetIBO();

		/**
		 * \brief Gets the specified VertexGroup.
		 * \param size_t vertex_group_number Number of the vertex group to get.
		 * \return VertexGroup& The specified VertexGroup.
		 */
		VertexGroup* GetVertexGroup(const size_t vertex_group_number);

		/**
		 * \brief Gets the number of vertex groups store in the buffer.
		 * \return size_t The number of vertex groups.
		 */
		size_t GetVertexGroupCount() const;

		/**
		 * \return true If the vertex buffer object was created dynamic
		 */
		bool IsDynamic() const;

		/**
		 * Called to update dynamic vertex buffer object.
		 */
		void Update();

		/**
		 * Loads a mesh into a vertex buffer object.
		 * note: calls Load(const std::vector<VertexData>& verts, const std::vector<GLuint>& indices).
		 * \param std::shared_ptr<Mesh> mesh The mesh to load into this VertexBufferObject.
		 * \param std::shared_ptr<Shader> shader The shader used if a material needs to be created for a VertexGroup.
		 * \return void
		 */
		void Load(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader);

		/**
		 * Loads a set of vertex and index data into a vertex buffer object.
		 */
		void Load(const std::vector<VertexData>& verts, const std::vector<GLuint>& indices);
	private:
		GLuint vao, vbo, ibo;
		size_t vertex_count; // Total vertex count.
		size_t index_count; // Total index count.
		std::vector<VertexGroup> vertex_groups;
		std::weak_ptr<Mesh> source_mesh;
	};
}
