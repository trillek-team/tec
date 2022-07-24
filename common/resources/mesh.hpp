#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "multiton.hpp"

namespace tec {
class MeshFile;
typedef Multiton<std::string, std::shared_ptr<MeshFile>> MeshMap;

// Vertex data used for rendering or other purposes.
namespace vertex {

// 32 bytes, common rendering data.
struct Vertex {
	glm::vec3 position{0.f, 0.f, 0.f};
	glm::vec3 normal{0.f, 0.f, 0.f};
	glm::vec2 uv{0.f, 0.f};
};

// 32 bytes: a small but versatile Vertex format
// offering the attributes of the 80 byte format
struct LiteFullVertex {
	// still full float quality position, seems excessive, but alignment works out
	glm::vec3 position{0.f, 0.f, 0.f};
	glm::u8vec4 color{255, 255, 255, 255};
	// for use with GL_INT_2_10_10_10_REV (normalized).
	// pack_normal = glm::packSnorm3x10_1x2(glm::vec4(glm::vec3(normal), 0.f))
	uint32_t pack_normal;
	// can be either GL_UNSIGNED_SHORT (normalized or not), or half-floats
	glm::u16vec2 uv{0, 0};
	// weight would be normalized GL_UNSIGNED_BYTE
	glm::u8vec4 bone_weights{0, 0, 0, 0};
	// weight index would be GL_UNSIGNED_BYTE, are you really going to use >255 joints?!
	glm::u8vec4 bone_indices{0, 0, 0, 0};
};

// 32 bytes, common data used for animation.
// The forth weight is implied by the shader, so this element is technically unused.
struct WeightData {
	WeightData() = default;
	glm::vec4 bone_weights{0.f, 0.f, 0.f, 0.f};
	glm::uvec4 bone_indices{0, 0, 0, 0};
};

// 64 bytes
struct WeightVertex {
	Vertex vert;
	WeightData weight;
};

// 48 bytes, render vertex with vec4 color
struct ColorVertex {
	Vertex vert;
	glm::vec4 color{1.f, 1.f, 1.f, 1.f};
};

// 64 bytes, integer color, and only 3 weights
struct PackWeightVertex {
	Vertex vert;
	glm::u8vec4 color{255, 255, 255, 255};
	glm::vec3 bone_weights{0.f, 0.f, 0.f}; // Used for animation.
	glm::uvec4 bone_indices{0, 0, 0, 0}; // Used for animation.
};

// 80 bytes!
struct FullVertex {
	Vertex vert;
	glm::vec4 color{1.f, 1.f, 1.f, 1.f};
	WeightData weight;
};

enum FormatCode { VF_BASE, VF_LITEFULL, VF_WEIGHT, VF_COLOR, VF_PWC, VF_FULL };
} // namespace vertex

struct MaterialGroup final {
	std::size_t start{0};
	std::size_t count{0};
	std::string material_name;
	std::list<std::string> textures;
};

struct ObjectGroup final {
	// array of triangle vertex indices, should always be a multiple of 3
	std::vector<unsigned int> indices;
	// materials this subobject uses
	std::vector<MaterialGroup> material_groups;
};

struct Mesh final {
	~Mesh() {
		for (ObjectGroup* object : this->object_groups) {
			if (object) {
				delete object;
			}
		}
	}
	// resize the vertex data arrays used by this mesh
	void resize(size_t n) {
		base_verts.resize(n);
		if (has_color)
			vert_color.resize(n);
		if (has_weight)
			vert_weight.resize(n);
	}
	size_t vert_count() const { return base_verts.size(); }
	std::vector<vertex::Vertex> base_verts;
	std::vector<glm::vec4> vert_color;
	std::vector<vertex::WeightData> vert_weight;
	// if this mesh uses vert_color for per vertex color data
	bool has_color{false};
	// if this mesh uses vert_weight for animation weights
	bool has_weight{false};
	std::vector<ObjectGroup*> object_groups;
};

class MeshFile {
public:
	virtual ~MeshFile();

	/**
	* \brief Creates a new mesh and adds it to this file.
	*
	* \return Mesh* The mesh that was added to this file.
	*/
	Mesh* CreateMesh() {
		this->meshes.push_back(new Mesh());
		return *(this->meshes.end() - 1);
	}

	/**
	* \brief Adds a mesh to this file. Ownership is transferred.
	*
	* \param[in] Mesh* mesh The mesh to add to this file.
	* \return void
	*/
	void AddMesh(Mesh* mesh) { this->meshes.push_back(mesh); }

	/**
	* \brief Returns a specific mesh.
	*
	* \param[in] const unsigned size_t index The index of the mesh to retrieve.
	* \return std::weak_ptr<MeshGroup> The requested mesh or null if the index is invalid.
	*/
	Mesh* GetMesh(const std::size_t index) {
		if (index < this->meshes.size()) {
			return this->meshes.at(index);
		}

		return nullptr;
	}

	/**
	* \brief Returns the number of meshes in this file.
	*
	* \return size_t The number of meshes in this file.
	*/
	std::size_t GetMeshCount() const { return this->meshes.size(); }

	const std::string GetName() const { return this->name; }
	void SetName(const std::string& _name) { this->name = _name; }

	bool IsDirty() const { return this->dirty; }
	/** \brief Mark dirty */
	void Invalidate() { this->dirty = true; }
	/** \brief Mark not dirty */
	void Validate() { this->dirty = false; }

protected:
	std::vector<Mesh*> meshes;
	std::string name{"test"};
	bool dirty{false};
};
} // namespace tec
