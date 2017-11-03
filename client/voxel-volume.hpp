// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <unordered_map>
#include <map>
#include <memory>
#include <vector>
#include <queue>
#include <cstdint>

#include "multiton.hpp"
#include "resources/mesh.hpp"
#include "command-queue.hpp"
#include "event-system.hpp"

namespace tec {
	struct MouseClickEvent;
	struct Vertex;
	class MeshFile;

	struct Voxel {
		enum NEIGHBORS { UP = 0, DOWN, LEFT, RIGHT, FRONT, BACK };
		float color[3];
		std::weak_ptr<Voxel> neighbors[6];
	};

	class VoxelVolume;
	typedef Multiton<eid, std::shared_ptr<VoxelVolume>> VoxelVoumeMap;

	typedef Command<VoxelVolume> VoxelCommand;

	class VoxelVolume : public CommandQueue < VoxelVolume >,
		public EventQueue<MouseClickEvent> {
	public:
		VoxelVolume(const eid entity_id, std::weak_ptr<MeshFile> mesh);
		~VoxelVolume();
	public:
		// Iterates over all the actions queued before the call to update.
		void Update(double delta);

		
		void Out(proto::Component* target) {

		}

		// Generates a vertex (and index) buffer for the current voxel state.
		void UpdateMesh();

		std::weak_ptr<MeshFile> GetMesh();

		// All values are relative to the front orientation and centered on the root voxel.
		// Slice is depth (away from the screen is positive). Row is up/down. Column is left/right.
		// Therefore adding a voxel to the front face is AddVoxel(0, 0, 1).
		void AddVoxel(const std::int16_t row, const std::int16_t column, const std::int16_t slice);

		// See AddVoxel().
		void RemoveVoxel(const std::int16_t row, const std::int16_t column, const std::int16_t slice);

		// Creates a VoxelVolume for entity_id and uses a PolygonMeshData with name and into sub-mesh.
		static std::weak_ptr<VoxelVolume> Create(const eid entity_id, const std::string name);
		// Creates a VoxelVolume for entity_id and uses PolygonMeshData and into sub-mesh.
		static std::weak_ptr<VoxelVolume> Create(const eid entity_id, std::weak_ptr<MeshFile> mesh = std::weak_ptr<MeshFile>());
		
		void On(std::shared_ptr<MouseClickEvent> data);
	private:
		std::unordered_map<std::int64_t, std::shared_ptr<Voxel>> voxels;
		std::queue<std::int64_t> changed_queue; // Used to reduce update to just what has changed.
		std::unordered_map<std::int64_t, std::size_t> vertex_index; // Used to update or remove voxels from the mesh.
		std::weak_ptr<MeshFile> mesh;
		eid entity_id;
	};

	class VoxelSystem {
	public:
		void Update(double delta) {
			for (auto itr = VoxelVolumeMap::Begin(); itr != VoxelVolumeMap::End(); ++itr) {
				itr->second->Update(delta);
			}
		}
	private:
		typedef Multiton<eid, std::shared_ptr<VoxelVolume>> VoxelVolumeMap;
	};
}
