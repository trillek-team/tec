#pragma once

#include <cstring>
#include <memory>
#include <vector>

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <resources/mesh.hpp>

namespace tec {
class Material;
class Shader;

struct VertexGroup {
	std::size_t mesh_group_number{0};
	std::size_t index_count{0};
	std::size_t starting_offset{0};
	std::shared_ptr<Material> material;
};

class VertexBufferObject {
public:
	VertexBufferObject(vertex::FormatCode _load_format);

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
	GLuint GetVAO() { return this->vao; }

	/**
	* \brief Gets the specified VertexGroup.
	* \param size_t vertex_group_number Number of the vertex group to get.
	* \return VertexGroup& The specified VertexGroup.
	*/
	VertexGroup* GetVertexGroup(const std::size_t vertex_group_number);

	/**
	* \brief Gets the number of vertex groups store in the buffer.
	* \return size_t The number of vertex groups.
	*/
	std::size_t GetVertexGroupCount() const;

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
	* \param std::shared_ptr<MeshFile> mesh The mesh to load into this VertexBufferObject.
	* \return void
	*/
	void Load(std::shared_ptr<MeshFile> mesh);

private:
	vertex::FormatCode load_format;
	GLuint vao{0}, bufferobj[2]{0, 0};
	std::size_t vertex_count{0}; // Total vertex count.
	std::size_t index_count{0}; // Total index count.
	std::vector<VertexGroup> vertex_groups;
	std::weak_ptr<MeshFile> source_mesh;
};
} // namespace tec
