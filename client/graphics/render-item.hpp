#pragma once

#include "animation.hpp"
#include "renderable.hpp"
#include "resources/mesh.hpp"
#include "vertex-buffer-object.hpp"

namespace tec {

struct RenderItem {
	glm::vec3 model_position{};
	glm::vec3 model_scale{};
	glm::quat model_quat{};
	std::vector<VertexGroup> vertex_groups;
	std::shared_ptr<VertexBufferObject> vbo;
	MeshFile* mesh_at_set{nullptr}; // used only for equality testing, does not own an object
	bool animated{false};
	Animation* animation{nullptr};
};

} // namespace tec
