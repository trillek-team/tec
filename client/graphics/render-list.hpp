#pragma once

#include <optional>
#include <ranges>
#include <spdlog/spdlog.h>
#include <utility>

#include "animation.hpp"
#include "entity.hpp"
#include "render-item.hpp"
#include "renderable.hpp"
#include "shader.hpp"
#include "view.hpp"

namespace tec::graphics {
using RenderableMap = Multiton<eid, Renderable*>;
using RenderItems = std::map<std::shared_ptr<Shader>, std::set<RenderItem*>>;

const std::string DEFAULT_SHADER_NAME = "deferred";

class RenderItemList {
public:
	void SetDefaultShader(std::shared_ptr<Shader> _default_shader) {
		this->default_shader = std::move(_default_shader);
	}
	void UpdateRenderList(const double delta) {
		static auto _log = spdlog::get("console_log");
		this->render_items.clear();

		if (!this->default_shader) {
			this->default_shader = ShaderMap::Get(DEFAULT_SHADER_NAME);
		}

		// Loop through each renderable and update its model matrix.
		for (auto& [entity_id, renderable] : RenderableMap::Instances()) {
			if (renderable->hidden) {
				continue;
			}

			auto& mesh = renderable->mesh;
			auto& ri = renderable->render_item;
			if (!mesh && ri) {
				renderable->render_item.reset();
				ri.reset();
			}
			if (mesh && (!ri || ri->mesh_at_set != mesh.get())) {
				auto buffer_itr = mesh_buffers.find(mesh);
				std::shared_ptr<VertexBufferObject> buffer;
				if (buffer_itr == mesh_buffers.cend()) {
					buffer = std::make_shared<VertexBufferObject>(vertex::VF_FULL);
					buffer->Load(mesh);
					mesh_buffers[mesh] = buffer;
				}
				else {
					buffer = buffer_itr->second;
				}
				if (const std::size_t group_count = buffer->GetVertexGroupCount(); group_count > 0) {
					if (!ri) {
						ri = std::make_shared<RenderItem>();
					}
					else {
						ri->vertex_groups.clear();
					}
					ri->vbo = buffer;
					ri->vertex_groups.reserve(group_count);
					ri->mesh_at_set = mesh.get();
					for (std::size_t i = 0; i < group_count; ++i) {
						ri->vertex_groups.push_back(*buffer->GetVertexGroup(i));
					}
					renderable->render_item = ri;
				}
				else {
					_log->warn("[RenderSystem] empty mesh on Renderable [{}]", entity_id);
					renderable->render_item.reset();
					ri.reset();
				}
			}

			if (ri) {
				if (ri->vbo->Update()) {
					// the mesh updated it's contents
					const std::size_t group_count = ri->vbo->GetVertexGroupCount();
					ri->vertex_groups.clear();
					ri->vertex_groups.reserve(group_count);
					for (std::size_t i = 0; i < group_count; ++i) {
						ri->vertex_groups.push_back(*ri->vbo->GetVertexGroup(i));
					}
				}
				auto [_position, _orientation, _scale, _animation] =
						Entity(entity_id).GetList<Position, Orientation, Scale, Animation>();
				ri->model_position = renderable->local_translation;
				if (_position) {
					ri->model_position += _position->value;
				}
				ri->model_quat = renderable->local_orientation.value;
				if (_orientation) {
					ri->model_quat *= _orientation->value;
				}
				ri->model_scale = glm::vec3{1.0};
				if (_scale) {
					ri->model_scale = _scale->value;
				}

				if (_animation) {
					const_cast<Animation*>(_animation)->UpdateAnimation(delta);
					if (_animation->HasBoneTransforms()) {
						ri->animated = true;
						ri->animation = const_cast<Animation*>(_animation);
					}
				}
				if (!renderable->shader) {
					renderable->shader = default_shader;
					renderable->shader_name = DEFAULT_SHADER_NAME;
				}
				this->render_items[renderable->shader].insert(ri.get());
			}
		}

		for (auto& [entity_id, view] : Multiton<eid, View*>::Instances()) {
			auto [_position, _orientation] = Entity(entity_id).GetList<Position, Orientation>();
			if (_position) {
				view->view_pos = -_position->value;
			}
			if (_orientation) {
				view->view_quat = glm::inverse(_orientation->value);
			}
			if (view->active) {
				this->current_view = *view;
			}
		}
	}

	RenderItems& GetRenderItems() { return this->render_items; }
	[[nodiscard]] std::optional<View> GetCurrentView() const { return this->current_view; }

private:
	std::map<std::shared_ptr<MeshFile>, std::shared_ptr<VertexBufferObject>> mesh_buffers{};
	RenderItems render_items{};
	std::shared_ptr<Shader> default_shader;
	std::optional<View> current_view{};
};
} // namespace tec::graphics
