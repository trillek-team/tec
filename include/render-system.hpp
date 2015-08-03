#pragma once

#include <memory>
#include <set>
#include <map>
#include <glm/mat4x4.hpp>

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#include "multiton.hpp"
#include "types.hpp"
#include "event-system.hpp"
#include "command-queue.hpp"
#include "graphics/animation.hpp"
#include "graphics/gbuffer.hpp"
#include "graphics/vertex-buffer-object.hpp"

namespace tec {
	struct VertexGroup;
	struct Renderable;
	struct PointLight;
	struct DirectionalLight;
	struct View;
	class Shader;

	class RenderSystem;
	typedef Command<RenderSystem> RenderCommand;

	struct KeyboardEvent;
	struct WindowResizedEvent;

	class RenderSystem : public CommandQueue < RenderSystem >,
		public EventQueue < KeyboardEvent >,
		public EventQueue < WindowResizedEvent > {
	public:
		RenderSystem();

		void SetViewportSize(const unsigned int width, const unsigned int height);

		void Update(const double delta);

		void RenderGbuffer();

		void RenderGeometryPass();

		void StencilPass(glm::mat4 scale_matrix);

		void PointLightPass();

		void DirectionalLightPass();
		
		void FinalPass();

		bool ActivateView(const eid entity_id);
	private:
		typedef Multiton<eid, std::shared_ptr<Renderable>> RenderableComponentMap;
		typedef Multiton<eid, std::shared_ptr<PointLight>> PointLightMap;
		typedef Multiton<eid, std::shared_ptr<DirectionalLight>> DirectionalLightMap;

		void On(std::shared_ptr<WindowResizedEvent> data);

		glm::mat4 projection;
		std::weak_ptr<View> current_view;
		unsigned int window_width, window_height;
		std::map<eid, glm::mat4> model_matricies;
		std::shared_ptr<Shader> default_shader;

		GBuffer gbuffer;
		VertexBufferObject sphere_vbo; // Used for rendering point lights.
		VertexBufferObject quad_vbo; // Used for rendering directional lights.

		struct RenderItem {
			glm::mat4* model_matrix;
			std::set<VertexGroup*>* vertex_groups;
			GLuint vao, ibo;
			bool animated = false;
			std::shared_ptr<Animation> animation;

			friend bool operator<(const RenderItem& a, const RenderItem& b) {
				return a.vao < b.vao;
			}
		};
		std::map<std::shared_ptr<Shader>, std::set<RenderItem>> render_item_list;
	};
}
