#include "render-system.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <thread>

#include "shader.hpp"
#include "vertexbuffer.hpp"
#include "components/transforms.hpp"
#include "material.hpp"
#include "entity.hpp"
#include "os.hpp"

namespace tec {
	class Texture {
	public:
		GLuint name;
	};

	typedef Multiton<std::string, std::shared_ptr<Texture>> TextureMap;

	RenderSystem::RenderSystem() : window_width(800), window_height(600) {
		auto err = glGetError();
		// If there is an error that means something went wrong when creating the context.
		if (err) {
			return;
		}

		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void RenderSystem::SetViewportSize(const unsigned int width, const unsigned int height) {
		this->window_height = height;
		this->window_width = width;

		float aspect_ratio = static_cast<float>(this->window_width) / static_cast<float>(this->window_height);
		if (aspect_ratio < 1.0f) {
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

		// Loop through each renderbale and update its model matrix.
		for (auto itr = RenderableComponentMap::Begin(); itr != RenderableComponentMap::End(); ++itr) {
			auto entity_id = itr->first;
			glm::vec3 position;
			glm::quat orientation;
			Entity e(entity_id);
			if (e.Has<Position>()) {
				position = e.Get<Position>().lock()->value;
			}
			if (e.Has<Orientation>()) {
				orientation = e.Get<Orientation>().lock()->value;
			}
			auto camera_translation = position;
			auto camera_orientation = orientation;

			this->model_matricies[entity_id] = glm::translate(glm::mat4(1.0), camera_translation) *
				glm::mat4_cast(camera_orientation);

			// Check if there is a view associated with the entity_id and update it as well.
			if (e.Has<View>()) {
				auto view = e.Get<View>().lock();
				view->view_matrix = glm::inverse(this->model_matricies[entity_id]);
				if (view->active) {
					this->current_view = view;
				}
			}

			this->render_list[itr->second->material][itr->second->buffer].push_back(entity_id);
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

		for (auto material_list : this->render_list) {
			auto material = material_list.first;
			glPolygonMode(GL_FRONT_AND_BACK, material->GetFillMode());

			auto shader = material->GetShader().lock();
			if (!shader) {
				continue;
			}
			shader->Use();

			glUniformMatrix4fv(shader->GetUniform("view"), 1, GL_FALSE, &camera_matrix[0][0]);
			glUniformMatrix4fv(shader->GetUniform("projection"), 1, GL_FALSE, &this->projection[0][0]);
			GLint model_index = shader->GetUniform("model");

			for (auto buffer_list : material_list.second) {
				auto buffer = buffer_list.first;
				glBindVertexArray(buffer->vao);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ibo);
				for (auto entity : buffer_list.second) {
					glm::mat4 model_matrix = glm::mat4(1.0);
					if (this->model_matricies.find(entity) != this->model_matricies.end()) {
						model_matrix = this->model_matricies.at(entity);
					}
					glUniformMatrix4fv(model_index, 1, GL_FALSE, &model_matrix[0][0]);
					glDrawElements(GL_TRIANGLES, buffer->index_count, GL_UNSIGNED_INT, 0);
				}
			}
			shader->UnUse();
		}
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
