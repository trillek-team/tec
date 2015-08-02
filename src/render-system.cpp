#include "render-system.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <thread>
#include <cmath>

#include "graphics/shader.hpp"
#include "graphics/view.hpp"
#include "graphics/material.hpp"
#include "graphics/lights.hpp"
#include "components/transforms.hpp"
#include "components/renderable.hpp"
#include "resources/mesh.hpp"
#include "resources/obj.hpp"
#include "entity.hpp"
#include "os.hpp"

namespace tec {
	RenderSystem::RenderSystem() : window_width(1024), window_height(768) {
		auto err = glGetError();
		// If there is an error that means something went wrong when creating the context.
		if (err) {
			return;
		}

		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
		this->gbuffer.Init(window_width, window_height);
		this->sphere_vbo.Load(OBJ::Create("assets/sphere/sphere.obj"));
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
		this->gbuffer.Resize(window_width, window_height);
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
		glViewport(0, 0, this->window_width, this->window_height);
		RenderGeometryPass();
		BeginLightPass();
		PointLightPass();
		//RenderGbuffer();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void RenderSystem::RenderGbuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		this->gbuffer.BindForRendering();

		GLsizei HalfWidth = (GLsizei)(this->window_width / 2.0f);
		GLsizei HalfHeight = (GLsizei)(this->window_height / 2.0f);
		GLsizei QuaterWidth = (GLsizei)(this->window_width / 4.0f);
		GLsizei QuaterHeight = (GLsizei)(this->window_height / 4.0f);
		GLsizei EightWidth = (GLsizei)(this->window_width / 8.0f);
		GLsizei EightHeight = (GLsizei)(this->window_height / 8.0f);

		this->gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
		glBlitFramebuffer(0, 0, this->window_width, this->window_height,
			this->window_width - QuaterWidth, 0, this->window_width, QuaterHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		this->gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
		glBlitFramebuffer(0, 0, this->window_width, this->window_height,
			this->window_width - QuaterWidth, QuaterHeight, this->window_width, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		this->gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
		glBlitFramebuffer(0, 0, this->window_width, this->window_height,
			this->window_width - QuaterWidth, HalfHeight, this->window_width, HalfHeight + QuaterHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		this->gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
		glBlitFramebuffer(0, 0, this->window_width, this->window_height,
			this->window_width - QuaterWidth, HalfHeight + QuaterHeight, this->window_width, this->window_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	void RenderSystem::RenderGeometryPass() {
		static float red = 0.0f, blue = 0.0f, green = 0.0f;

		glClearColor(red, green, blue, 0.5f);
		glDepthMask(GL_TRUE);

		this->gbuffer.BindForWriting();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glm::mat4 camera_matrix(1.0);
		auto view = this->current_view.lock();
		if (view) {
			camera_matrix = view->view_matrix;
		}

		auto def_shader = ShaderMap::Get("deferred");
		def_shader->Use();
		for (auto shader_list : this->render_item_list) {
			if (!def_shader) {
				continue;
			}

			glUniformMatrix4fv(def_shader->GetUniformLocation("view"), 1, GL_FALSE, &camera_matrix[0][0]);
			glUniformMatrix4fv(def_shader->GetUniformLocation("projection"), 1, GL_FALSE, &this->projection[0][0]);
			GLint animatrix_loc = def_shader->GetUniformLocation("animation_matrix");
			GLint animated_loc = def_shader->GetUniformLocation("animated");
			GLint model_index = def_shader->GetUniformLocation("model");

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
		}
		def_shader->UnUse();
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
	}

	void RenderSystem::BeginLightPass() {
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		this->gbuffer.BindForReading();
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderSystem::PointLightPass() {
		glm::mat4 camera_matrix(1.0);
		auto view = this->current_view.lock();
		if (view) {
			camera_matrix = view->view_matrix;
		}

		auto def_pl_shader = ShaderMap::Get("deferred_pointlight");
		def_pl_shader->Use();

		glUniformMatrix4fv(def_pl_shader->GetUniformLocation("view"), 1, GL_FALSE, &camera_matrix[0][0]);
		glUniformMatrix4fv(def_pl_shader->GetUniformLocation("projection"), 1, GL_FALSE, &this->projection[0][0]);
		glUniform1i(def_pl_shader->GetUniformLocation("gPositionMap"), GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
		glUniform1i(def_pl_shader->GetUniformLocation("gNormalMap"), GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
		glUniform1i(def_pl_shader->GetUniformLocation("gColorMap"), GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
		glUniform2f(def_pl_shader->GetUniformLocation("gScreenSize"), this->window_width, this->window_height);
		GLint model_index = def_pl_shader->GetUniformLocation("model");
		GLint Color_index = def_pl_shader->GetUniformLocation("gPointLight.Base.Color");
		GLint AmbientIntensity_index = def_pl_shader->GetUniformLocation("gPointLight.Base.AmbientIntensity");
		GLint DiffuseIntensity_index = def_pl_shader->GetUniformLocation("gPointLight.Base.DiffuseIntensity");
		GLint Atten_Constant_index = def_pl_shader->GetUniformLocation("gPointLight.Atten.Constant");
		GLint Atten_Linear_index = def_pl_shader->GetUniformLocation("gPointLight.Atten.Linear");
		GLint Atten_Exp_index = def_pl_shader->GetUniformLocation("gPointLight.Atten.Exp");

		for (auto itr = PointLightMap::Begin(); itr != PointLightMap::End(); ++itr) {
			auto entity_id = itr->first;
			std::shared_ptr<PointLight> light = itr->second;

			Entity e(entity_id);
			glm::vec3 position, scale;
			glm::quat orientation;
			if (e.Has<Position>()) {
				position = e.Get<Position>().lock()->value;
			}
			if (e.Has<Orientation>()) {
				orientation = e.Get<Orientation>().lock()->value;
			}
			if (e.Has<Scale>()) {
				scale = e.Get<Scale>().lock()->value;
			}
			
			glm::mat4 transform_matrix = glm::scale(glm::translate(glm::mat4(1.0), position) *
				glm::mat4_cast(orientation), scale);

			glUniform3f(Color_index, light->color.x, light->color.y, light->color.z);
			glUniform1f(AmbientIntensity_index, light->ambient_intensity);
			glUniform1f(DiffuseIntensity_index, light->diffuse_intensity);
			glUniform1f(Atten_Constant_index, light->Attenuation.constant);
			glUniform1f(Atten_Linear_index, light->Attenuation.linear);
			glUniform1f(Atten_Exp_index, light->Attenuation.exponential);

			light->UpdateBoundingRadius();
			glm::mat4 scale_matrix = glm::scale(transform_matrix, glm::vec3(light->bounding_radius));
			glUniformMatrix4fv(model_index, 1, GL_FALSE, glm::value_ptr(scale_matrix));
			glBindVertexArray(this->sphere_vbo.GetVAO());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sphere_vbo.GetIBO());
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, this->sphere_vbo.GetVertexGroup(0)->index_count, GL_UNSIGNED_INT, 0);
		}

		def_pl_shader->UnUse();
	}

	void RenderSystem::DirectionalLightPass() {

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
