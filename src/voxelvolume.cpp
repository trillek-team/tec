#include "voxelvolume.hpp"
#include "vertexbuffer.hpp"
#include "polygonmeshdata.hpp"

namespace vv {
	VoxelVolume::VoxelVolume(const eid entity_id, std::weak_ptr<PolygonMeshData> mesh, const size_t submesh) :
		entity_id(entity_id), mesh(mesh), submesh(submesh) { }

	VoxelVolume::~VoxelVolume() { }

	void VoxelVolume::AddVoxel(const std::int16_t row, const std::int16_t column, const std::int16_t slice) {
		Voxel v;
		std::int64_t index = (static_cast<std::uint64_t>(row & 0xFFFF) << 32) +
			(static_cast<std::uint32_t>(column & 0xFFFF) << 16) + static_cast<std::uint16_t>(slice & 0xFFFF);

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
		std::vector<Vertex> verts;
		std::vector<unsigned int> indicies;
		std::map<std::tuple<float, float, float>, unsigned int> index_list;
		static std::vector<Vertex> IdentityVerts({
			// Front
			Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f),	// Bottom left
			Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f),	// Bottom right
			Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),		// Top right
			Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	// Top Left
			// Back
			Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f),	// Bottom left
			Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f),	// Bottom right
			Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f),	// Top right
			Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f)	// Top left
		});

		for (auto v : this->voxels) {
			std::int16_t row = static_cast<std::int16_t>((v.first & 0xFFFF00000000) >> 32);
			std::int16_t column = static_cast<std::int16_t>((v.first & 0xFFFF0000) >> 16);
			std::int16_t slice = static_cast<std::int16_t>(v.first & 0xFFFF);
			GLuint index[8];

			for (size_t i = 0; i < 8; ++i) {
				auto vert_position = std::make_tuple(IdentityVerts[i].position[0] + column * 2,
					IdentityVerts[i].position[1] + row * 2, IdentityVerts[i].position[2] + slice * 2);

				if (index_list.find(vert_position) == index_list.end()) {
					verts.push_back(Vertex(IdentityVerts[i].position[0] + column * 2,
						IdentityVerts[i].position[1] + row * 2, IdentityVerts[i].position[2] + slice * 2,
						IdentityVerts[i].color[0], IdentityVerts[i].color[1], IdentityVerts[i].color[2]));
					//v.second.color[0], v.second.color[1], v.second.color[2]));
					index_list[vert_position] = index_list.size();
				}
				index[i] = index_list[vert_position];
			}

			// Front
			indicies.push_back(index[0]); indicies.push_back(index[1]); indicies.push_back(index[2]);
			indicies.push_back(index[2]); indicies.push_back(index[3]); indicies.push_back(index[0]);
			// Top
			indicies.push_back(index[3]); indicies.push_back(index[2]); indicies.push_back(index[6]);
			indicies.push_back(index[6]); indicies.push_back(index[7]); indicies.push_back(index[3]);
			// Back
			indicies.push_back(index[7]); indicies.push_back(index[6]); indicies.push_back(index[5]);
			indicies.push_back(index[5]); indicies.push_back(index[4]); indicies.push_back(index[7]);
			// Bottom
			indicies.push_back(index[4]); indicies.push_back(index[5]); indicies.push_back(index[1]);
			indicies.push_back(index[1]); indicies.push_back(index[0]); indicies.push_back(index[4]);
			// Left
			indicies.push_back(index[4]); indicies.push_back(index[0]); indicies.push_back(index[3]);
			indicies.push_back(index[3]); indicies.push_back(index[7]); indicies.push_back(index[4]);
			// Right
			indicies.push_back(index[1]); indicies.push_back(index[5]); indicies.push_back(index[6]);
			indicies.push_back(index[6]); indicies.push_back(index[2]); indicies.push_back(index[1]);

			auto m = this->mesh.lock();
			if (m) {
				m->SetVerts(verts, this->submesh);
				m->SetIndicies(indicies, this->submesh);
			}
		}
	}

	std::weak_ptr<PolygonMeshData> VoxelVolume::GetMesh() {
		return this->mesh;
	}

	std::weak_ptr<VoxelVolume> VoxelVolume::Create(eid entity_id,
		const std::string name, const size_t submesh) {
		std::weak_ptr<PolygonMeshData> mesh = PolygonMeshMap::Get(name);
		if (!mesh.lock()) {
			mesh = PolygonMeshData::Create(name);
		}
		auto voxvol = std::make_shared<VoxelVolume>(entity_id, mesh, submesh);
		VoxelVoumeMap::Set(entity_id, voxvol);
		return voxvol;
	}

	std::weak_ptr<VoxelVolume> VoxelVolume::Create(eid entity_id,
		std::weak_ptr<PolygonMeshData> mesh, const size_t submesh) {
		auto voxvol = std::make_shared<VoxelVolume>(entity_id, mesh, submesh);
		VoxelVoumeMap::Set(entity_id, voxvol);
		return voxvol;
	}
}
