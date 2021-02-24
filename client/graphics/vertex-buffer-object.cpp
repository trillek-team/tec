#include "vertex-buffer-object.hpp"

#include "material.hpp"
#include "resources/mesh.hpp"
#include "shader.hpp"
#include "texture-object.hpp"

namespace tec {
VertexBufferObject::VertexBufferObject() {}

VertexBufferObject::VertexBufferObject(std::shared_ptr<MeshFile> mesh) : source_mesh(mesh) { Load(mesh); }

VertexBufferObject::~VertexBufferObject() { Destroy(); }

void VertexBufferObject::Destroy() {
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ibo);
	glDeleteVertexArrays(1, &this->vao);
}

const GLuint VertexBufferObject::GetVAO() { return this->vao; }

const GLuint VertexBufferObject::GetIBO() { return this->ibo; }

VertexGroup* VertexBufferObject::GetVertexGroup(const std::size_t vertex_group_number) {
	if (vertex_group_number < this->vertex_groups.size()) {
		return &this->vertex_groups[vertex_group_number];
	}
	return nullptr;
}

std::size_t VertexBufferObject::GetVertexGroupCount() const { return this->vertex_groups.size(); }

bool VertexBufferObject::IsDynamic() const { return !this->source_mesh.expired(); }

void VertexBufferObject::Update() {
	std::shared_ptr<MeshFile> locked_ptr = this->source_mesh.lock();
	if (locked_ptr) {
		if (locked_ptr->IsDirty()) {
			Load(locked_ptr);
			locked_ptr->Validate();
		}
	}
}

void VertexBufferObject::Load(std::shared_ptr<MeshFile> mesh) {
	if (mesh) {
		this->source_mesh = mesh;
		std::vector<GLuint> all_indices;
		std::vector<VertexData> all_verts;
		GLuint vert_offset;
		this->vertex_groups.clear();
		for (std::size_t i = 0; i < mesh->GetMeshCount(); ++i) {
			Mesh* mfmesh = mesh->GetMesh(i);
			vert_offset = static_cast<GLuint>(all_verts.size());
			all_verts.insert(all_verts.end(), mfmesh->verts.begin(), mfmesh->verts.end());
			for (ObjectGroup* objgroup : mfmesh->object_groups) {
				for (auto mat_group : objgroup->material_groups) {
					VertexGroup group;
					group.index_count = mat_group.count;
					group.starting_offset = mat_group.start + all_indices.size();
					group.mesh_group_number = i;
					if (MaterialMap::Has(mat_group.material_name)) {
						group.material = MaterialMap::Get(mat_group.material_name);
					}
					else {
						group.material = Material::Create(mat_group.material_name);
						group.material->SetDrawElementsMode(GL_TRIANGLES);
						group.material->SetPolygonMode(GL_FILL);
						for (auto texture : mat_group.textures) {
							if (TextureMap::Has(texture)) {
								group.material->AddTexture(TextureMap::Get(texture));
							}
						}
					}
					this->vertex_groups.push_back(std::move(group));
				}
				for (GLuint index : objgroup->indices) {
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
		auto* buffer = glMapBufferRange(
				GL_ARRAY_BUFFER,
				0,
				this->vertex_count * sizeof(VertexData),
				GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		if (buffer) {
			auto buffer_size = verts.size() * sizeof(VertexData);
			std::memcpy(buffer, &verts[0], buffer_size);
			std::memset((char*)buffer + buffer_size - 1, 0, (this->vertex_count - verts.size()) * sizeof(VertexData));
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		else {
			// std::err << "glMapBufferRange() failed" << __LINE__ << __FILE__ << std::endl;
		}
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(VertexData), &verts[0], GL_STATIC_DRAW);
		this->vertex_count = verts.size();
	}

	glVertexAttribPointer(
			(GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer((GLuint)3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, uv));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
			(GLuint)4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, bone_weights));
	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(
			(GLuint)5, 4, GL_UNSIGNED_INT, sizeof(VertexData), (GLvoid*)offsetof(VertexData, bone_indices));
	glEnableVertexAttribArray(5);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	if (this->index_count >= indices.size()) {
		auto* buffer = glMapBufferRange(
				GL_ELEMENT_ARRAY_BUFFER,
				0,
				this->index_count * sizeof(GLuint),
				GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		if (buffer) {
			auto buffer_size = indices.size() * sizeof(GLuint);
			std::memcpy(buffer, &indices[0], buffer_size);
			std::memset((char*)buffer + buffer_size - 1, 0, (this->index_count - indices.size()) * sizeof(GLuint));
			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		}
		else {
			// std::err << "glMapBufferRange() failed" << __LINE__ << __FILE__ << std::endl;
		}
	}
	else {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
		this->index_count = indices.size();
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (this->vertex_groups.size() == 0) {
		VertexGroup group;
		group.index_count = indices.size();
		group.starting_offset = 0;
		group.mesh_group_number = 0;
		this->vertex_groups.push_back(group);
	}
}
} // namespace tec
