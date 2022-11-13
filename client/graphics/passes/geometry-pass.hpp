#pragma once

#include "graphics/texture-object.hpp"
#include "render-pass.hpp"

#include <memory>

namespace tec::graphics::pass {
struct UniformLocations {
	GLint model_pos;
	GLint model_scale;
	GLint model_quat;
	GLint bones;
	GLint animated;
	GLint vertex_group;
};

class GeometryPass final : public RenderPass {
public:
	GeometryPass() : RenderPass("GeometryPass") {}
	void Prepare(GBuffer& gbuffer) override { gbuffer.BeginGeometryPass(); }
	void Run(const gfx::ShaderSet& shaders, const Viewport& viewport, const View& view, const RenderItems& render_items)
			override {
		const GLuint def_textures[]{TextureMap::Get("default")->GetID(), TextureMap::Get("default_sp")->GetID()};

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, def_textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, def_textures[1]);

		const std::shared_ptr<Shader> def_shader = ShaderMap::Get(shaders.visual());
		def_shader->Use();

		glUniform3fv(def_shader->GetUniformLocation("view_pos"), 1, glm::value_ptr(view.view_pos));
		glUniform4fv(def_shader->GetUniformLocation("view_quat"), 1, glm::value_ptr(view.view_quat));
		glUniformMatrix4fv(
				def_shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(viewport.projection));
		const UniformLocations default_shader_locations{
				def_shader->GetUniformLocation("model_position"),
				def_shader->GetUniformLocation("model_scale"),
				def_shader->GetUniformLocation("model_quat"),
				def_shader->GetUniformLocation("animated"),
				def_shader->GetUniformLocation("animation_matrix"),
				-1};

		for (auto [_shader, _render_item] : render_items) {
			auto [model_pos, model_scale, model_quat, bones, animated, vertex_group] = default_shader_locations;
			// Check if we need to use a specific shader and set it up.
			if (_shader) {
				def_shader->UnUse();
				_shader->Use();
				glUniform3fv(_shader->GetUniformLocation("view_pos"), 1, glm::value_ptr(view.view_pos));
				glUniform4fv(_shader->GetUniformLocation("view_quat"), 1, glm::value_ptr(view.view_quat));
				glUniformMatrix4fv(
						_shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(viewport.projection));
				model_pos = _shader->GetUniformLocation("model_position");
				model_scale = _shader->GetUniformLocation("model_scale");
				model_quat = _shader->GetUniformLocation("model_quat");
				bones = _shader->GetUniformLocation("animation_bones[0]");
				animated = _shader->GetUniformLocation("animated");
				vertex_group = _shader->GetUniformLocation("vertex_group");
			}
			for (const auto render_item : _render_item) {
				glBindVertexArray(render_item->vbo->GetVAO());
				glUniform1i(animated, render_item->animated ? 1 : 0);

				if (render_item->animated && bones != -1) {
					auto& bone_transforms = render_item->animation->GetBoneTransforms();
					glUniformMatrix3x4fv(
							bones,
							static_cast<int>(bone_transforms.size()),
							GL_FALSE,
							glm::value_ptr(bone_transforms[0].orientation));
				}
				GLint vertex_group_number = 0;
				for (auto& [material, mesh_group_number, index_count, starting_offset] : render_item->vertex_groups) {
					glPolygonMode(GL_FRONT_AND_BACK, material->GetPolygonMode());
					RenderSystem::ActivateMaterial(*material);
					if (vertex_group != -1) {
						glUniform1i(vertex_group, vertex_group_number++);
					}
					//glUniformMatrix4fv(model_index, 1, GL_FALSE, glm::value_ptr(render_item->model_matrix));
					glUniform3fv(model_pos, 1, glm::value_ptr(render_item->model_position));
					glUniform3fv(model_scale, 1, glm::value_ptr(render_item->model_scale));
					glUniform4fv(model_quat, 1, glm::value_ptr(render_item->model_quat));
					glDrawElements(
							material->GetDrawElementsMode(),
							static_cast<GLsizei>(index_count),
							GL_UNSIGNED_INT,
							reinterpret_cast<GLvoid*>(starting_offset * sizeof(GLuint)));
					RenderSystem::DeactivateMaterial(*material, def_textures);
				}
			}
			// If we used a special shader set things back to the deferred shader.
			if (_shader) {
				_shader->UnUse();
				def_shader->Use();
			}
		}
		def_shader->UnUse();
		glBindVertexArray(0);
	}
	void Complete(GBuffer& gbuffer) override { gbuffer.EndGeometryPass(); }
};
} // namespace tec::graphics::pass
