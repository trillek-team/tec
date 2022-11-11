#pragma once

#include "render-pass.hpp"
#include "resources/obj.hpp"

#include <memory>

namespace tec::graphics::pass {

class DebugPass final : public RenderPass {
public:
	DebugPass() : RenderPass("DebugPass") {}
	void Prepare(const GBuffer& gbuffer) override {
		gbuffer.BindForRendering();
		glActiveTexture(GL_TEXTURE0 + static_cast<int>(GBuffer::DEPTH_TYPE::DEPTH));
		glBindSampler(static_cast<int>(GBuffer::DEPTH_TYPE::DEPTH), 0);
		glBindTexture(GL_TEXTURE_2D, gbuffer.GetDepthTexture());
	}
	void Run(const gfx::ShaderSet& default_shaders, const View&, const RenderItems&) override {
		glDisable(GL_BLEND);
		glDrawBuffer(GL_BACK);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		const auto& quad_vbo = RenderSystem::quad_vbo;

		glBindVertexArray(quad_vbo->GetVAO());

		const std::shared_ptr<Shader> def_db_shader = ShaderMap::Get(default_shaders.gbufdebug());
		def_db_shader->Use();

		glUniform2f(def_db_shader->GetUniformLocation("gScreenSize"), this->inv_view_size.x, this->inv_view_size.y);

		const auto index_count{static_cast<GLsizei>(quad_vbo->GetVertexGroupIndexCount(0))};
		glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);

		def_db_shader->UnUse();
		glBindVertexArray(0);
	}
	void Complete(const GBuffer&) override {}
};
} // namespace tec::graphics::pass
