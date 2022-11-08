#pragma once

#include "graphics/gbuffer.hpp"
#include "graphics/render-list.hpp"
#include <glm/ext.hpp>
#include <optional>

namespace tec::graphics::pass {

class RenderPass {
protected:
	explicit RenderPass(std::string _pass_name) : pass_name(std::move(_pass_name)) {}
public:
	RenderPass(const RenderPass& other) = default;

	RenderPass(RenderPass&& other) noexcept : inv_view_size(other.inv_view_size), projection(other.projection) {}

	RenderPass& operator=(const RenderPass& other) {
		if (this == &other)
			return *this;
		inv_view_size = other.inv_view_size;
		projection = other.projection;
		pass_name = other.pass_name;
		return *this;
	}

	RenderPass& operator=(RenderPass&& other) noexcept {
		if (this == &other)
			return *this;
		inv_view_size = other.inv_view_size;
		projection = other.projection;
		pass_name = other.pass_name;
		return *this;
	}

	virtual ~RenderPass() = default;

	virtual void Prepare(const GBuffer&) = 0;
	virtual void Run(const gfx::ShaderSet&, const View&, const RenderItems&) = 0;
	virtual void Complete(const GBuffer&) = 0;

	void SetProjection(const glm::mat4 _projection) { this->projection = _projection; }
	void SetInvViewSize(const glm::vec2 _inv_view_size) { this->inv_view_size = _inv_view_size; }

	std::string_view GetPassName() { return this->pass_name; }

protected:
	glm::vec2 inv_view_size{};
	glm::mat4 projection{0};
	std::string pass_name{"BaseRenderBase"};
};
} // namespace tec::graphics::pass
