#include "vertex-buffer-object.hpp"
#include <glm/gtc/packing.hpp>
#include <spdlog/spdlog.h>

#include "material.hpp"
#include "resources/mesh.hpp"
#include "shader.hpp"
#include "texture-object.hpp"

namespace tec {

namespace vertex {

struct AttribData {
	GLuint channel;
	GLuint count;
	GLuint kind;
	GLuint normalize;
	size_t offset;
};

/**
* \brief Holds relevent data to process and bind vertex data via glVertexAttrib*Pointer calls
* The convert() function loads data from a Mesh into the described format
*/
struct FormatDefinition {
	typedef std::function<void(void* dest, const Mesh* source)> func_t;
	const size_t format_stride;
	const std::vector<AttribData> info;
	const func_t convert;
	template <typename C, typename... A>
	FormatDefinition(const C*, func_t f, A&&... a) : format_stride{sizeof(C)}, info{a...}, convert{f} {}
};
// this helper initializes a FormatDefinition with the vertex type
template <typename T> struct FormatDefine : public FormatDefinition {
	template <typename... A>
	FormatDefine(func_t f, A&&... a) : FormatDefinition((T*)nullptr, f, std::forward<A>(a)...) {}
};
// formats are defined by calling a specialized constructor of this type
template <typename Q> struct FormatInfo { static FormatDefine<Q> define_format; };
// the basic Vertex data, and an example of how the Attribute data is provided
// it's mostly just manually filling it in
// Attribute ID bindings in TEC currently follow a simple convention:
// 0-2 - position, normal, uv (aka texture coord)
// 3   - vertex color
// 4   - skinning weights (XYZ are used)
// 5   - skinning indices (there are 4, and must always be an integer format)
template <>
FormatDefine<Vertex> FormatInfo<Vertex>::define_format(
		[](void* dest, const Mesh* source) {
			auto destination = (vertex::Vertex*)dest;
			for (size_t i = 0; i < source->vert_count(); i++) {
				destination[i] = source->base_verts[i];
			}
		},
		// it is possible to have type inference fill in the element count and datatype(GL_FLOAT etc)
		// but I went with the simple option of fill-in all the things.
		AttribData{0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position)},
		AttribData{1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal)},
		AttribData{2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv)});

template <>
FormatDefine<ColorVertex> FormatInfo<ColorVertex>::define_format(
		[](void* dest, const Mesh* source) {
			auto destination = (vertex::ColorVertex*)dest;
			for (size_t i = 0; i < source->vert_count(); i++) {
				destination[i].vert = source->base_verts[i];
				destination[i].color = source->vert_color[i];
			}
		},
		AttribData{0, 3, GL_FLOAT, GL_FALSE, offsetof(ColorVertex, vert.position)},
		AttribData{1, 3, GL_FLOAT, GL_FALSE, offsetof(ColorVertex, vert.normal)},
		AttribData{2, 2, GL_FLOAT, GL_FALSE, offsetof(ColorVertex, vert.uv)},
		AttribData{3, 4, GL_FLOAT, GL_FALSE, offsetof(ColorVertex, color)});

template <>
FormatDefine<LiteFullVertex> FormatInfo<LiteFullVertex>::define_format(
		[](void* dest, const Mesh* source) {
			// light-ish on memory, but it takes some processing to put together
			auto destination = (vertex::LiteFullVertex*)dest;
			for (size_t i = 0; i < source->vert_count(); i++) {
				destination[i].position = source->base_verts[i].position;
				destination[i].pack_normal = glm::packSnorm3x10_1x2(glm::vec4(source->base_verts[i].normal, 0.f));
				*reinterpret_cast<uint32_t*>(&destination[i].uv) = glm::packHalf2x16(source->base_verts[i].uv);
				if (source->has_color)
					*reinterpret_cast<uint32_t*>(&destination[i].color) = glm::packUnorm4x8(source->vert_color[i]);
				if (source->has_weight) {
					*reinterpret_cast<uint32_t*>(&destination[i].bone_weights) =
							glm::packUnorm4x8(source->vert_weight[i].bone_weights);
					destination[i].bone_indices = source->vert_weight[i].bone_indices;
				}
			}
		},
		AttribData{0, 3, GL_FLOAT, GL_FALSE, offsetof(LiteFullVertex, position)},
		AttribData{1, 4, GL_INT_2_10_10_10_REV, GL_TRUE, offsetof(LiteFullVertex, pack_normal)},
		AttribData{2, 2, GL_HALF_FLOAT, GL_FALSE, offsetof(LiteFullVertex, uv)},
		AttribData{3, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(LiteFullVertex, color)},
		AttribData{4, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(LiteFullVertex, bone_weights)},
		AttribData{5, 4, GL_UNSIGNED_BYTE, GL_FALSE, offsetof(LiteFullVertex, bone_indices)});

template <>
FormatDefine<FullVertex> FormatInfo<FullVertex>::define_format(
		[](void* dest, const Mesh* source) {
			// when memory is no object
			auto destination = (vertex::FullVertex*)dest;
			for (size_t i = 0; i < source->vert_count(); i++) {
				destination[i].vert = source->base_verts[i];
				if (source->has_color)
					destination[i].color = source->vert_color[i];
				if (source->has_weight)
					destination[i].weight = source->vert_weight[i];
			}
		},
		AttribData{0, 3, GL_FLOAT, GL_FALSE, offsetof(FullVertex, vert.position)},
		AttribData{1, 3, GL_FLOAT, GL_FALSE, offsetof(FullVertex, vert.normal)},
		AttribData{2, 2, GL_FLOAT, GL_FALSE, offsetof(FullVertex, vert.uv)},
		AttribData{3, 4, GL_FLOAT, GL_FALSE, offsetof(FullVertex, color)},
		AttribData{4, 4, GL_FLOAT, GL_FALSE, offsetof(FullVertex, weight.bone_weights)},
		AttribData{5, 4, GL_UNSIGNED_INT, GL_FALSE, offsetof(FullVertex, weight.bone_indices)});

const FormatDefinition* from_vftype[]{
		&FormatInfo<Vertex>::define_format, // VF_BASE
		&FormatInfo<LiteFullVertex>::define_format, // VF_LITEFULL
		nullptr, // VF_WEIGHT
		&FormatInfo<ColorVertex>::define_format, // VF_COLOR
		nullptr, // VF_PWC
		&FormatInfo<FullVertex>::define_format, // VF_FULL
};

} // namespace vertex

VertexBufferObject::VertexBufferObject(vertex::FormatCode _load_format) : load_format{_load_format} {}

VertexBufferObject::~VertexBufferObject() { Destroy(); }

void VertexBufferObject::Destroy() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(2, this->bufferobj);
	this->vao = 0;
	this->bufferobj[0] = 0;
	this->bufferobj[1] = 0;
}

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

void VertexBufferObject::Load(std::shared_ptr<MeshFile> meshes) {
	auto _log = spdlog::get("console_log");
	if (!meshes) { // someone might want to know!
		_log->error("VertexBufferObject::Load null MeshFile");
		return;
	}
	this->source_mesh = meshes;
	this->vertex_groups.clear();
	const vertex::FormatDefinition* vertex_format = vertex::from_vftype[this->load_format];
	if (!vertex_format) {
		_log->error("FIXME: missing vertex::from_vftype[{}] in {}", this->load_format, __FILE__);
		return;
	}
	const size_t stride = vertex_format->format_stride;
	GLuint vert_offset{0};
	GLuint index_offset{0};
	uint8_t* vert_buffer{nullptr}; // bytes so we can offset by stride
	GLuint* index_buffer{nullptr};
	size_t all_vertices_size{0}; // the total number of vertices in the MeshFile
	size_t all_indices_size{0}; // the total number of indices in the MeshFile
	for (std::size_t i{0}; i < meshes->GetMeshCount(); ++i) {
		Mesh* mfmesh = meshes->GetMesh(i);
		all_vertices_size += mfmesh->vert_count();
		for (ObjectGroup* objgroup : mfmesh->object_groups) {
			all_indices_size += objgroup->indices.size();
		}
	}
	if ((all_vertices_size == 0) || (all_indices_size == 0)) {
		_log->error("VertexBufferObject::Load error: empty data: v:{}, i:{}", all_vertices_size, all_indices_size);
		return;
	}
	_log->debug("[VBO]::Load mesh data: v:{}, i:{}", all_vertices_size, all_indices_size);

	if (this->vao) {
		glBindVertexArray(this->vao);
		glBindBuffer(GL_ARRAY_BUFFER, this->bufferobj[0]);
	}
	else {
		glGenVertexArrays(1, &this->vao);
		glGenBuffers(2, this->bufferobj);
		glBindVertexArray(this->vao);
		glBindBuffer(GL_ARRAY_BUFFER, this->bufferobj[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->bufferobj[1]); // attach the ELEMENT_ARRAY to the VAO
		// if this is the first time, we bind attributes
		for (auto& attrib : vertex_format->info) {
			if (attrib.channel == 5) // just always assumed to be integer bone indices
				glVertexAttribIPointer(attrib.channel, attrib.count, attrib.kind, stride, (GLvoid*)attrib.offset);
			else
				glVertexAttribPointer(
						attrib.channel, attrib.count, attrib.kind, attrib.normalize, stride, (GLvoid*)attrib.offset);
			glEnableVertexAttribArray(attrib.channel);
		}
	}
	// Scale up the buffers if they are too small
	if (this->vertex_count < all_vertices_size) {
		glBufferData(GL_ARRAY_BUFFER, all_vertices_size * stride, nullptr, GL_STATIC_DRAW);
		this->vertex_count = all_vertices_size;
	}
	if (this->index_count < all_indices_size) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, all_indices_size * sizeof(GLuint), nullptr, GL_STATIC_DRAW);
		this->index_count = all_indices_size;
	}
	GLuint mapping_bits = GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	vert_buffer = (uint8_t*)glMapBufferRange(GL_ARRAY_BUFFER, 0, this->vertex_count * stride, mapping_bits);
	if (!vert_buffer) {
		_log->error("glMapBufferRange(GL_ARRAY_BUFFER) failed {}:{} - GL:{}", __FILE__, __LINE__, glGetError());
		goto error_leave;
	}
	index_buffer =
			(GLuint*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, this->index_count * sizeof(GLuint), mapping_bits);
	if (!index_buffer) {
		_log->error("glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER) failed {}:{} - GL:{}", __FILE__, __LINE__, glGetError());
		goto error_leave;
	}

	for (std::size_t i{0}; i < meshes->GetMeshCount(); ++i) {
		Mesh* mfmesh = meshes->GetMesh(i);
		vertex_format->convert(vert_buffer + (vert_offset * stride), mfmesh);
		for (ObjectGroup* objgroup : mfmesh->object_groups) {
			for (auto mat_group : objgroup->material_groups) {
				VertexGroup group;
				group.index_count = mat_group.count;
				group.starting_offset = mat_group.start + index_offset;
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
				assert((index + vert_offset) < (vert_offset + mfmesh->vert_count()));
				index_buffer[index_offset++] = index + vert_offset;
			}
		}
		vert_offset += mfmesh->vert_count();
	}
	// if no explicit groups defined, definite an implicit one
	if (this->vertex_groups.size() == 0) {
		VertexGroup group;
		group.index_count = all_indices_size;
		group.starting_offset = 0;
		group.mesh_group_number = 0;
		this->vertex_groups.push_back(group);
	}
error_leave:
	if (vert_buffer) {
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	if (index_buffer) {
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0); // all done
	glBindVertexArray(0); // finished with vertex array
}

} // namespace tec
