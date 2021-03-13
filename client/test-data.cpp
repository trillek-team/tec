#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>

#include <components.pb.h>
#include <google/protobuf/util/json_util.h>
#include <spdlog/spdlog.h>

#include "voxel-volume.hpp"

#include "events.hpp"
#include "filesystem.hpp"

namespace tec {
void BuildTestVoxelVolume() {
	auto voxvol = VoxelVolume::Create(1000, "bob");
	auto voxvol_shared = voxvol.lock();

	VoxelCommand add_voxel([](VoxelVolume* vox_vol) {
		for (int16_t i = -25; i <= 25; ++i) {
			for (int16_t j = -25; j <= 25; ++j) {
				vox_vol->AddVoxel(-1, i, j);
			}
		}
	});
	VoxelVolume::QueueCommand(std::move(add_voxel));
	voxvol_shared->Update(0.0);
}

} // namespace tec
