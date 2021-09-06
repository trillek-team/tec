#include "voxel-volume.hpp"

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include "components/transforms.hpp"
#include "entity.hpp"
#include "events.hpp"
#include "graphics/texture-object.hpp"
#include "graphics/vertex-buffer-object.hpp"

namespace tec {
VoxelVolume::VoxelVolume(std::weak_ptr<MeshFile> mesh) : mesh(mesh) {
	auto pixbuf = PixelBuffer::Create("metal_wall", Path::GetAssetPath("metal_wall.png"), true);
	auto tex = std::make_shared<TextureObject>(pixbuf);
	TextureMap::Set("metal_wall", tex);
	pixbuf = PixelBuffer::Create("metal_wall_sp", Path::GetAssetPath("metal_wall_sp.png"));
	tex = std::make_shared<TextureObject>(pixbuf);
	TextureMap::Set("metal_wall_sp", tex);
}

VoxelVolume::~VoxelVolume() {}

void VoxelVolume::AddVoxel(const std::int16_t y, const std::int16_t x, const std::int16_t z) {
	std::int64_t index = (static_cast<std::uint64_t>(y & 0xFFFF) << 32) + (static_cast<std::uint32_t>(x & 0xFFFF) << 16)
						 + static_cast<std::uint16_t>(z & 0xFFFF);
	this->changed_queue.push(index);

	if (this->voxels.find(index) == this->voxels.end()) {
		this->voxels[index] = std::make_shared<Voxel>();
	}
}

void VoxelVolume::RemoveVoxel(const std::int16_t y, const std::int16_t x, const std::int16_t z) {
	std::int64_t index = (static_cast<std::uint64_t>(y & 0xFFFF) << 32) + (static_cast<std::uint32_t>(x & 0xFFFF) << 16)
						 + static_cast<std::uint16_t>(z & 0xFFFF);
	this->changed_queue.push(index);

	if (this->voxels.find(index) != this->voxels.end()) {
		this->voxels.erase(index);
	}
}

void VoxelVolume::Update(double) {
	ProcessCommandQueue();
	UpdateMesh();
}

void VoxelVolume::UpdateMesh() {
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	using tec::vertex::Vertex;
	static std::vector<Vertex> IdentityVerts({
			// Front
			Vertex{vec3(0.f, 0.f, 1.f), vec3(0.f, 0.f, 1.f), vec2(0.f, 1.f)}, // Bottom left
			Vertex{vec3(1.f, 0.f, 1.f), vec3(0.f, 0.f, 1.f), vec2(1.f, 1.f)}, // Bottom right
			Vertex{vec3(1.f, 1.f, 1.f), vec3(0.f, 0.f, 1.f), vec2(1.f, 0.f)}, // Top right
			Vertex{vec3(0.f, 1.f, 1.f), vec3(0.f, 0.f, 1.f), vec2(0.f, 0.f)}, // Top Left
			// Back
			Vertex{vec3(1.f, 0.f, 0.f), vec3(0.f, 0.f, -1.f), vec2(0.f, 1.f)}, // Bottom left
			Vertex{vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, -1.f), vec2(1.f, 1.f)}, // Bottom right
			Vertex{vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, -1.f), vec2(1.f, 0.f)}, // Top right
			Vertex{vec3(1.f, 1.f, 0.f), vec3(0.f, 0.f, -1.f), vec2(0.f, 0.f)}, // Top left
			// Left
			Vertex{vec3(0.f, 0.f, 0.f), vec3(-1.f, 0.f, 0.f), vec2(0.f, 1.f)}, // Bottom left
			Vertex{vec3(0.f, 0.f, 1.f), vec3(-1.f, 0.f, 0.f), vec2(1.f, 1.f)}, // Bottom right
			Vertex{vec3(0.f, 1.f, 1.f), vec3(-1.f, 0.f, 0.f), vec2(1.f, 0.f)}, // Top right
			Vertex{vec3(0.f, 1.f, 0.f), vec3(-1.f, 0.f, 0.f), vec2(0.f, 0.f)}, // Top Left
			// Right
			Vertex{vec3(1.f, 0.f, 1.f), vec3(1.f, 0.f, 0.f), vec2(0.f, 1.f)}, // Bottom left
			Vertex{vec3(1.f, 0.f, 0.f), vec3(1.f, 0.f, 0.f), vec2(1.f, 1.f)}, // Bottom right
			Vertex{vec3(1.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), vec2(1.f, 0.f)}, // Top right
			Vertex{vec3(1.f, 1.f, 1.f), vec3(1.f, 0.f, 0.f), vec2(0.f, 0.f)}, // Top Left
			// Top
			Vertex{vec3(0.f, 1.f, 1.f), vec3(0.f, 1.f, 0.f), vec2(0.f, 1.f)}, // Bottom left
			Vertex{vec3(1.f, 1.f, 1.f), vec3(0.f, 1.f, 0.f), vec2(1.f, 1.f)}, // Bottom right
			Vertex{vec3(1.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), vec2(1.f, 0.f)}, // Top right
			Vertex{vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), vec2(0.f, 0.f)}, // Top Left
			// Bottom
			Vertex{vec3(0.f, 0.f, 0.f), vec3(0.f, -1.f, 0.f), vec2(0.f, 1.f)}, // Bottom left
			Vertex{vec3(1.f, 0.f, 0.f), vec3(0.f, -1.f, 0.f), vec2(1.f, 1.f)}, // Bottom right
			Vertex{vec3(1.f, 0.f, 1.f), vec3(0.f, -1.f, 0.f), vec2(1.f, 0.f)}, // Top right
			Vertex{vec3(0.f, 0.f, 1.f), vec3(0.f, -1.f, 0.f), vec2(0.f, 0.f)} // Top Left
	});

	// TODO: Sort voxels by material and update object group material groups.
	// TODO: use object groups to do a sort of "chunk" grouping to reduce load?
	auto m = this->mesh.lock();
	if (m) {
		if (this->changed_queue.size() == 0) {
			return;
		}
		spdlog::get("console_log")->debug("[Voxel] Updating mesh");
		Mesh* _mesh;
		if (m->GetMeshCount() > 0) {
			_mesh = m->GetMesh(0);
		}
		else {
			_mesh = m->CreateMesh();
		}
		if (_mesh->object_groups.size() == 0) {
			_mesh->object_groups.push_back(new ObjectGroup());
		}
		ObjectGroup* objgroup = _mesh->object_groups[0];
		while (this->changed_queue.size() > 0) {
			std::int64_t index = this->changed_queue.front();
			this->changed_queue.pop();
			if (this->voxels.find(index) != this->voxels.end()) {
				std::int16_t y = static_cast<std::int16_t>((index & 0xFFFF00000000) >> 32);
				std::int16_t x = static_cast<std::int16_t>((index & 0xFFFF0000) >> 16);
				std::int16_t z = static_cast<std::int16_t>(index & 0xFFFF);
				std::size_t vertex_offset = _mesh->base_verts.size();
				auto voxel_pos = vec3(x, y, z);
				for (std::size_t i = 0; i < 24; ++i) {
					_mesh->base_verts.push_back(Vertex{
							IdentityVerts[i].position + voxel_pos, IdentityVerts[i].normal, IdentityVerts[i].uv});
				}
				this->vertex_index[index] = vertex_offset;
				for (std::size_t i = 0; i < 6; ++i) {
					objgroup->indices.push_back(static_cast<int>(i * 4 + vertex_offset + 0));
					objgroup->indices.push_back(static_cast<int>(i * 4 + vertex_offset + 1));
					objgroup->indices.push_back(static_cast<int>(i * 4 + vertex_offset + 2));
					objgroup->indices.push_back(static_cast<int>(i * 4 + vertex_offset + 2));
					objgroup->indices.push_back(static_cast<int>(i * 4 + vertex_offset + 3));
					objgroup->indices.push_back(static_cast<int>(i * 4 + vertex_offset + 0));
				}
			}
			else {
				if (this->vertex_index.find(index) != this->vertex_index.end()) {
					for (std::size_t i = 0; i < objgroup->indices.size(); ++i) {
						if (objgroup->indices[i] == this->vertex_index[index]) {
							for (int j = 0; j < 36; ++j, ++i) {
								objgroup->indices.pop_back();
							}
						}
					}
					if (this->vertex_index[index] == _mesh->base_verts.size() - 24) {
						for (std::size_t i = 0; i < 24; i++) {
							_mesh->base_verts.pop_back();
						}
					}
					else {
						for (std::size_t i = 0; i < 24; i++) {
							std::swap(_mesh->base_verts[this->vertex_index[index] + i], _mesh->base_verts.back());
							_mesh->base_verts.pop_back();
						}
						Vertex& vert = _mesh->base_verts[this->vertex_index[index]];
						auto vert_pos = glm::floor(vert.position - IdentityVerts[0].position);

						std::int16_t x = static_cast<std::int16_t>(vert_pos.x);
						std::int16_t y = static_cast<std::int16_t>(vert_pos.y);
						std::int16_t z = static_cast<std::int16_t>(vert_pos.z);

						std::int64_t changed_index =
								(static_cast<std::uint64_t>(y & 0xFFFF) << static_cast<std::uint64_t>(32))
								+ (static_cast<std::uint32_t>(x & 0xFFFF) << static_cast<std::uint32_t>(16))
								+ static_cast<std::uint16_t>(z & 0xFFFF);

						this->vertex_index[changed_index] = this->vertex_index[index];
					}
					this->vertex_index.erase(index);
				}
			}
		}
		if (objgroup->material_groups.size() == 0) {
			MaterialGroup mat_group = {0, static_cast<unsigned int>(objgroup->indices.size()), "voxel"};
			mat_group.textures.push_back("metal_wall");
			mat_group.textures.push_back("metal_wall_sp");
			objgroup->material_groups.push_back(std::move(mat_group));
		}
		objgroup->material_groups[0].count = objgroup->indices.size();
		m->Invalidate();
	}
}

std::weak_ptr<MeshFile> VoxelVolume::GetMesh() { return this->mesh; }

std::weak_ptr<VoxelVolume> VoxelVolume::Create(eid entity_id, const std::string name) {
	std::weak_ptr<MeshFile> mesh = MeshMap::Get(name);
	if (!mesh.lock()) {
		MeshMap::Set(name, std::make_shared<MeshFile>());
		mesh = MeshMap::Get(name);
		mesh.lock()->SetName(name);
	}
	return Create(entity_id, mesh);
}

std::weak_ptr<VoxelVolume> VoxelVolume::Create(eid entity_id, std::weak_ptr<MeshFile> mesh) {
	auto voxvol = std::make_shared<VoxelVolume>(mesh);
	VoxelVolumeMap::Set(entity_id, voxvol);
	return voxvol;
}

void VoxelSystem::On(eid entity_id, std::shared_ptr<MouseClickEvent> data) {
	if (!this->edit_allowed) {
		return;
	}
	if (!VoxelVolumeMap::Has(entity_id)) {
		return;
	}
	auto voxvol = VoxelVolumeMap::Get(entity_id);
	if (data->button == MouseBtnEvent::LEFT) {
		const Position* pos = Entity(entity_id).Get<Position>();
		const Orientation* orientation = Entity(entity_id).Get<Orientation>();
		glm::mat4 model_view =
				glm::inverse(glm::translate(glm::mat4(1.0), pos->value) * glm::mat4_cast(orientation->value));
		glm::vec4 local_coords = model_view * glm::vec4(data->ray_hit_point_world, 1.0f);
		std::int16_t grid_x = static_cast<std::int16_t>(floor(local_coords.x));
		local_coords.y += FLT_EPSILON * (std::signbit(local_coords.y) ? -1.0f : 0.0f);
		std::int16_t grid_y = static_cast<std::int16_t>(floor(local_coords.y));
		std::int16_t grid_z = static_cast<std::int16_t>(floor(local_coords.z));

		voxvol->AddVoxel(grid_y, grid_x, grid_z);
	}
	else if (data->button == MouseBtnEvent::RIGHT) {
		const Position* pos = Entity(entity_id).Get<Position>();
		const Orientation* orientation = Entity(entity_id).Get<Orientation>();
		glm::mat4 model_view =
				glm::inverse(glm::translate(glm::mat4(1.0), pos->value) * glm::mat4_cast(orientation->value));
		glm::vec4 local_coords = model_view * glm::vec4(data->ray_hit_point_world, 1.0f);
		std::int16_t grid_x = static_cast<std::int16_t>(floor(local_coords.x));
		local_coords.y -= FLT_EPSILON * (std::signbit(local_coords.y) ? 0.0f : 1.0f);
		std::int16_t grid_y = static_cast<std::int16_t>(floor(local_coords.y));
		std::int16_t grid_z = static_cast<std::int16_t>(floor(local_coords.z));

		voxvol->RemoveVoxel(grid_y, grid_x, grid_z);
	}
}

void VoxelSystem::Update(double delta) {
	EventQueue<MouseClickEvent>::ProcessEventQueue();
	for (auto itr = VoxelVolumeMap::Begin(); itr != VoxelVolumeMap::End(); ++itr) {
		itr->second->Update(delta);
	}
}

} // namespace tec
