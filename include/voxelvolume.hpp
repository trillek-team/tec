#pragma once

#include <unordered_map>
#include <map>
#include <memory>
#include <vector>
#include <queue>
#include <cstdint>

#include "multiton.hpp"
#include "command-queue.hpp"

namespace vv {
	struct Vertex;
	class PolygonMeshData;

	struct Voxel {
		enum NEIGHBORS { UP = 0, DOWN, LEFT, RIGHT, FRONT, BACK };
		float color[3];
		std::weak_ptr<Voxel> neighbors[6];
	};

	class VoxelVolume;
	typedef Multiton<eid, std::shared_ptr<VoxelVolume>> VoxelVoumeMap;

	typedef Command<VoxelVolume> VoxelCommand;

	class VoxelVolume : public CommandQueue < VoxelVolume > {
	public:
		VoxelVolume(const eid entity_id, std::weak_ptr<PolygonMeshData> mesh, const size_t submesh = 0);
		~VoxelVolume();
	public:
		// Iterates over all the actions queued before the call to update.
		void Update(double delta);

		// Generates a vertex (and index) buffer for the current voxel state.
		void UpdateMesh();

		std::weak_ptr<PolygonMeshData> GetMesh();

		// All values are relative to the front orientation and centered on the root voxel.
		// Slice is depth (away from the screen is positive). Row is up/down. Column is left/right.
		// Therefore adding a voxel to the front face is AddVoxel(0, 0, 1).
		void AddVoxel(const std::int16_t row, const std::int16_t column, const std::int16_t slice);

		// See AddVoxel().
		void RemoveVoxel(const std::int16_t row, const std::int16_t column, const std::int16_t slice);

		// Creates a VoxelVolume for entity_id and uses a PolygonMeshData with name and into submesh.
		static std::weak_ptr<VoxelVolume> Create(const eid entity_id,
			const std::string name, const size_t submesh = 0);
		// Creates a VoxelVolume for entity_id and uses PolygonMeshData and into submesh.
		static std::weak_ptr<VoxelVolume> Create(const eid entity_id,
			std::weak_ptr<PolygonMeshData> mesh = std::weak_ptr<PolygonMeshData>(), const size_t submesh = 0);
	private:
		std::unordered_map<std::int64_t, std::shared_ptr<Voxel>> voxels;
		std::weak_ptr<PolygonMeshData> mesh;
		size_t submesh;
		eid entity_id;
	};
}
