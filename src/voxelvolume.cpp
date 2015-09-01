#include "voxelvolume.hpp"
#include "graphics/vertex-buffer-object.hpp"

namespace tec {
	VoxelVolume::VoxelVolume(const eid entity_id, std::weak_ptr<MeshFile> mesh) :
		entity_id(entity_id), mesh(mesh) { }

	VoxelVolume::~VoxelVolume() { }

	void VoxelVolume::AddVoxel(const std::int16_t row, const std::int16_t column, const std::int16_t slice) {
		Voxel v;
		std::int64_t index = (static_cast<std::uint64_t>(row & 0xFFFF) << 32) +
			(static_cast<std::uint32_t>(column & 0xFFFF) << 16) + static_cast<std::uint16_t>(slice & 0xFFFF);
		this->changed_queue.push(index);

		if (this->voxels.find(index) == this->voxels.end()) {
			this->voxels[index] = std::make_shared<Voxel>();

			// Since we are adding a voxel we must set the new voxels neighbors.
			std::int64_t up_index, down_index, left_index, right_index, back_index, front_index;
			up_index = (static_cast<std::uint64_t>((row + 1) & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>(column & 0xFFFF) << 16) + static_cast<std::uint16_t>(slice & 0xFFFF);
			down_index = (static_cast<std::uint64_t>((row - 1) & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>(column & 0xFFFF) << 16) + static_cast<std::uint16_t>(slice & 0xFFFF);
			left_index = (static_cast<std::uint64_t>(row & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>((column - 1) & 0xFFFF) << 16) + static_cast<std::uint16_t>(slice & 0xFFFF);
			right_index = (static_cast<std::uint64_t>(row & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>((column + 1) & 0xFFFF) << 16) + static_cast<std::uint16_t>(slice & 0xFFFF);
			front_index = (static_cast<std::uint64_t>(row & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>(column & 0xFFFF) << 16) + static_cast<std::uint16_t>((slice - 1) & 0xFFFF);
			back_index = (static_cast<std::uint64_t>(row & 0xFFFF) << 32) +
				(static_cast<std::uint32_t>(column & 0xFFFF) << 16) + static_cast<std::uint16_t>((slice + 1) & 0xFFFF);

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

	void VoxelVolume::RemoveVoxel(const std::int16_t row, const std::int16_t column, const std::int16_t slice) {
		std::int64_t index = (static_cast<std::uint64_t>(row & 0xFFFF) << 32) +
			(static_cast<std::uint32_t>(column & 0xFFFF) << 16) + static_cast<std::uint16_t>(slice & 0xFFFF);
		this->changed_queue.push(index);

		if (this->voxels.find(index) != this->voxels.end()) {
			std::weak_ptr<Voxel> v = this->voxels[index];
			this->voxels.erase(index);
		}
	}

	void VoxelVolume::Update(double delta) {
		ProcessCommandQueue();
		UpdateMesh();
	}

	void VoxelVolume::UpdateMesh() {
		static std::vector<VertexData> IdentityVerts({
			// Front
			VertexData(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),	// Bottom right
			VertexData(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),		// Top right
			VertexData(-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),	// Top Left
			// Back
			VertexData(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),	// Bottom right
			VertexData(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),	// Top right
			VertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),	// Top left
			// Left
			VertexData(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),	// Bottom right
			VertexData(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),	// Top right
			VertexData(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),	// Top Left
			// Right
			VertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),	// Bottom right
			VertexData(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),	// Top right
			VertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),		// Top Left
			// Top
			VertexData(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),		// Bottom right
			VertexData(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),	// Top right
			VertexData(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),	// Top Left
			// Bottom
			VertexData(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),	// Bottom left
			VertexData(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),	// Bottom right
			VertexData(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),	// Top right
			VertexData(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f)	// Top Left
		});

		// TODO: Sort voxels by material and update object group material groups.
		// TODO: use object groups to do a sort of "chunk" grouping to reduce load?
		auto m = this->mesh.lock();
		if (m) {
			m->Invalidate();
			Mesh* mesh = nullptr;
			if (m->GetMeshCount() > 0) {
				mesh = m->GetMesh(0);
			}
			else {
				mesh = m->CreateMesh();
			}
			ObjectGroup* objgroup = nullptr;
			if (mesh->object_groups.size() == 0) {
				mesh->object_groups.push_back(new ObjectGroup());
			}
			objgroup = mesh->object_groups[0];
			while (this->changed_queue.size() > 0) {
				std::int64_t index = this->changed_queue.front();
				this->changed_queue.pop();
				if (this->voxels.find(index) != this->voxels.end()) {
					std::int16_t row = static_cast<std::int16_t>((index & 0xFFFF00000000) >> 32);
					std::int16_t column = static_cast<std::int16_t>((index & 0xFFFF0000) >> 16);
					std::int16_t slice = static_cast<std::int16_t>(index & 0xFFFF);
					size_t vertex_offset = mesh->verts.size();
					for (size_t i = 0; i < 24; ++i) {
						mesh->verts.push_back(std::move(VertexData(IdentityVerts[i].position[0] + column * 2,
							IdentityVerts[i].position[1] + row * 2, IdentityVerts[i].position[2] + slice * 2,
							IdentityVerts[i].color[0], IdentityVerts[i].color[1], IdentityVerts[i].color[2],
							IdentityVerts[i].uv[0], IdentityVerts[i].uv[1])));
						this->vertex_index[index] = mesh->verts.size() - 1;
					}
					for (size_t i = 0; i < 6; ++i) {
						objgroup->indicies.push_back(i * 4 + vertex_offset + 0);
						objgroup->indicies.push_back(i * 4 + vertex_offset + 1);
						objgroup->indicies.push_back(i * 4 + vertex_offset + 2);
						objgroup->indicies.push_back(i * 4 + vertex_offset + 2);
						objgroup->indicies.push_back(i * 4 + vertex_offset + 3);
						objgroup->indicies.push_back(i * 4 + vertex_offset + 0);
					}
				}
				else {
					if (this->vertex_index.find(index) != this->vertex_index.end()) {
						for (size_t i = 0; i < objgroup->indicies.size(); ++i) {
							if (objgroup->indicies[i] == this->vertex_index[index]) {
								for (int j = 35; j >= 0; --j) {
									std::swap(objgroup->indicies[i + j], objgroup->indicies.back());
									objgroup->indicies[i + j] = this->vertex_index[index] + j;
									objgroup->indicies.pop_back();
								}
								i += 35;
							}
						}
						for (size_t i = 0; i < 24; i++) {
							std::swap(mesh->verts[this->vertex_index[index] + i], mesh->verts.back());
							mesh->verts.pop_back();
						}
					}
				}
			}
			if (objgroup->material_groups.size() == 0) {
				MaterialGroup mat_group = {0, objgroup->indicies.size(), "voxel"};
				mat_group.textures.push_back("metal_wall");
				objgroup->material_groups.push_back(std::move(mat_group));
			}
			objgroup->material_groups[0].count = objgroup->indicies.size();
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
		auto voxvol = std::make_shared<VoxelVolume>(entity_id, mesh);
		VoxelVoumeMap::Set(entity_id, voxvol);
		return voxvol;
	}

	std::weak_ptr<VoxelVolume> VoxelVolume::Create(eid entity_id, std::weak_ptr<MeshFile> mesh) {
		auto voxvol = std::make_shared<VoxelVolume>(entity_id, mesh);
		VoxelVoumeMap::Set(entity_id, voxvol);
		return voxvol;
	}
}
