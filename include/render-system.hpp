#pragma once

#include <memory>
#include <list>
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

namespace vv {
	struct VertexBuffer;
	class Material;

	class RenderSystem;
	typedef Command<RenderSystem> RenderCommand;

	struct KeyboardEvent;

	struct Renderable {
		glm::mat4 model_matrix;
	};

	struct View {
		glm::mat4 view_matrix;
		bool active = false;
	};

	typedef Multiton<eid, std::shared_ptr<Renderable>> RenderableMap;

	class RenderSystem : public CommandQueue < RenderSystem >, public EventQueue < KeyboardEvent > {
	public:
		RenderSystem();

		void SetViewportSize(const unsigned int width, const unsigned int height);

		void Update(const double delta);

		void AddVertexBuffer(const std::weak_ptr<Material> mat, const std::weak_ptr<VertexBuffer> buffer, const eid entity_id);

		// Checks if there is a view associated entity_id and sets it as the current view.
		bool ActivateView(const eid entity_id);
	protected:
		//void CreateVertexBuffer(eid entity_id, const std::vector<Vertex>& verts, const std::vector<GLuint>& indices);
	private:
		glm::mat4 projection;
		std::weak_ptr<View> current_view;
		unsigned int window_width, window_height;
		std::map < std::weak_ptr<Material>, std::pair < std::weak_ptr<VertexBuffer>,
			std::list<eid >> , std::owner_less<std::weak_ptr<Material>>> buffers;
	};
}
