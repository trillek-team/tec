#include "render-system.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <thread>
#include <cmath>

#include "graphics/shader.hpp"
#include "graphics/view.hpp"
#include "graphics/vertex-buffer-object.hpp"
#include "graphics/material.hpp"
#include "components/transforms.hpp"
#include "components/renderable.hpp"
#include "entity.hpp"
#include "os.hpp"

namespace tec {
	RenderSystem::RenderSystem() : window_width(800), window_height(600) {
		auto err = glGetError();
		// If there is an error that means something went wrong when creating the context.
		if (err) {
			return;
		}

		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
	}

	void RenderSystem::SetViewportSize(const unsigned int width, const unsigned int height) {
		this->window_height = height;
		this->window_width = width;

		float aspect_ratio = static_cast<float>(this->window_width) / static_cast<float>(this->window_height);
		if ((aspect_ratio < 1.0f) || std::isnan(aspect_ratio)) {
			aspect_ratio = 4.0f / 3.0f;
		}

		this->projection = glm::perspective(
			glm::radians(45.0f),
			aspect_ratio,
			0.1f,
			10000.0f
			);
	}

	void RenderSystem::Update(const double delta) {
		ProcessCommandQueue();
		EventQueue<WindowResizedEvent>::ProcessEventQueue();
		this->render_item_list.clear();

		if (!this->default_shader) {
			this->default_shader = ShaderMap::Get("debug");
		}

		// Loop through each renderbale and update its model matrix.
		for (auto itr = RenderableComponentMap::Begin(); itr != RenderableComponentMap::End(); ++itr) {
			auto entity_id = itr->first;
			std::shared_ptr<Renderable> renderable = itr->second;
			if (renderable->hidden) {
				continue;
			}
			glm::vec3 position;
			glm::quat orientation;
			glm::vec3 scale(1.0);
			Entity e(entity_id);
			if (e.Has<Position>()) {
				position = e.Get<Position>().lock()->value;
			}
			if (e.Has<Orientation>()) {
				orientation = e.Get<Orientation>().lock()->value;
			}
			if (e.Has<Scale>()) {
				scale = e.Get<Scale>().lock()->value;
			}
			auto camera_translation = position;
			auto camera_orientation = orientation;

			this->model_matricies[entity_id] = glm::scale(glm::translate(glm::mat4(1.0), camera_translation) *
				glm::mat4_cast(camera_orientation), scale);
			if (!renderable->buffer) {
				renderable->buffer = std::make_shared<VertexBufferObject>();
				renderable->buffer->Load(renderable->mesh);
				size_t group_count = renderable->buffer->GetVertexGroupCount();
				for (size_t i = 0; i < group_count; ++i) {
					renderable->vertex_groups.insert(renderable->buffer->GetVertexGroup(i));
				}
			}

			RenderItem ri;
			ri.model_matrix = &this->model_matricies[entity_id];
			ri.vao = renderable->buffer->GetVAO();
			ri.ibo = renderable->buffer->GetIBO();
			ri.vertex_groups = &renderable->vertex_groups;

			if (e.Has<Animation>()) {
				auto anim = e.Get<Animation>().lock();
				anim->UpdateAnimation(delta);
				if (anim->animation_matrices.size() > 0) {
					ri.animated = true;
					ri.animation = anim;
				}
			}

			std::shared_ptr<Shader> shader = renderable->shader;
			if (!shader) {
				shader = this->default_shader;
			}
			for (auto group : renderable->vertex_groups) {
				this->render_item_list[shader].insert(std::move(ri));
			}
		}

		for (auto itr = Multiton<eid, std::shared_ptr<View>>::Begin();
			itr != Multiton<eid, std::shared_ptr<View>>::End(); ++itr) {
			auto entity_id = itr->first;
			std::shared_ptr<View> view = itr->second;

			glm::vec3 position;
			glm::quat orientation;
			glm::vec3 scale(1.0);
			Entity e(entity_id);
			if (e.Has<Position>()) {
				position = e.Get<Position>().lock()->value;
			}
			if (e.Has<Orientation>()) {
				orientation = e.Get<Orientation>().lock()->value;
			}
			if (e.Has<Scale>()) {
				scale = e.Get<Scale>().lock()->value;
			}
			auto camera_translation = position;
			auto camera_orientation = orientation;

			this->model_matricies[entity_id] = glm::scale(glm::translate(glm::mat4(1.0), camera_translation) *
				glm::mat4_cast(camera_orientation), scale);

			view->view_matrix = glm::inverse(this->model_matricies[entity_id]);
			if (view->active) {
				this->current_view = view;
			}
		}
		static float red = 0.3f, blue = 0.3f, green = 0.3f;

		glClearColor(red, green, blue, 1.0f);
		glViewport(0, 0, this->window_width, this->window_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 camera_matrix(1.0);
		auto view = this->current_view.lock();
		if (view) {
			camera_matrix = view->view_matrix;
		}

		for (auto shader_list : this->render_item_list) {
			auto shader = shader_list.first;
			if (!shader) {
				continue;
			}
			shader->Use();

			glUniformMatrix4fv(shader->GetUniformLocation("view"), 1, GL_FALSE, &camera_matrix[0][0]);
			glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, GL_FALSE, &this->projection[0][0]);
			GLint animatrix_loc = shader->GetUniformLocation("animation_matrix");
			GLint animated_loc = shader->GetUniformLocation("animated");
			GLint model_index = shader->GetUniformLocation("model");

			for (auto render_item : shader_list.second) {
				glBindVertexArray(render_item.vao);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_item.ibo);
				glUniform1i(animated_loc, 0);
				if (render_item.animated) {
					glUniform1i(animated_loc, 1);
					auto& animmatricies = render_item.animation->animation_matrices;
					glUniformMatrix4fv(animatrix_loc, animmatricies.size(), GL_FALSE, glm::value_ptr(animmatricies[0]));
				}
				for (auto vertex_group : *render_item.vertex_groups) {
					glPolygonMode(GL_FRONT_AND_BACK, vertex_group->material->GetPolygonMode());
					vertex_group->material->Activate();
					glUniformMatrix4fv(model_index, 1, GL_FALSE, glm::value_ptr(*render_item.model_matrix));
					glDrawElements(vertex_group->material->GetDrawElementsMode(), vertex_group->index_count, GL_UNSIGNED_INT, (GLvoid*)(vertex_group->starting_offset * sizeof(GLuint)));
					vertex_group->material->Deactivate();
				}
			}
			shader->UnUse();
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	bool RenderSystem::ActivateView(const eid entity_id) {
		if (Entity(entity_id).Has<View>()) {
			this->current_view = Entity(entity_id).Get<View>();;
			return true;
		}
		return false;
	}

	void RenderSystem::On(std::shared_ptr<WindowResizedEvent> data) {
		SetViewportSize(data->new_width, data->new_height);
	}
}
