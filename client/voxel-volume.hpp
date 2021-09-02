#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "command-queue.hpp"
#include "event-system.hpp"
#include "multiton.hpp"
#include "resources/mesh.hpp"

namespace tec {
struct MouseClickEvent;
struct Vertex;
class MeshFile;

struct Voxel {
	enum NEIGHBORS { UP = 0, DOWN, LEFT, RIGHT, FRONT, BACK };
	float color[3]{0.f, 0.f, 0.f};
};

class VoxelVolume;
typedef Multiton<eid, std::shared_ptr<VoxelVolume>> VoxelVolumeMap;

typedef Command<VoxelVolume> VoxelCommand;

class VoxelVolume : public CommandQueue<VoxelVolume> {
public:
	VoxelVolume(std::weak_ptr<MeshFile> mesh);
	~VoxelVolume();

public:
	// Iterates over all the actions queued before the call to update.
	void Update(double delta);

	void Out(proto::Component*) {}

	// Generates a vertex (and index) buffer for the current voxel state.
	void UpdateMesh();

	std::weak_ptr<MeshFile> GetMesh();

	// All values are relative to the front orientation and centered on the root voxel.
	// Slice is depth (away from the screen is positive). Row is up/down. Column is left/right.
	// Therefore adding a voxel to the front face is AddVoxel(0, 0, 1).
	void AddVoxel(const std::int16_t row, const std::int16_t column, const std::int16_t slice);

	// See AddVoxel().
	void RemoveVoxel(const std::int16_t row, const std::int16_t column, const std::int16_t slice);

	// Creates a VoxelVolume for entity_id and uses a PolygonMeshData with name and into
	// sub-mesh.
	static std::weak_ptr<VoxelVolume> Create(const eid entity_id, const std::string name);
	// Creates a VoxelVolume for entity_id and uses PolygonMeshData and into sub-mesh.
	static std::weak_ptr<VoxelVolume>
	Create(const eid entity_id, std::weak_ptr<MeshFile> mesh = std::weak_ptr<MeshFile>());

private:
	std::unordered_map<std::int64_t, std::shared_ptr<Voxel>> voxels;
	std::queue<std::int64_t> changed_queue; // Used to reduce update to just what has changed.
	std::unordered_map<std::int64_t, std::size_t> vertex_index; // Used to update or remove voxels from the mesh.
	std::weak_ptr<MeshFile> mesh;
};

class VoxelSystem : public EventQueue<MouseClickEvent> {
public:
	void Update(double delta);

	void On(eid, std::shared_ptr<MouseClickEvent> data) override;

	// TODO: the input scheme for this system conflicts with other systems, here's an easy way to toggle it ;)
	bool edit_allowed{false};

private:
	typedef Multiton<eid, std::shared_ptr<VoxelVolume>> VoxelVolumeMap;
};
} // namespace tec
