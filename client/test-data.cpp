// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include <iostream>
#include <map>
#include <set>
#include <memory>

#include <spdlog/spdlog.h>
#include <google/protobuf/util/json_util.h>
#include <components.pb.h>

// Used in InitializeComponents
#include "components/transforms.hpp" // TODO: Figured out who should handle loading Scale component
#include "voxel-volume.hpp"

// Used in InitializeFileFactories 
#include "resources/md5mesh.hpp"
#include "resources/obj.hpp"
#include "resources/md5anim.hpp"
#include "resources/vorbis-stream.hpp"
#include "resources/script-file.hpp"

#include "entity.hpp"
#include "types.hpp"
#include "events.hpp"

namespace tec {
	void BuildTestEntities() {
		auto voxvol = VoxelVolume::Create(1000, "bob");
		auto voxvol_shared = voxvol.lock();

		VoxelCommand add_voxel(
			[](VoxelVolume* vox_vol) {
			for (int16_t i = -25; i <= 25; ++i) {
				for (int16_t j = -25; j <= 25; ++j) {
					vox_vol->AddVoxel(-1, i, j);
				}
			}
		});
		VoxelVolume::QueueCommand(std::move(add_voxel));
		voxvol_shared->Update(0.0);
	}

	// Move below to more appropriate classes or files

	std::map<std::string, std::function<void(std::string)>> file_factories;
	template <typename T>
	void AddFileFactory() {
		file_factories[GetTypeEXT<T>()] = [](std::string fname) {
			FilePath path(fname);
			if (path.isAbsolutePath()) {
				T::Create(fname);
			}
			else {
				T::Create(FilePath::GetAssetPath(fname));
			}
		};
	}

	void InitializeFileFactories() {
		AddFileFactory<MD5Mesh>();
		AddFileFactory<MD5Anim>();
		AddFileFactory<OBJ>();
		AddFileFactory<VorbisStream>();
		AddFileFactory<ScriptFile>();
	}

	std::string LoadJSON(const FilePath& fname) {
		std::fstream input(fname.GetNativePath(), std::ios::in | std::ios::binary);
		std::string in;
		input.seekg(0, std::ios::end);
		in.reserve(static_cast<std::size_t>(input.tellg()));
		input.seekg(0, std::ios::beg);
		std::copy((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>(), std::back_inserter(in));
		input.close();
		return in;
	}

	void ProtoLoadEntity(const FilePath& fname) {
		std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
		std::string json_string = LoadJSON(fname);
		google::protobuf::util::JsonStringToMessage(json_string, &data->entity);
		data->entity_id = data->entity.id();
		EventSystem<EntityCreated>::Get()->Emit(data);
	}

	void ProtoLoad(std::string filename) {
		auto _log = spdlog::get("console_log");
		FilePath fname = FilePath::GetAssetPath(filename);
		if (fname.isValidPath() && fname.FileExists()) {
			std::string json_string = LoadJSON(fname);
			proto::EntityFileList elist;
			google::protobuf::util::JsonStringToMessage(json_string, &elist);
			_log->debug("[ProtoLoad] :\n {}", elist.DebugString());
			for (int i = 0; i < elist.entity_file_list_size(); i++) {
				FilePath entity_filename = FilePath::GetAssetPath(elist.entity_file_list(i));
				if (entity_filename.isValidPath() && entity_filename.FileExists()) {
					ProtoLoadEntity(entity_filename);
				}
				else {
					_log->error("[ProtoLoadEntity] Error opening {} file. Can't find it", entity_filename.FileName());
				}
			}
		}
		else {
			_log->error("[ProtoLoad] Error opening {} file. Can't find it\n", fname.FileName());
		}
	}
}
