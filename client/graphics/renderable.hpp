#pragma once

#include <functional>
#include <map>
#include <memory>
#include <set>

#include <components.pb.h>

#include "components/transforms.hpp"

namespace tec {
class Shader;
class MeshFile;
struct RenderItem;

struct Renderable {
	Renderable() : local_translation(0.f) {}

	void Out(proto::Component* target);
	void In(const proto::Component& source);

	std::shared_ptr<RenderItem> render_item;
	std::string mesh_name;
	std::shared_ptr<MeshFile> mesh;
	std::string shader_name;
	std::shared_ptr<Shader> shader;
	glm::vec3 local_translation;
	Orientation local_orientation;
	bool hidden{false};
};
} // namespace tec
