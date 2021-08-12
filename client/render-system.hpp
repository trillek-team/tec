#pragma once

#include <glm/mat4x4.hpp>
#include <map>
#include <memory>
#include <set>
#include <unordered_set>

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

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

class RenderSystem;
typedef Command<RenderSystem> RenderCommand;

struct WindowResizedEvent;
struct EntityDestroyed;
struct EntityCreated;

struct RenderItem {
	glm::mat4 model_matrix;
	std::vector<VertexGroup> vertex_groups;
	std::shared_ptr<VertexBufferObject> vbo;
	MeshFile* mesh_at_set{nullptr}; // used only for equality testing, does not own an object
	bool animated{false};
	Animation* animation{nullptr};
};

class RenderSystem :
		public CommandQueue<RenderSystem>,
		public EventQueue<WindowResizedEvent>,
		public EventQueue<EntityDestroyed>,
		public EventQueue<EntityCreated> {
public:
	void Startup();

	void SetViewportSize(const unsigned int width, const unsigned int height);

	void Update(const double delta);

	bool HasExtension(const std::string& x) const { return extensions.find(x) != extensions.cend(); }

private:
	std::shared_ptr<spdlog::logger> _log;

	void GeometryPass();
	void BeginPointLightPass();
	void DirectionalLightPass();
	void FinalPass();
	void RenderGbuffer();

	void SetupDefaultShaders();

	using EventQueue<WindowResizedEvent>::On;
	using EventQueue<EntityDestroyed>::On;
	using EventQueue<EntityCreated>::On;
	void On(std::shared_ptr<WindowResizedEvent> data);
	void On(std::shared_ptr<EntityDestroyed> data);
	void On(std::shared_ptr<EntityCreated> data);
	void UpdateRenderList(double delta);

	glm::mat4 projection{0};
	View* current_view{nullptr};
	glm::uvec2 view_size{1024, 768};
	glm::vec2 inv_view_size;
	std::shared_ptr<Shader> default_shader;

	GBuffer light_gbuffer;
	VertexBufferObject sphere_vbo{vertex::VF_BASE}; // Used for rendering point lights.
	VertexBufferObject quad_vbo{vertex::VF_BASE}; // Used for rendering directional lights.

	// all the functional extensions this GL supports
	std::unordered_set<std::string> extensions;
	std::map<std::shared_ptr<MeshFile>, std::shared_ptr<VertexBufferObject>> mesh_buffers;
	std::map<std::shared_ptr<Shader>, std::set<RenderItem*>> render_item_list;
};
} // namespace tec
