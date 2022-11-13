#pragma once

#include "render-pass.hpp"

#include <memory>

namespace tec::graphics::pass {
using DirectionalLightMap = Multiton<eid, DirectionalLight*>;

class DirLightPass final : public RenderPass {
public:
	DirLightPass() : RenderPass("DirLightPass") {}
	void Prepare(GBuffer& gbuffer) override { gbuffer.BeginDirLightPass(); }
	void Run(const gfx::ShaderSet& shaders, const Viewport& viewport, const View& view, const RenderItems&) override {
		const std::shared_ptr<Shader> def_dl_shader = ShaderMap::Get(shaders.dirlight());
		def_dl_shader->Use();

		glUniform2f(
				def_dl_shader->GetUniformLocation("gScreenSize"), viewport.inv_view_size.x, viewport.inv_view_size.y);
		glUniform3fv(def_dl_shader->GetUniformLocation("view_pos"), 1, glm::value_ptr(view.view_pos));
		const GLint color_index = def_dl_shader->GetUniformLocation("gDirectionalLight.Base.Color");
		const GLint ambient_intensity_index =
				def_dl_shader->GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
		const GLint diffuse_intensity_index =
				def_dl_shader->GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
		const GLint direction_index = def_dl_shader->GetUniformLocation("gDirectionalLight.Direction");

		const auto& quad_vbo = RenderSystem::quad_vbo;

		glBindVertexArray(quad_vbo->GetVAO());

		const auto index_count{static_cast<GLsizei>(quad_vbo->GetVertexGroupIndexCount(0))};

		for (auto itr = DirectionalLightMap::Begin(); itr != DirectionalLightMap::End(); ++itr) {
			const DirectionalLight* light = itr->second;

			glUniform3f(color_index, light->color.x, light->color.y, light->color.z);
			glUniform1f(ambient_intensity_index, light->ambient_intensity);
			glUniform1f(diffuse_intensity_index, light->diffuse_intensity);
			glUniform3f(direction_index, light->direction.x, light->direction.y, light->direction.z);

			glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
		}
		def_dl_shader->UnUse();
		glBindVertexArray(0);
	}
	void Complete(GBuffer& gbuffer) override { gbuffer.EndPointLightPass(); }
};
} // namespace tec::graphics::pass
