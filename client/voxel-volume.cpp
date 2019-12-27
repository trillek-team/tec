// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "voxel-volume.hpp"

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/vertex-buffer-object.hpp"
#include "graphics/texture-object.hpp"
#include "events.hpp"
#include "entity.hpp"
#include "components/transforms.hpp"


namespace tec {
	VoxelVolume::VoxelVolume(const eid entity_id, std::weak_ptr<MeshFile> mesh)
		: mesh(mesh), entity_id(entity_id) {
		auto pixbuf = PixelBuffer::Create("metal_wall", FilePath::GetAssetPath("metal_wall.png"));
		auto tex = std::make_shared<TextureObject>(pixbuf);
		TextureMap::Set("metal_wall", tex);
	}

	VoxelVolume::~VoxelVolume() { }

	void VoxelVolume::AddVoxel(const std::int16_t y, const std::int16_t x, const std::int16_t z) {
		Voxel v;
		std::int64_t index = (static_cast<std::uint64_t>(y & 0xFFFF) << 32) +
			(static_cast<std::uint32_t>(x & 0xFFFF) << 16) + static_cast<std::uint16_t>(z & 0xFFFF);
		this->changed_queue.push(index);

		if (this->voxels.find(index) == this->voxels.end()) {
			this->voxels[index] = std::make_shared<Voxel>();

			// Since we are adding a voxel we must set the new voxels neighbors.
			std::int64_t up_index, down_index, left_index, right_index, back_index, front_index;
			up_index = (static_cast<std::uint64_t>((y + 1) & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>(x & 0xFFFF) << 16) + static_cast<std::uint16_t>(z & 0xFFFF);
			down_index = (static_cast<std::uint64_t>((y - 1) & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>(x & 0xFFFF) << 16) + static_cast<std::uint16_t>(z & 0xFFFF);
			left_index = (static_cast<std::uint64_t>(y & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>((x - 1) & 0xFFFF) << 16) + static_cast<std::uint16_t>(z & 0xFFFF);
			right_index = (static_cast<std::uint64_t>(y & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>((x + 1) & 0xFFFF) << 16) + static_cast<std::uint16_t>(z & 0xFFFF);
			front_index = (static_cast<std::uint64_t>(y & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>(x & 0xFFFF) << 16) + static_cast<std::uint16_t>((z - 1) & 0xFFFF);
			back_index = (static_cast<std::uint64_t>(y & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>(x & 0xFFFF) << 16) + static_cast<std::uint16_t>((z + 1) & 0xFFFF);

			if (this->voxels.find(up_index) != this->voxels.end()) {
				v.neighbors[Voxel::UP] = this->voxels[up_index];
				this->voxels[up_index]->neighbors[Voxel::DOWN] = this->voxels[index];
			}
			if (this->voxels.find(down_index) != this->voxels.end()) {
				v.neighbors[Voxel::DOWN] = this->voxels[down_index];
				this->voxels[down_index]->neighbors[Voxel::UP] = this->voxels[index];
			}
			if (this->voxels.find(left_index) != this->voxels.end()) {
				v.neighbors[Voxel::LEFT] = this->voxels[left_index];
				this->voxels[left_index]->neighbors[Voxel::RIGHT] = this->voxels[index];
			}
			if (this->voxels.find(right_index) != this->voxels.end()) {
				v.neighbors[Voxel::RIGHT] = this->voxels[right_index];
				this->voxels[right_index]->neighbors[Voxel::LEFT] = this->voxels[index];
			}
			if (this->voxels.find(front_index) != this->voxels.end()) {
				v.neighbors[Voxel::FRONT] = this->voxels[front_index];
				this->voxels[front_index]->neighbors[Voxel::BACK] = this->voxels[index];
			}
			if (this->voxels.find(back_index) != this->voxels.end()) {
				v.neighbors[Voxel::BACK] = this->voxels[back_index];
				this->voxels[back_index]->neighbors[Voxel::FRONT] = this->voxels[index];
			}
		}
	}

	void VoxelVolume::RemoveVoxel(const std::int16_t y, const std::int16_t x, const std::int16_t z) {
		std::int64_t index = (static_cast<std::uint64_t>(y & 0xFFFF) << 32) +
			(static_cast<std::uint32_t>(x & 0xFFFF) << 16) + static_cast<std::uint16_t>(z & 0xFFFF);
		this->changed_queue.push(index);

		if (this->voxels.find(index) != this->voxels.end()) {
			std::weak_ptr<Voxel> v = this->voxels[index];
			this->voxels.erase(index);
		}
	}

	void VoxelVolume::Update(double) {
		ProcessCommandQueue();
		EventQueue<MouseClickEvent>::ProcessEventQueue();
		UpdateMesh();
	}

	void VoxelVolume::UpdateMesh() {
		static std::vector<VertexData> IdentityVerts({
			// Front
			VertexData(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	// Bottom right
			VertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),	// Top right
			VertexData(0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),	// Top Left
			// Back
			VertexData(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	// Bottom right
			VertexData(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),	// Top right
			VertexData(1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),	// Top left
			// Left
			VertexData(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	// Bottom right
			VertexData(0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),	// Top right
			VertexData(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),	// Top Left
			// Right
			VertexData(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	// Bottom right
			VertexData(1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),	// Top right
			VertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),	// Top Left
			// Top
			VertexData(0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	// Bottom right
			VertexData(1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),	// Top right
			VertexData(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),	// Top Left
			// Bottom
			VertexData(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	// Bottom right
			VertexData(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),	// Top right
			VertexData(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f)	// Top Left
			});

		// TODO: Sort voxels by material and update object group material groups.
		// TODO: use object groups to do a sort of "chunk" grouping to reduce load?
		auto m = this->mesh.lock();
		if (m) {
			if (this->changed_queue.size() == 0) {
				return;
			}
			Mesh* _mesh = nullptr;
			if (m->GetMeshCount() > 0) {
				_mesh = m->GetMesh(0);
			}
			else {
				_mesh = m->CreateMesh();
			}
			ObjectGroup* objgroup = nullptr;
			if (_mesh->object_groups.size() == 0) {
				_mesh->object_groups.push_back(new ObjectGroup());
			}
			objgroup = _mesh->object_groups[0];
			while (this->changed_queue.size() > 0) {
				std::int64_t index = this->changed_queue.front();
				this->changed_queue.pop();
				if (this->voxels.find(index) != this->voxels.end()) {
					std::int16_t y = static_cast<std::int16_t>((index & 0xFFFF00000000) >> 32);
					std::int16_t x = static_cast<std::int16_t>((index & 0xFFFF0000) >> 16);
					std::int16_t z = static_cast<std::int16_t>(index & 0xFFFF);
					std::size_t vertex_offset = _mesh->verts.size();
					for (std::size_t i = 0; i < 24; ++i) {
						_mesh->verts.push_back(VertexData(IdentityVerts[i].position[0] + x,
							IdentityVerts[i].position[1] + y, IdentityVerts[i].position[2] + z,
							IdentityVerts[i].color[0], IdentityVerts[i].color[1], IdentityVerts[i].color[2],
							IdentityVerts[i].uv[0], IdentityVerts[i].uv[1]));
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
						if (this->vertex_index[index] == _mesh->verts.size() - 24) {
							for (std::size_t i = 0; i < 24; i++) {
								_mesh->verts.pop_back();
							}
						}
						else {
							for (std::size_t i = 0; i < 24; i++) {
								std::swap(_mesh->verts[this->vertex_index[index] + i], _mesh->verts.back());
								_mesh->verts.pop_back();
							}
							VertexData& vert = _mesh->verts[this->vertex_index[index]];

							std::int16_t x = static_cast<std::int16_t>(floor(vert.position.x - IdentityVerts[0].position[0]));
							std::int16_t y = static_cast<std::int16_t>(floor(vert.position.y - IdentityVerts[0].position[1]));
							std::int16_t z = static_cast<std::int16_t>(floor(vert.position.z - IdentityVerts[0].position[2]));

							std::int64_t changed_index = (static_cast<std::uint64_t>(y & 0xFFFF) << static_cast<std::uint64_t>(32)) +
								(static_cast<std::uint32_t>(x & 0xFFFF) << static_cast<std::uint32_t>(16)) + static_cast<std::uint16_t>(z & 0xFFFF);

							this->vertex_index[changed_index] = this->vertex_index[index];
						}
						this->vertex_index.erase(index);
					}
				}
			}
			if (objgroup->material_groups.size() == 0) {
				MaterialGroup mat_group = { 0, static_cast<unsigned int>(objgroup->indices.size()), "voxel" };
				mat_group.textures.push_back("metal_wall");
				objgroup->material_groups.push_back(std::move(mat_group));
			}
			objgroup->material_groups[0].count = objgroup->indices.size();
			m->Invalidate();
		}
	}

	std::weak_ptr<MeshFile> VoxelVolume::GetMesh() {
		return this->mesh;
	}

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
		auto voxvol = std::make_shared<VoxelVolume>(entity_id, mesh);
		VoxelVolumeMap::Set(entity_id, voxvol);
		return voxvol;
	}

	void VoxelVolume::On(std::shared_ptr<MouseClickEvent> data) {
		if (data->button == MouseBtnEvent::LEFT) {
			if (data->entity_id == this->entity_id) {
				const Position* pos = Entity(entity_id).Get<Position>();
				const Orientation* orientation = Entity(entity_id).Get<Orientation>();
				glm::mat4 model_view = glm::inverse(glm::translate(glm::mat4(1.0), pos->value) * glm::mat4_cast(orientation->value));
				glm::vec4 local_coords = model_view * glm::vec4(data->ray_hit_piont_world, 1.0f);
				std::int16_t grid_x = static_cast<std::int16_t>(floor(local_coords.x));
				local_coords.y += FLT_EPSILON * (std::signbit(local_coords.y) ? -1.0f : 0.0f);
				std::int16_t grid_y = static_cast<std::int16_t>(floor(local_coords.y));
				std::int16_t grid_z = static_cast<std::int16_t>(floor(local_coords.z));

				AddVoxel(grid_y, grid_x, grid_z);
			}
		}
		else if (data->button == MouseBtnEvent::RIGHT) {
			if (data->entity_id == this->entity_id) {
				const Position* pos = Entity(entity_id).Get<Position>();
				const Orientation* orientation = Entity(entity_id).Get<Orientation>();
				glm::mat4 model_view = glm::inverse(glm::translate(glm::mat4(1.0), pos->value) * glm::mat4_cast(orientation->value));
				glm::vec4 local_coords = model_view * glm::vec4(data->ray_hit_piont_world, 1.0f);
				std::int16_t grid_x = static_cast<std::int16_t>(floor(local_coords.x));
				local_coords.y -= FLT_EPSILON * (std::signbit(local_coords.y) ? 0.0f : 1.0f);
				std::int16_t grid_y = static_cast<std::int16_t>(floor(local_coords.y));
				std::int16_t grid_z = static_cast<std::int16_t>(floor(local_coords.z));

				RemoveVoxel(grid_y, grid_x, grid_z);
			}
		}
	}
}
