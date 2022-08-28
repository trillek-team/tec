#pragma once

#include <map>
#include <memory>
#include <set>
#include <unordered_set>

#include <glad/glad.h>
#include <fmt/core.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <graphics.pb.h>

#include "command-queue.hpp"
#include "event-system.hpp"
#include "graphics/gbuffer.hpp"
#include "graphics/vertex-buffer-object.hpp"
#include "tec-types.hpp"

namespace spdlog {
class logger;
}

namespace tec {
struct VertexGroup;
struct View;
class Shader;
class Animation;
class Console;

class RenderSystem;
typedef Command<RenderSystem> RenderCommand;

struct WindowResizedEvent;
struct EntityDestroyed;
struct EntityCreated;

struct RenderItem {
	glm::vec3 model_position;
	glm::vec3 model_scale;
	glm::quat model_quat;
	std::vector<VertexGroup> vertex_groups;
	std::shared_ptr<VertexBufferObject> vbo;
	MeshFile* mesh_at_set{nullptr}; // used only for equality testing, does not own an object
	bool animated{false};
	Animation* animation{nullptr};
};

class GLSymbol {
public:
	static const GLSymbol& Get(GLenum which);
	std::string name;
};

class RenderSystem :
		public CommandQueue<RenderSystem>,
		public EventQueue<WindowResizedEvent>,
		public EventQueue<EntityDestroyed>,
		public EventQueue<EntityCreated> {
public:
	void Startup();

	void RegisterConsole(Console& console);

	void SetViewportSize(const glm::uvec2& view_size);

	void Update(const double delta);

	bool HasExtension(const std::string& x) const { return extensions.find(x) != extensions.cend(); }

	static void ErrorCheck(
			const std::string_view what,
			size_t line,
			const std::string_view where = "RenderSystem",
			bool severe = false);

private:
	static std::shared_ptr<spdlog::logger> _log;

	void GeometryPass();
	void BeginPointLightPass();
	void DirectionalLightPass();
	void FinalPass();
	void RenderGbuffer();

	void ActivateMaterial(const Material&);
	void DeactivateMaterial(const Material&, GLuint*);
	void SetupDefaultShaders(const gfx::RenderConfig&);

	void On(eid, std::shared_ptr<WindowResizedEvent> data) override;
	void On(eid, std::shared_ptr<EntityDestroyed> data) override;
	void On(eid, std::shared_ptr<EntityCreated> data) override;
	void UpdateRenderList(double delta);

	glm::mat4 projection{0};
	View* current_view{nullptr};
	glm::uvec2 view_size{1024, 768};
	glm::vec2 inv_view_size;
	std::shared_ptr<Shader> default_shader;
	gfx::RenderOptions options;
	gfx::ShaderSet active_shaders;

	GBuffer light_gbuffer;
	VertexBufferObject sphere_vbo{vertex::VF_BASE}; // Used for rendering point lights.
	VertexBufferObject quad_vbo{vertex::VF_BASE}; // Used for rendering directional lights.

	// all the functional extensions this GL supports
	std::unordered_set<std::string> extensions;
	std::map<std::shared_ptr<MeshFile>, std::shared_ptr<VertexBufferObject>> mesh_buffers;
	std::map<std::shared_ptr<Shader>, std::set<RenderItem*>> render_item_list;
};
} // namespace tec

template <> struct fmt::formatter<tec::GLSymbol> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		auto it = ctx.begin(), end = ctx.end();
		if (it != end && *it != '}')
			static_assert("invalid format");
		return it;
	}
	template <typename FormatContext> auto format(const tec::GLSymbol& p, FormatContext& ctx) -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{}", p.name);
	}
};
