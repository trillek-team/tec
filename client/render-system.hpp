#pragma once

#include <glad/glad.h>
#include <graphics.pb.h>
#include <memory>
#include <unordered_set>

#include "command-queue.hpp"
#include "event-system.hpp"
#include "graphics/gbuffer.hpp"
#include "graphics/passes/render-pass.hpp"
#include "graphics/render-list.hpp"
#include "graphics/vertex-buffer-object.hpp"
#include "tec-types.hpp"

namespace spdlog {
class logger;
}

namespace tec {
class Console;

class RenderSystem;
typedef Command<RenderSystem> RenderCommand;

struct WindowResizedEvent;
struct EntityDestroyed;
struct EntityCreated;

class RenderSystem final :
		public CommandQueue<RenderSystem>,
		public EventQueue<WindowResizedEvent>,
		public EventQueue<EntityDestroyed>,
		public EventQueue<EntityCreated> {
public:
	void Startup();

	void RegisterConsole(Console& console);

	void SetViewportSize(const glm::uvec2& view_size);

	void Update(double delta);

	bool HasExtension(const std::string& x) const { return extensions.find(x) != extensions.cend(); }

	static void ErrorCheck(std::string_view what, size_t line, std::string_view where = "RenderSystem");

	static void ActivateMaterial(const Material&);

	static void DeactivateMaterial(const Material&, const GLuint*);

	inline static VertexBufferObject quad_vbo{vertex::VF_BASE};

private:
	static std::shared_ptr<spdlog::logger> _log;
	
	static void SetupDefaultShaders(const gfx::RenderConfig&);

	void On(eid, std::shared_ptr<WindowResizedEvent> data) override;
	void On(eid, std::shared_ptr<EntityDestroyed> data) override;
	void On(eid, std::shared_ptr<EntityCreated> data) override;

	gfx::RenderOptions options;
	gfx::ShaderSet active_shaders;

	GBuffer light_gbuffer;

	// all the functional extensions this GL supports
	std::unordered_set<std::string> extensions;
	graphics::RenderItemList render_item_list;
	std::vector<std::shared_ptr<graphics::pass::RenderPass>> render_passes;
};
} // namespace tec
