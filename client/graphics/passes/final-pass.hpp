#pragma once

#include "render-pass.hpp"

#include <memory>

namespace tec::graphics::pass {

class FinalPass final : public RenderPass {
public:
	FinalPass() : RenderPass("FinalPass") {}
	void Prepare(GBuffer& gbuffer) override { gbuffer.FinalPass(); }
	void
	Run(const gfx::ShaderSet& default_shaders, const Viewport& viewport, const View&, const RenderItems&) override {
		const std::shared_ptr<Shader> post_shader = ShaderMap::Get(default_shaders.postprocess());

		post_shader->Use();
		glUniform2f(post_shader->GetUniformLocation("gScreenSize"), viewport.inv_view_size.x, viewport.inv_view_size.y);

		const auto& quad_vbo = RenderSystem::quad_vbo;

		glBindVertexArray(quad_vbo->GetVAO());
		const auto index_count{static_cast<GLsizei>(quad_vbo->GetVertexGroupIndexCount(0))};
		glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		post_shader->UnUse();
	}
	void Complete(GBuffer&) override {}
};
} // namespace tec::graphics::pass
