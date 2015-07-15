#include "graphics/vertex-buffer-object.hpp"
#include "graphics/shader.hpp"
#include "graphics/material.hpp"
#include "graphics/texture-object.hpp"
#include "components/transforms.hpp"
#include "components/collisionbody.hpp"
#include "components/renderable.hpp"
#include "resources/md5mesh.hpp"
#include "resources/obj.hpp"
#include "resources/pixel-buffer.hpp"
#include "resources/md5anim.hpp"
#include "resources/vorbis-stream.hpp"
#include "graphics/animation.hpp"
#include "render-system.hpp"
#include "entity.hpp"
#include "component-update-system.hpp"
#include "sound-system.hpp"
#include "physics-system.hpp"
#include "voxelvolume.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../proto/components.pb.h"

namespace tec {
	void IntializeComponents() {
		ComponentUpdateSystem<Velocity>::Initialize();
		ComponentUpdateSystem<Position>::Initialize();
		ComponentUpdateSystem<Orientation>::Initialize();
		ComponentUpdateSystem<Scale>::Initialize();
		ComponentUpdateSystem<Renderable>::Initialize();
		ComponentUpdateSystem<View>::Initialize();
		ComponentUpdateSystem<Animation>::Initialize();
		ComponentUpdateSystem<CollisionBody>::Initialize();
		ComponentUpdateSystem<AudioSource>::Initialize();
	}

	std::map<proto::Component::ComponentCase, std::function<void(proto::Entity*)>> out_functors;
	std::map<proto::Component::ComponentCase, std::function<void(const proto::Entity&, const proto::Component&)>> in_functors;
	void IntializeIOFunctors() {
		in_functors[proto::Component::ComponentCase::kPosition] = [ ] (const proto::Entity& entity, const proto::Component& comp) {
			auto position = std::make_shared<Position>();
			position->In(comp.position());
			Entity(entity.id()).Add(position);
		};

		out_functors[proto::Component::ComponentCase::kPosition] = [ ] (proto::Entity* entity) {
			Entity e(entity->id());
			if (e.Has<Position>()) {
				proto::Component* comp = entity->add_components();
				e.Get<Position>().lock()->Out(comp->mutable_position());
			}
		};

		in_functors[proto::Component::ComponentCase::kOrientation] = [ ] (const proto::Entity& entity, const proto::Component& comp) {
			auto orientation = std::make_shared<Orientation>();
			orientation->In(comp.orientation());
			Entity(entity.id()).Add(orientation);
		};

		out_functors[proto::Component::ComponentCase::kOrientation] = [ ] (proto::Entity* entity) {
			Entity e(entity->id());
			if (e.Has<Orientation>()) {
				proto::Component* comp = entity->add_components();
				e.Get<Orientation>().lock()->Out(comp->mutable_orientation());
			}
		};
	}

	void BuildTestEntities() {
		auto shader_files = std::list < std::pair<Shader::ShaderType, std::string> > {
			std::make_pair(Shader::VERTEX, "assets/basic.vert"), std::make_pair(Shader::FRAGMENT, "assets/basic.frag"),
		};
		auto shader1 = Shader::CreateFromFile("shader1", shader_files);

		auto debug_shader_files = std::list < std::pair<Shader::ShaderType, std::string> > {
			std::make_pair(Shader::VERTEX, "assets/debug.vert"), std::make_pair(Shader::FRAGMENT, "assets/debug.frag"),
		};
		auto debug_shader = Shader::CreateFromFile("debug", debug_shader_files);

		auto debug_fill = Material::Create("material_debug");
		debug_fill->SetPolygonMode(GL_LINE);
		debug_fill->SetDrawElementsMode(GL_LINES);

		auto voxvol = VoxelVolume::Create(100, "bob", 0);
		auto voxvol_shared = voxvol.lock();
		Entity voxel1(100);

		VoxelCommand add_voxel(
			[ ] (VoxelVolume* vox_vol) {
			vox_vol->AddVoxel(-1, 1, 1);
			vox_vol->AddVoxel(-1, -1, 1);
			vox_vol->AddVoxel(-1, -1, 0);
			vox_vol->AddVoxel(-1, -1, -1);
			vox_vol->AddVoxel(-1, 0, 1);
			vox_vol->AddVoxel(-1, 0, 0);
			vox_vol->AddVoxel(-1, 0, -1);
			vox_vol->AddVoxel(1, -1, 1);
		});
		VoxelVolume::QueueCommand(std::move(add_voxel));
		voxvol_shared->Update(0.0);
		{
			std::shared_ptr<VertexBufferObject> vbo = std::make_shared<VertexBufferObject>();
			std::shared_ptr<Mesh> mesh = voxvol_shared->GetMesh().lock();
			vbo->Load(mesh);
			voxel1.Add<Renderable>(vbo, debug_shader);
			std::shared_ptr<CollisionBody> colbody = std::make_shared<CollisionMesh>(100, mesh);
			colbody->mass = 0.0;
			voxel1.Add(colbody);
		}

		{
			Entity bob(99);
			std::shared_ptr<MD5Mesh> mesh1 = MD5Mesh::Create("assets/bob/bob.md5mesh");
			std::shared_ptr<VertexBufferObject> vbo = std::make_shared<VertexBufferObject>();
			vbo->Load(mesh1);
			bob.Add<Renderable>(vbo, shader1);

			std::shared_ptr<MD5Anim> anim1 = MD5Anim::Create("assets/bob/bob.md5anim", mesh1);
			bob.Add<Animation>(anim1);
			std::shared_ptr<CollisionBody> colbody = std::make_shared<CollisionCapsule>(99, 1.0f, 0.5f);
			bob.Add(colbody);

			std::shared_ptr<VorbisStream> vorbis_stream = VorbisStream::Create("assets/theme.ogg");
			bob.Add<AudioSource>(vorbis_stream, true);
		}

		{
			Entity vidstand(101);
			std::shared_ptr<OBJ> vidmesh = OBJ::Create("assets/vidstand/VidStand_Full.obj");
			std::shared_ptr<VertexBufferObject> vbo = std::make_shared<VertexBufferObject>();
			vbo->Load(vidmesh);
			vidstand.Add<Renderable>(vbo, shader1);
		}

		{
			Entity camera(1);
			camera.Add<Velocity>();
			std::shared_ptr<View> view = std::make_shared<View>();
			view->active = true;
			camera.Add<View>(view);
			std::shared_ptr<CollisionBody> colbody = std::make_shared<CollisionCapsule>(1, 0.5f, 0.5f);
			colbody->disable_deactivation = true;
			colbody->mass = 1.0;
			colbody->disable_rotation = true;
			camera.Add(colbody);
		}

		Entity floor(1000);
		{
			std::shared_ptr<CollisionBody> colbody = std::make_shared<CollisionBox>(1000, 100.0f, 1.0f, 100.0f);
			colbody->mass = 0.0;
			floor.Add(colbody);
		}

	}

	void ProtoLoad() {
		std::fstream input("assets/test.proto", std::ios::in | std::ios::binary);
		proto::EntityList elist;
		elist.ParseFromIstream(&input);
		for (int i = 0; i < elist.entities_size(); i++) {
			const proto::Entity& entity = elist.entities(i);
			eid entity_id = entity.id();
			for (int i = 0; i < entity.components_size(); ++i) {
				const proto::Component& comp = entity.components(i);
				if (in_functors.find(comp.component_case()) != in_functors.end()) {
					in_functors[comp.component_case()](entity, comp);
				}
			}
		}
	}

	void ProtoSave() {
		std::fstream output("assets/test.proto", std::ios::out | std::ios::trunc | std::ios::binary);
		proto::EntityList elist;
		for (auto pos_itr = Multiton<eid, std::shared_ptr<Position>>::Begin(); pos_itr != Multiton<eid, std::shared_ptr<Position>>::End(); ++pos_itr) {
			proto::Entity* entity = elist.add_entities();
			entity->set_id(pos_itr->first);
			out_functors[proto::Component::ComponentCase::kPosition](entity);
			out_functors[proto::Component::ComponentCase::kOrientation](entity);
		}
		elist.SerializeToOstream(&output);
	}
}
