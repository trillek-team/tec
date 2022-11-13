#pragma once

#include "render-pass.hpp"
#include "resources/obj.hpp"

#include <memory>

namespace tec::graphics::pass {
using PointLightMap = Multiton<eid, PointLight*>;

class PointLightPass final : public RenderPass {
public:
	PointLightPass() : RenderPass("PointLightPass") {
		if (const auto sphere = OBJ::Create(Path("assets:/sphere/sphere.obj")); !sphere) {
			spdlog::get("console_log")->debug("[RenderSystem] Error loading sphere.obj.");
		}
		else {
			this->sphere_vbo.Load(sphere);
		}
	}
	void Prepare(GBuffer& gbuffer) override {
		gbuffer.BeginLightPass();
		gbuffer.BeginPointLightPass();
	}
	void Run(const gfx::ShaderSet& default_shaders, const Viewport& viewport, const View& view, const RenderItems&)
			override {
		const std::shared_ptr<Shader> def_pl_shader = ShaderMap::Get(default_shaders.pointlight());

		def_pl_shader->Use();
		glUniform3fv(def_pl_shader->GetUniformLocation("view_pos"), 1, glm::value_ptr(view.view_pos));
		glUniform4fv(def_pl_shader->GetUniformLocation("view_quat"), 1, glm::value_ptr(view.view_quat));
		glUniformMatrix4fv(
				def_pl_shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(viewport.projection));
		glUniform2f(
				def_pl_shader->GetUniformLocation("gScreenSize"), viewport.inv_view_size.x, viewport.inv_view_size.y);
		const GLint model_index = def_pl_shader->GetUniformLocation("model");
		const GLint color_index = def_pl_shader->GetUniformLocation("gPointLight.Base.Color");
		const GLint ambient_intensity_index = def_pl_shader->GetUniformLocation("gPointLight.Base.AmbientIntensity");
		const GLint diffuse_intensity_index = def_pl_shader->GetUniformLocation("gPointLight.Base.DiffuseIntensity");
		const GLint atten_constant_index = def_pl_shader->GetUniformLocation("gPointLight.Atten.Constant");
		const GLint atten_linear_index = def_pl_shader->GetUniformLocation("gPointLight.Atten.Linear");
		const GLint atten_exp_index = def_pl_shader->GetUniformLocation("gPointLight.Atten.Exp");

		const auto index_count{static_cast<GLsizei>(this->sphere_vbo.GetVertexGroup(0)->index_count)};

		glBindVertexArray(this->sphere_vbo.GetVAO());
		for (auto itr = PointLightMap::Begin(); itr != PointLightMap::End(); ++itr) {
			auto& [entity_id, light] = *itr;

			glm::vec3 position;
			if (Multiton<eid, Position*>::Has(entity_id)) {
				position = Multiton<eid, Position*>::Get(entity_id)->value;
			}

			light->UpdateBoundingRadius();
			glm::mat4 transform_matrix =
					glm::scale(glm::translate(glm::mat4(1.0), position), glm::vec3(light->bounding_radius));

			glUniformMatrix4fv(model_index, 1, GL_FALSE, glm::value_ptr(transform_matrix));
			glUniform3f(color_index, light->color.x, light->color.y, light->color.z);
			glUniform1f(ambient_intensity_index, light->ambient_intensity);
			glUniform1f(diffuse_intensity_index, light->diffuse_intensity);
			glUniform1f(atten_constant_index, light->Attenuation.constant);
			glUniform1f(atten_linear_index, light->Attenuation.linear);
			glUniform1f(atten_exp_index, light->Attenuation.exponential);
			glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
		}
		glBindVertexArray(0);
		def_pl_shader->UnUse();
	}
	void Complete(GBuffer& gbuffer) override { gbuffer.EndPointLightPass(); }

private:
	VertexBufferObject sphere_vbo{vertex::VF_BASE};
};
} // namespace tec::graphics::pass
