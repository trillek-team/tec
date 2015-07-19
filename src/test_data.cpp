#include "graphics/shader.hpp"
#include "graphics/material.hpp"
#include "components/transforms.hpp"
#include "components/collisionbody.hpp"
#include "components/renderable.hpp"
#include "resources/md5mesh.hpp"
#include "resources/obj.hpp"
#include "resources/md5anim.hpp"
#include "resources/vorbis-stream.hpp"
#include "graphics/animation.hpp"
#include "graphics/view.hpp"
#include "entity.hpp"
#include "component-update-system.hpp"
#include "sound-system.hpp"
#include "physics-system.hpp"
#include "voxelvolume.hpp"
#include <map>
#include <set>
#include <memory>

#include "../proto/components.pb.h"

namespace tec {
	std::map<proto::Component::ComponentCase, std::function<void(proto::Entity*)>> out_functors;
	std::map<proto::Component::ComponentCase, std::function<void(const proto::Entity&, const proto::Component&)>> in_functors;
	std::map<eid, std::set<std::function<void(proto::Entity*)>*>> entity_out_functors;
	std::map<std::string, std::function<void(std::string)>> file_factories;
	std::map<std::string, std::function<void(eid)>> component_factories;

	template <typename T>
	void AddComponentFactory(proto::Component::ComponentCase component_case) {
		component_factories[GetTypeName<T>()] = [component_case] (eid entity_id) {
			std::shared_ptr<T> comp = std::make_shared<T>();
			Entity(entity_id).Add<T>(comp);
			entity_out_functors[entity_id].insert(&out_functors.at(component_case));
		};
	}

	void IntializeComponents() {
		ComponentUpdateSystem<Velocity>::Initialize();
		//AddComponentFactory<Velocity>(proto::Component::ComponentCase::kVelocity);
		ComponentUpdateSystem<Position>::Initialize();
		AddComponentFactory<Position>(proto::Component::ComponentCase::kPosition);
		ComponentUpdateSystem<Orientation>::Initialize();
		AddComponentFactory<Orientation>(proto::Component::ComponentCase::kOrientation);
		ComponentUpdateSystem<Scale>::Initialize();
		AddComponentFactory<Scale>(proto::Component::ComponentCase::kScale);
		ComponentUpdateSystem<Renderable>::Initialize();
		AddComponentFactory<Renderable>(proto::Component::ComponentCase::kRenderable);
		ComponentUpdateSystem<View>::Initialize();
		AddComponentFactory<View>(proto::Component::ComponentCase::kView);
		ComponentUpdateSystem<Animation>::Initialize();
		AddComponentFactory<Animation>(proto::Component::ComponentCase::kAnimation);
		ComponentUpdateSystem<CollisionBody>::Initialize();
		//AddComponentFactory<CollisionBody>(proto::Component::ComponentCase::kCollisionBody);
		ComponentUpdateSystem<AudioSource>::Initialize();
		//AddComponentFactory<AudioSource>(proto::Component::ComponentCase::kAudioSource);
	}
	
	template <typename T>
	void AddFileFactory() {
		file_factories[GetTypeEXT<T>()] = [ ] (std::string fname) {
			T::Create(fname);
		};
	}

	void IntializeFileFactories() {
		AddFileFactory<MD5Mesh>();
		AddFileFactory<OBJ>();
	}

	void IntializeIOFunctors() {
		in_functors[proto::Component::ComponentCase::kRenderable] = [ ] (const proto::Entity& entity, const proto::Component& comp) {
			auto renderable = std::make_shared<Renderable>();
			renderable->In(comp.renderable());
			Entity(entity.id()).Add(renderable);
		};

		out_functors[proto::Component::ComponentCase::kRenderable] = [ ] (proto::Entity* entity) {
			Entity e(entity->id());
			if (e.Has<Renderable>()) {
				proto::Component* comp = entity->add_components();
				e.Get<Renderable>().lock()->Out(comp->mutable_renderable());
			}
		};

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

		in_functors[proto::Component::ComponentCase::kView] = [ ] (const proto::Entity& entity, const proto::Component& comp) {
			auto view = std::make_shared<View>();
			view->In(comp.view());
			Entity(entity.id()).Add(view);
		};

		out_functors[proto::Component::ComponentCase::kView] = [ ] (proto::Entity* entity) {
			Entity e(entity->id());
			if (e.Has<View>()) {
				proto::Component* comp = entity->add_components();
				e.Get<View>().lock()->Out(comp->mutable_view());
			}
		};

		in_functors[proto::Component::ComponentCase::kScale] = [ ] (const proto::Entity& entity, const proto::Component& comp) {
			auto scale = std::make_shared<Scale>();
			const proto::Scale& proto_scale = comp.scale();
			if (proto_scale.has_x()) {
				scale->value.x = proto_scale.x();
			}
			if (proto_scale.has_y()) {
				scale->value.y = proto_scale.y();
			}
			if (proto_scale.has_z()) {
				scale->value.z = proto_scale.z();
			}
			Entity(entity.id()).Add(scale);
		};

		out_functors[proto::Component::ComponentCase::kScale] = [ ] (proto::Entity* entity) {
			Entity e(entity->id());
			if (e.Has<Scale>()) {
				proto::Scale* proto_scale = entity->add_components()->mutable_scale();
				auto scale = e.Get<Scale>().lock();

				proto_scale->set_x(scale->value.x);
				proto_scale->set_y(scale->value.y);
				proto_scale->set_z(scale->value.z);
			}
		};

		in_functors[proto::Component::ComponentCase::kCollisionBody] = [ ] (const proto::Entity& entity, const proto::Component& comp) {
			const proto::CollisionBody& body = comp.collision_body();
			eid entity_id = entity.id();
			std::shared_ptr<CollisionBody> colbody;
			switch (body.shape_case()) {
				case proto::CollisionBody::ShapeCase::kBox:
					colbody = std::make_shared<CollisionBox>(entity_id,
						body.box().x_extent(), body.box().y_extent(), body.box().z_extent());
					break;
				case proto::CollisionBody::ShapeCase::kSphere:
					colbody = std::make_shared<CollisionSphere>(entity_id, body.sphere().radius());
					break;
				case proto::CollisionBody::ShapeCase::kCapsule:
					colbody = std::make_shared<CollisionCapsule>(entity_id,
						body.capsule().height(), body.capsule().radius());
					break;

			}
			if (colbody) {
				if (body.has_disable_deactivation()) {
					colbody->disable_deactivation = body.disable_deactivation();
				}
				if (body.has_disable_rotation()) {
					colbody->disable_rotation = body.disable_rotation();
				}
				if (body.has_mass()) {
					colbody->mass = body.mass();
				}
				Entity(entity_id).Add(colbody);
			}
		};

		out_functors[proto::Component::ComponentCase::kCollisionBody] = [ ] (proto::Entity* entity) {
			Entity e(entity->id());
			if (e.Has<CollisionBody>()) {
				proto::Component* comp = entity->add_components();
				std::shared_ptr<CollisionBody> colbody = e.Get<CollisionBody>().lock();
				proto::CollisionBody* body = comp->mutable_collision_body();
				body->set_disable_deactivation(colbody->disable_deactivation);
				body->set_disable_rotation(colbody->disable_rotation);
				body->set_mass(colbody->mass);
				switch (colbody->collision_shape) {
					case COLLISION_SHAPE::BOX:
						{
							proto::CollisionBody::Box* box = body->mutable_box();
							std::shared_ptr<CollisionBox> colbox = std::static_pointer_cast<CollisionBox>(colbody);
							box->set_x_extent(static_cast<float>(colbox->half_extents.x()));
							box->set_y_extent(static_cast<float>(colbox->half_extents.y()));
							box->set_z_extent(static_cast<float>(colbox->half_extents.z()));
						}
						break;
					case COLLISION_SHAPE::SPHERE:
						{
							proto::CollisionBody::Sphere* sphere = body->mutable_sphere();
							std::shared_ptr<CollisionSphere> colsphere = std::static_pointer_cast<CollisionSphere>(colbody);
							sphere->set_radius(colsphere->radius);
						}
						break;
					case COLLISION_SHAPE::CAPSULE:
						{
							proto::CollisionBody::Capsule* capsule = body->mutable_capsule();
							std::shared_ptr<CollisionCapsule> colcapsule = std::static_pointer_cast<CollisionCapsule>(colbody);
							capsule->set_radius(colcapsule->radius);
							capsule->set_height(colcapsule->height);
						}
						break;
				}
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
			Entity voxel1(100);
			std::shared_ptr<Mesh> mesh = voxvol_shared->GetMesh().lock();
			std::shared_ptr<CollisionBody> colbody = std::make_shared<CollisionMesh>(100, mesh);
			colbody->mass = 0.0;
			voxel1.Add(colbody);
		}

		OBJ::Create("assets/vidstand/VidStand_Full.obj");
		{
			Entity bob(99);
			std::shared_ptr<MD5Mesh> mesh1 = MD5Mesh::Create("assets/bob/bob.md5mesh");
			std::shared_ptr<MD5Anim> anim1 = MD5Anim::Create("assets/bob/bob.md5anim", mesh1);
			bob.Add<Animation>(anim1);
			std::shared_ptr<VorbisStream> vorbis_stream = VorbisStream::Create("assets/theme.ogg");
			bob.Add<AudioSource>(vorbis_stream, true);
		}

		{
			Entity camera(1);
			camera.Add<Velocity>();
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
					entity_out_functors[entity_id].insert(&out_functors.at(comp.component_case()));
				}
			}
		}
	}

	void ProtoSave() {
		std::fstream output("assets/test.proto", std::ios::out | std::ios::trunc | std::ios::binary);
		proto::EntityList elist;
		for (auto entity_functors : entity_out_functors) {
			proto::Entity* entity = elist.add_entities();
			entity->set_id(entity_functors.first);
			for (auto functor : entity_functors.second) {
				(*functor)(entity);
			}
			//out_functors[proto::Component::ComponentCase::kCollisionBody](entity);
		}
		elist.SerializeToOstream(&output);
	}
}
