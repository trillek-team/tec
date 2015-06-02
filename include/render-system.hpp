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

namespace tec {
	class VertexBufferObject;
	class Shader;

	class RenderSystem;
	typedef Command<RenderSystem> RenderCommand;

	struct KeyboardEvent;
	struct WindowResizedEvent;

	struct Renderable {
		Renderable(std::shared_ptr<VertexBufferObject> buf) :
			buffer(buf) {

		}
		Renderable() {

		}
		bool hidden = false;
		std::set<size_t> vertex_groups;
		std::shared_ptr<VertexBufferObject> buffer;

	};

	struct View {
		glm::mat4 view_matrix;
		bool active = false;
	};

	class RenderSystem : public CommandQueue < RenderSystem >,
		public EventQueue < KeyboardEvent >,
		public EventQueue < WindowResizedEvent > {
	public:
		RenderSystem();

		void SetViewportSize(const unsigned int width, const unsigned int height);

		void Update(const double delta);

		bool ActivateView(const eid entity_id);
	private:
		typedef Multiton<eid, std::shared_ptr<Renderable>> RenderableComponentMap;

		void On(std::shared_ptr<WindowResizedEvent> data);

		glm::mat4 projection;
		std::weak_ptr<View> current_view;
		unsigned int window_width, window_height;
		std::map<eid, glm::mat4> model_matricies;
		// TODO: Make this into a data structure or organized better?
		std::map<std::shared_ptr<Shader>, std::map<std::shared_ptr<VertexBufferObject>, std::set<eid>>> render_list;
	};
}
