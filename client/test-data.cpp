// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "components/transforms.hpp"
#include "components/collision-body.hpp"
#include "components/lua-script.hpp"

#include "resources/md5mesh.hpp"
#include "resources/obj.hpp"
#include "resources/md5anim.hpp"
#include "resources/vorbis-stream.hpp"
#include "resources/script-file.hpp"

#include "graphics/shader.hpp"
#include "graphics/material.hpp"
#include "graphics/texture-object.hpp"
#include "graphics/animation.hpp"
#include "graphics/lights.hpp"
#include "graphics/view.hpp"
#include "graphics/renderable.hpp"

#include "entity.hpp"
#include "types.hpp"

#include "sound-system.hpp"
#include "vcomputer-system.hpp"
#include "physics-system.hpp"
#include "voxel-volume.hpp"
#include "lua-system.hpp"

#include <spdlog/spdlog.h>
#include <google/protobuf/util/json_util.h>

#include <iostream>
#include <map>
#include <set>
#include <memory>

#include "components.pb.h"

namespace tec {
	std::map<tid, std::function<void(const proto::Entity&, const proto::Component&)>> in_functors;
	std::map<tid, std::function<void(const proto::Entity&, const proto::Component&, const state_id_t)>> update_functors;
	std::map<std::string, std::function<void(std::string)>> file_factories;

	template <typename T>
	void AddInOutFunctors() {
		in_functors[GetTypeID<T>()] = [](const proto::Entity& entity, const proto::Component& proto_comp) {
			T* comp = new T();
			comp->In(proto_comp);
			Multiton<eid, T*>::Set(entity.id(), comp);
		};
		update_functors[GetTypeID<T>()] = [](const proto::Entity& entity, const proto::Component& proto_comp, const state_id_t) {
			T* comp = new T();
			comp->In(proto_comp);
			Multiton<eid, T*>::Set(entity.id(), comp);
		};
	}

	void InitializeComponents() {
		AddInOutFunctors<DirectionalLight>();
		AddInOutFunctors<PointLight>();
		AddInOutFunctors<Scale>();
		AddInOutFunctors<AudioSource>();
		AddInOutFunctors<LuaScript>();
		AddInOutFunctors<Computer>();
	}

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
		AddFileFactory<OBJ>();
		AddFileFactory<VorbisStream>();
		AddFileFactory<ScriptFile>();
	}

	void BuildTestEntities() {
		auto debug_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> >{
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/debug.vert")),
				std::make_pair(Shader::FRAGMENT, FilePath::GetAssetPath("shaders/debug.frag")),
		};
		auto debug_shader = Shader::CreateFromFile("debug", debug_shader_files);

		auto debug_fill = Material::Create("material_debug");
		debug_fill->SetPolygonMode(GL_LINE);
		debug_fill->SetDrawElementsMode(GL_LINES);

		auto deferred_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> >{
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/deferred_geometry.vert")),
				std::make_pair(Shader::FRAGMENT, FilePath::GetAssetPath("shaders/deferred_geometry.frag")),
		};
		auto deferred_shader = Shader::CreateFromFile("deferred", deferred_shader_files);

		auto deferred_pl_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> >{
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/deferred_light.vert")),
				std::make_pair(Shader::FRAGMENT, FilePath::GetAssetPath("shaders/deferred_pointlight.frag")),
		};
		auto deferred_pl_shader = Shader::CreateFromFile("deferred_pointlight", deferred_pl_shader_files);

		auto deferred_dl_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> >{
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/deferred_light.vert")),
				std::make_pair(Shader::FRAGMENT, FilePath::GetAssetPath("shaders/deferred_dirlight.frag")),
		};
		auto deferred_dl_shader = Shader::CreateFromFile("deferred_dirlight", deferred_dl_shader_files);

		auto deferred_stencil_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> >{
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/deferred_light.vert")),
		};
		auto deferred_stencil_shader = Shader::CreateFromFile("deferred_stencil", deferred_stencil_shader_files);

		auto deferred_shadow_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> >{
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/deferred_shadow.vert")),
				std::make_pair(Shader::FRAGMENT, FilePath::GetAssetPath("shaders/deferred_shadow.frag")),
		};
		auto deferred_shadow_shader = Shader::CreateFromFile("deferred_shadow", deferred_shadow_shader_files);

		{
			Entity bob(99);
			std::shared_ptr<MD5Mesh> mesh1 = MD5Mesh::Create(FilePath::GetAssetPath("bob/bob.md5mesh"));
			std::shared_ptr<MD5Anim> anim1 = MD5Anim::Create(FilePath::GetAssetPath("bob/bob.md5anim"), mesh1);
			Multiton<eid, Animation*>::Set(99, new Animation(anim1));
			//std::shared_ptr<ScriptFile> script1 = ScriptFile::Create("Script1", FilePath::GetAssetPath("scripts/test.lua"));
			//bob.Add<LuaScript>(script1);
		}

		auto voxvol = VoxelVolume::Create(1000, "bob");
		auto voxvol_shared = voxvol.lock();
		auto pixbuf = PixelBuffer::Create("metal_wall", FilePath::GetAssetPath("metal_wall.png"));
		auto tex = std::make_shared<TextureObject>(pixbuf);
		TextureMap::Set("metal_wall", tex);

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

		{
			Entity vidstand(101);
			std::shared_ptr<ComputerKeyboard> keybaord = std::make_shared<ComputerKeyboard>();
			vidstand.Add(keybaord);

			VComputerCommand add_devoces(
				[keybaord](VComputerSystem* vcomputer) {
				if (vcomputer->LoadROMFile(101, FilePath::GetAssetPath("asm/type1.ffi").toString())) {
					vcomputer->SetDevice(101, 1, keybaord);
				}
			});
			VComputerSystem::QueueCommand(std::move(add_devoces));
		}
	}

	std::string LoadJSON(const FilePath& fname) {
		std::fstream input(fname.GetNativePath(), std::ios::in | std::ios::binary);
		std::string in;
		input.seekg(0, std::ios::end);
		in.reserve(static_cast<std::size_t>(input.tellg()));
		input.seekg(0, std::ios::beg);
		std::copy((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>(), std::back_inserter(in));
		input.close();
		return std::move(in);
	}

	void ProtoLoadEntity(const FilePath& fname) {
		std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
		std::string json_string = LoadJSON(fname);
		google::protobuf::util::JsonStringToMessage(json_string, &data->entity);
		data->entity_id = data->entity.id();;
		EventSystem<EntityCreated>::Get()->Emit(data);
		for (int i = 0; i < data->entity.components_size(); ++i) {
			const proto::Component& comp = data->entity.components(i);
			if (in_functors.find(comp.component_case()) != in_functors.end()) {
				in_functors[comp.component_case()](data->entity, comp);
			}
		}
	}

	void ProtoLoad() {
		const std::string file("json/test.json");

		auto _log = spdlog::get("console_log");
		FilePath fname = FilePath::GetAssetPath(file);
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
