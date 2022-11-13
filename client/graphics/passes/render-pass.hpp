#pragma once

#include "graphics/gbuffer.hpp"
#include "graphics/render-list.hpp"
#include "graphics/viewport.hpp"

namespace tec::graphics::pass {

class RenderPass {
protected:
	explicit RenderPass(const std::string_view _pass_name) : pass_name(_pass_name) {}

public:
	RenderPass(const RenderPass& other) = default;

	RenderPass(RenderPass&& other) noexcept : pass_name(other.pass_name) {}

	RenderPass& operator=(const RenderPass& other) {
		if (this == &other)
			return *this;
		pass_name = other.pass_name;
		return *this;
	}

	RenderPass& operator=(RenderPass&& other) noexcept {
		if (this == &other)
			return *this;
		pass_name = other.pass_name;
		return *this;
	}

	virtual ~RenderPass() = default;

	virtual void Prepare(GBuffer&) = 0;
	virtual void Run(const gfx::ShaderSet&, const Viewport&, const View&, const RenderItems&) = 0;
	virtual void Complete(GBuffer&) = 0;

	[[nodiscard]] std::string_view GetPassName() const { return this->pass_name; }

protected:
	std::string_view pass_name{"BaseRenderBase"};
};
} // namespace tec::graphics::pass
