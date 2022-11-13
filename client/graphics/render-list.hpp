#pragma once

#include <optional>
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

class RenderItemList {
public:
	void SetDefaultShader(std::shared_ptr<Shader> _default_shader) {
		this->default_shader = std::move(_default_shader);
	}
	void UpdateRenderList(double delta) {
		static auto _log = spdlog::get("console_log");
		RenderItemList render_item_list;

		if (auto& fallback_shader = this->default_shader; !fallback_shader) {
			fallback_shader = ShaderMap::Get("debug");
		}

		// Loop through each renderable and update its model matrix.
		for (auto itr = RenderableMap::Begin(); itr != RenderableMap::End(); ++itr) {
			eid entity_id = itr->first;
			Renderable* renderable = itr->second;
			if (renderable->hidden) {
				continue;
			}
			Entity entity(entity_id);
			auto [_position, _orientation, _scale, _animation] =
					entity.GetList<Position, Orientation, Scale, Animation>();
			glm::vec3 position = renderable->local_translation;
			if (_position) {
				position += _position->value;
			}
			glm::quat orientation = renderable->local_orientation.value;
			if (_orientation) {
				orientation *= _orientation->value;
			}
			glm::vec3 scale(1.0);
			if (_scale) {
				scale = _scale->value;
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
				if (std::size_t group_count = buffer->GetVertexGroupCount(); group_count > 0) {
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
					std::size_t group_count = ri->vbo->GetVertexGroupCount();
					ri->vertex_groups.clear();
					ri->vertex_groups.reserve(group_count);
					for (std::size_t i = 0; i < group_count; ++i) {
						ri->vertex_groups.push_back(*ri->vbo->GetVertexGroup(i));
					}
				}
				ri->model_position = position;
				ri->model_scale = scale;
				ri->model_quat = orientation;

				if (_animation) {
					const_cast<Animation*>(_animation)->UpdateAnimation(delta);
					if (_animation->HasBoneTransforms()) {
						ri->animated = true;
						ri->animation = const_cast<Animation*>(_animation);
					}
				}
				this->render_items[renderable->shader].insert(ri.get());
			}
		}

		for (auto itr = Multiton<eid, View*>::Begin(); itr != Multiton<eid, View*>::End(); ++itr) {
			auto& [entity_id, view] = *itr;

			Entity entity(entity_id);
			auto [_position, _orientation] = entity.GetList<Position, Orientation>();
			glm::vec3 position;
			if (_position) {
				position = _position->value;
			}
			glm::quat orientation;
			if (_orientation) {
				orientation = _orientation->value;
			}

			view->view_pos = -position;
			view->view_quat = glm::inverse(orientation);
			if (view->active) {
				this->current_view = *view;
			}
		}
	}

	RenderItems& GetRenderItems() { return this->render_items; }
	std::optional<View> GetCurrentView() const { return this->current_view; }

private:
	std::map<std::shared_ptr<MeshFile>, std::shared_ptr<VertexBufferObject>> mesh_buffers;
	RenderItems render_items;
	std::shared_ptr<Shader> default_shader;
	std::optional<View> current_view;
};
} // namespace tec::graphics
