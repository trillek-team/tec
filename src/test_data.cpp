#include "graphics/vertex-buffer-object.hpp"
#include "graphics/shader.hpp"
#include "voxelvolume.hpp"
#include "components/transforms.hpp"
#include "graphics/material.hpp"
#include "entity.hpp"
#include "components/camera.hpp"
#include "component-update-system.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "resources/md5mesh.hpp"
#include "resources/pixel-buffer.hpp"
#include "graphics/texture-object.hpp"
#include "render-system.hpp"
#include "resources/md5anim.hpp"

namespace tec {
	void IntializeComponents() {
		ComponentUpdateSystem<Position>::Initialize();
		ComponentUpdateSystem<Orientation>::Initialize();
		ComponentUpdateSystem<Camera>::Initialize();
		ComponentUpdateSystem<Renderable>::Initialize();
		ComponentUpdateSystem<View>::Initialize();
		ComponentUpdateSystem<Animation>::Initialize();
	}

	void BuildTestEntities() {
		auto shader_files = std::list < std::pair<Shader::ShaderType, std::string> > {
			std::make_pair(Shader::VERTEX, "assets/basic.vert"), std::make_pair(Shader::FRAGMENT, "assets/basic.frag"),
		};
		auto s = Shader::CreateFromFile("shader1", shader_files);
		auto basic_fill = Material::Create("material_basic", s);

		shader_files = std::list < std::pair<Shader::ShaderType, std::string> > {
			std::make_pair(Shader::VERTEX, "assets/basic.vert"), std::make_pair(Shader::FRAGMENT, "assets/overlay.frag"),
		};
		auto s_overlay = Shader::CreateFromFile("shader_overlay", shader_files);
		auto overlay = Material::Create("material_overlay", s_overlay);
		overlay->SetPolygonMode(GL_LINE);

		auto voxvol = VoxelVolume::Create(100, "bob", 0);
		auto voxvol_shared = voxvol.lock();
		Entity voxel1(100);
		voxel1.Add<Position>();
		voxel1.Add<Orientation>();

		VoxelCommand add_voxel(
			[ ] (VoxelVolume* vox_vol) {
			vox_vol->AddVoxel(0, 1, 1);
			vox_vol->AddVoxel(0, -1, 1);
			vox_vol->AddVoxel(0, -1, 0);
			vox_vol->AddVoxel(0, -1, -1);
			vox_vol->AddVoxel(1, -1, 1);
		});
		VoxelVolume::QueueCommand(std::move(add_voxel));
		voxvol_shared->Update(0.0);
		auto voxvol_vert_buffer = std::make_shared<VertexBufferObject>();
		voxel1.Add<Renderable>(voxvol_vert_buffer);

		RenderCommand buffer_func([voxvol_vert_buffer, voxvol_shared, s] (RenderSystem* sys) {
			auto mesh = voxvol_shared->GetMesh().lock();
			voxvol_vert_buffer->Load(mesh, s);
			auto voxel1Renderable = Entity(100).Get<Renderable>().lock();
			for (size_t i = 0; i < voxvol_vert_buffer->GetVertexGroupCount(); ++i) {
				voxel1Renderable->vertex_groups.insert(voxvol_vert_buffer->GetVertexGroup(i));
			}
		});
		RenderSystem::QueueCommand(std::move(buffer_func));

		std::shared_ptr<MD5Mesh> mesh1 = std::make_shared<MD5Mesh>();
		mesh1->Load("assets/bob/bob.md5mesh");
		{
			auto renderable = std::make_shared<Renderable>(std::make_shared<VertexBufferObject>());
			renderable->buffer->Load(mesh1, s);
			for (size_t i = 0; i < renderable->buffer->GetVertexGroupCount(); ++i) {
				renderable->vertex_groups.insert(renderable->buffer->GetVertexGroup(i));
			}
			Entity(99).Add<Renderable>(renderable);
		}

		std::shared_ptr<MD5Anim> anim1 = std::make_shared<MD5Anim>();
		anim1->Load("assets/bob/bob.md5anim");
		anim1->CheckMesh(mesh1);
		Entity(99).Add<Animation>(anim1);
		Entity(99).Add<Position>(glm::vec3(0.0,-2.0,-5.0));
		Entity(99).Add<Orientation>(glm::vec3(glm::radians(-90.0),0.0,0.0));

		Entity camera(1);
		camera.Add<Position>();
		camera.Add<Orientation>();
		camera.Add<Camera>(1);
		camera.Add<Renderable>(voxvol_vert_buffer);
		Entity camera2(2);
		camera2.Add<Position>();
		camera2.Add<Orientation>();
		camera2.Add<Camera>(2);
	}
}
