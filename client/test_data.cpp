#include "components/transforms.hpp"
#include "components/collisionbody.hpp"
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
#include "voxelvolume.hpp"
#include "lua-system.hpp"

#include <spdlog/spdlog.h>

#include <iostream>
#include <map>
#include <set>
#include <memory>

#include "../proto/components.pb.h"

namespace tec {
	std::map<tid, std::function<void(proto::Entity*)>> out_functors;
	std::map<tid, std::function<void(const proto::Entity&, const proto::Component&)>> in_functors;
	std::map<tid, std::function<void(const proto::Entity&, const proto::Component&, const state_id_t)>> update_functors;
	std::map<eid, std::set<std::function<void(proto::Entity*)>*>> entity_out_functors;
	std::map<std::string, std::function<void(std::string)>> file_factories;
	std::map<std::string, std::function<void(eid)>> component_factories;
	std::map<std::string, std::function<void(eid)>> component_removal_factories;

	template <typename T>
	void AddComponentFactory() {
		auto component_case = GetTypeID<T>();
		component_factories[GetTypeName<T>()] = [component_case] (eid entity_id) {
			std::shared_ptr<T> comp = std::make_shared<T>();
			Entity(entity_id).Add<T>(comp);
			entity_out_functors[entity_id].insert(&out_functors.at(component_case));
		};
		component_removal_factories[GetTypeName<T>()] = [component_case] (eid entity_id) {
			Entity e(entity_id);
			if (e.Has<T>()) {
				e.Remove<T>();
			}
		};
	}
	template <typename T>
	void AddInOutFunctors() {
		in_functors[GetTypeID<T>()] = [ ] (const proto::Entity& entity, const proto::Component& proto_comp) {
			auto comp = std::make_shared<T>();
			comp->In(proto_comp);
			Entity(entity.id()).Add<T>(comp);
		};
		update_functors[GetTypeID<T>()] = [ ] (const proto::Entity& entity, const proto::Component& proto_comp, const state_id_t frame_id) {
			auto comp = std::make_shared<T>();
			comp->In(proto_comp);
			Entity(entity.id()).Update<T>(comp);
		};
		out_functors[GetTypeID<T>()] = [ ] (proto::Entity* entity) {
			Entity e(entity->id());
			if (e.Has<T>()) {
				proto::Component* comp = entity->add_components();
				e.Get<T>().lock()->Out(comp);
			}
		};
	}
	
	template <typename T>
	void AddPointerInOutFunctors() {
		in_functors[GetTypeID<T>()] = [ ] (const proto::Entity& entity, const proto::Component& proto_comp) {
			T* comp = new T();
			comp->In(proto_comp);
			Multiton<eid, T*>::Set(entity.id(), comp);
		};
		update_functors[GetTypeID<T>()] = [ ] (const proto::Entity& entity, const proto::Component& proto_comp, const state_id_t frame_id) {
			T* comp = new T();
			comp->In(proto_comp);
			Multiton<eid, T*>::Set(entity.id(), comp);
		};
		out_functors[GetTypeID<T>()] = [ ] (proto::Entity* entity) {
			Entity e(entity->id());
			if (e.Has<T*>()) {
				proto::Component* comp = entity->add_components();
				Multiton<eid, T*>::Get(entity->id())->Out(comp);
			}
		};
	}
	template <typename T>
	void AddPointerComponentFactory() {
		auto component_case = GetTypeID<T>();
		component_factories[GetTypeName<T>()] = [component_case] (eid entity_id) {
			T* comp = new T();
			Multiton<eid, T*>::Set(entity_id, comp);
			entity_out_functors[entity_id].insert(&out_functors.at(component_case));
		};
		component_removal_factories[GetTypeName<T>()] = [component_case] (eid entity_id) {
			Entity e(entity_id);
			if (e.Has<T*>()) {
				delete Multiton<eid, T*>::Get(entity_id);
				e.Remove<T*>();
			}
		};
	}

	template <typename T>
	void SetupComponent() {
		AddInOutFunctors<T>();
		AddComponentFactory<T>();
	}
	template <typename T>
	void SetupComponentP() {
		AddPointerInOutFunctors<T>();
		AddPointerComponentFactory<T>();
	}

	void InitializeComponents() {
		SetupComponent<Renderable>();
		SetupComponent<Position>();
		SetupComponent<Orientation>();
		SetupComponent<Scale>();
		SetupComponent<Velocity>();
		SetupComponent<View>();
		SetupComponent<Animation>();
		SetupComponentP<CollisionBody>();
		SetupComponent<AudioSource>();
		SetupComponent<PointLight>();
		SetupComponent<DirectionalLight>();
		SetupComponent<LuaScript>();
		SetupComponent<Computer>();
	}

	template <typename T>
	void AddFileFactory() {
		file_factories[GetTypeEXT<T>()] = [ ] (std::string fname) {
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
		auto debug_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> > {
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/debug.vert")),
				std::make_pair(Shader::FRAGMENT, FilePath::GetAssetPath("shaders/debug.frag")),
		};
		auto debug_shader = Shader::CreateFromFile("debug", debug_shader_files);

		auto debug_fill = Material::Create("material_debug");
		debug_fill->SetPolygonMode(GL_LINE);
		debug_fill->SetDrawElementsMode(GL_LINES);

		auto deferred_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> > {
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/deferred_geometry.vert")),
				std::make_pair(Shader::FRAGMENT, FilePath::GetAssetPath("shaders/deferred_geometry.frag")),
		};
		auto deferred_shader = Shader::CreateFromFile("deferred", deferred_shader_files);

		auto deferred_pl_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> > {
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/deferred_light.vert")),
				std::make_pair(Shader::FRAGMENT, FilePath::GetAssetPath("shaders/deferred_pointlight.frag")),
		};
		auto deferred_pl_shader = Shader::CreateFromFile("deferred_pointlight", deferred_pl_shader_files);

		auto deferred_dl_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> > {
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/deferred_light.vert")),
				std::make_pair(Shader::FRAGMENT, FilePath::GetAssetPath("shaders/deferred_dirlight.frag")),
		};
		auto deferred_dl_shader = Shader::CreateFromFile("deferred_dirlight", deferred_dl_shader_files);

		auto deferred_stencil_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> > {
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/deferred_light.vert")),
		};
		auto deferred_stencil_shader = Shader::CreateFromFile("deferred_stencil", deferred_pl_shader_files);

		auto deferred_shadow_shader_files = std::list < std::pair<Shader::ShaderType, FilePath> > {
			std::make_pair(Shader::VERTEX, FilePath::GetAssetPath("shaders/deferred_shadow.vert")),
				std::make_pair(Shader::FRAGMENT, FilePath::GetAssetPath("shaders/deferred_shadow.frag")),
		};
		auto deferred_shadow_shader = Shader::CreateFromFile("deferred_shadow", deferred_shadow_shader_files);

		auto voxvol = VoxelVolume::Create(100, "bob");
		auto voxvol_shared = voxvol.lock();
		FilePath metal_wall_filename = FilePath::GetAssetPath("metal_wall.png");
		auto pixbuf = PixelBuffer::Create("metal_wall", metal_wall_filename);
		auto tex = std::make_shared<TextureObject>(pixbuf);
		TextureMap::Set("metal_wall", tex);

		VoxelCommand add_voxel(
			[ ] (VoxelVolume* vox_vol) {
			for (int i = -5; i <= 5; ++i) {
				for (int j = -25; j <= 25; ++j) {
					vox_vol->AddVoxel(-1, i, j);
				}
			}
		});
		VoxelVolume::QueueCommand(std::move(add_voxel));
		voxvol_shared->Update(0.0);

		VoxelCommand rem_voxel(
			[ ] (VoxelVolume* vox_vol) {
			vox_vol->RemoveVoxel(-1, 5, 5);
		});
		VoxelVolume::QueueCommand(std::move(rem_voxel));
		voxvol_shared->Update(0.0);
		{
			Entity voxel1(100);
			voxel1.Add(voxvol_shared);
		}

		{
			Entity bob(99);
			std::shared_ptr<MD5Mesh> mesh1 = MD5Mesh::Create(FilePath::GetAssetPath("bob/bob.md5mesh"));
			std::shared_ptr<MD5Anim> anim1 = MD5Anim::Create(FilePath::GetAssetPath("bob/bob.md5anim"), mesh1);
			bob.Add<Animation>(anim1);
			std::shared_ptr<ScriptFile> script1 = ScriptFile::Create("Script1", FilePath::GetAssetPath("scripts/test.lua"));
			bob.Add<LuaScript>(script1);
		}

		{
			Entity vidstand(101);
			std::shared_ptr<ComputerKeyboard> keybaord = std::make_shared<ComputerKeyboard>();
			vidstand.Add(keybaord);

			VComputerCommand add_devoces(
				[keybaord] (VComputerSystem* vcomputer) {
				if (vcomputer->LoadROMFile(101, FilePath::GetAssetPath("asm/type1.ffi").toString())) {
					vcomputer->SetDevice(101, 1, keybaord);
				}
			});
			VComputerSystem::QueueCommand(std::move(add_devoces));
		}
	}

	// NOTE : This would be change on a future, not ? Actually is loading/saving from assets (where not is supposed to be write by a user)

	void ProtoLoadEntity(const FilePath& fname) {
		auto _log = spdlog::get("console_log");
		if (fname.isValidPath() && fname.FileExists()) {
			std::fstream input(fname.GetNativePath(), std::ios::in | std::ios::binary);
			std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
			data->entity.ParseFromIstream(&input);
			eid entity_id = data->entity.id();
			data->entity_id = entity_id;
			EventSystem<EntityCreated>::Get()->Emit(data);
			for (int i = 0; i < data->entity.components_size(); ++i) {
				const proto::Component& comp = data->entity.components(i);
				if (in_functors.find(comp.component_case()) != in_functors.end()) {
					in_functors[comp.component_case()](data->entity, comp);
					entity_out_functors[entity_id].insert(&out_functors.at(comp.component_case()));
				}
			}
		}
		else {
			_log->error() << "[ProtoLoadEntity] Error opening " << fname.FileName() << " file. Can't find it";
		}
	}

	void ProtoLoad() {
		const std::string file("test.proto");

		auto _log = spdlog::get("console_log");
		FilePath fname = FilePath::GetAssetPath(file);
		if (fname.isValidPath() && fname.FileExists()) {
			std::fstream input(fname.GetNativePath(), std::ios::in | std::ios::binary);
			proto::EntityFileList elist;
			elist.ParseFromIstream(&input);
			_log->debug() << "[ProtoLoad] :\n" << elist.DebugString();
			for (int i = 0; i < elist.entity_file_list_size(); i++) {
				FilePath entity_filename = FilePath::GetAssetPath(elist.entity_file_list(i));
				ProtoLoadEntity(entity_filename);
			}
		}
		else {
			_log->error() << "[ProtoLoad] Error opening " << fname.FileName() << " file. Can't find it\n";
		}
	}

	void ProtoSave() {
		const std::string file("test.proto");

		auto _log = spdlog::get("console_log");
		FilePath fname = FilePath::GetAssetPath("test.proto");
		if (fname.isValidPath()) {
			std::fstream output(fname.GetNativePath(), std::ios::out | std::ios::trunc | std::ios::binary);
			proto::EntityFileList elist;
			for (auto entity_functors : entity_out_functors) {
				proto::Entity entity;
				entity.set_id(entity_functors.first);
				for (auto functor : entity_functors.second) {
					(*functor)(&entity);
				}

				std::string fname = "entities/" + std::to_string(entity_functors.first) + ".proto";
				auto entity_filename = FilePath::GetAssetPath(fname);
				std::fstream entity_output(entity_filename.GetNativePath(), std::ios::out | std::ios::trunc | std::ios::binary);
				entity.SerializeToOstream(&entity_output);
				elist.add_entity_file_list(fname);
			}
			elist.SerializeToOstream(&output);
		}
		else {
			_log->error() << "Error opening test.proto file. Invalid path: " << fname << "\n";
		}

	}
}
