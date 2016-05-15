// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

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

#include "types.hpp"
#include "game-state.hpp"
#include "event-system.hpp"
#include "command-queue.hpp"
#include "graphics/gbuffer.hpp"
#include "graphics/vertex-buffer-object.hpp"

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

	class RenderSystem : public CommandQueue < RenderSystem >,
		public EventQueue < WindowResizedEvent >, public EventQueue < EntityDestroyed > {
	public:
		RenderSystem();

		void SetViewportSize(const unsigned int width, const unsigned int height);

		void Update(const double delta, const GameState& state);
	private:
		std::shared_ptr<spdlog::logger> _log;

		void ShadowPass();
		void GeometryPass();
		void PointLightPass();
		void DirectionalLightPass();
		void FinalPass();
		void RenderGbuffer();

		void On(std::shared_ptr<WindowResizedEvent> data);
		void On(std::shared_ptr<EntityDestroyed> data);
		void UpdateRenderList(double delta, const GameState& state);

		glm::mat4 projection;
		View* current_view;
		unsigned int window_width, window_height;
		std::map<eid, glm::mat4> model_matricies;
		std::shared_ptr<Shader> default_shader;

		GBuffer light_gbuffer;
		GBuffer shadow_gbuffer;
		VertexBufferObject sphere_vbo; // Used for rendering point lights.
		VertexBufferObject quad_vbo; // Used for rendering directional lights.

		struct RenderItem {
			glm::mat4* model_matrix;
			std::set<VertexGroup*>* vertex_groups;
			GLuint vao, ibo;
			bool animated = false;
			Animation* animation;

			friend bool operator<(const RenderItem& a, const RenderItem& b) {
				return a.vao < b.vao;
			}
		};
		std::map<std::shared_ptr<Shader>, std::set<RenderItem>> render_item_list;
	};
}
