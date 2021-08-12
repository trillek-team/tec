#include "renderable.hpp"

#include "resources/mesh.hpp"
#include "shader.hpp"
#include "vertex-buffer-object.hpp"

namespace tec {

void Renderable::Out(proto::Component* target) {
	proto::Renderable* comp = target->mutable_renderable();
	comp->set_mesh_name(this->mesh_name);
	comp->set_shader_name(this->shader_name);
	comp->set_hidden(this->hidden);
	auto pos = comp->mutable_position();
	pos->set_x(this->local_translation.x);
	pos->set_y(this->local_translation.y);
	pos->set_z(this->local_translation.z);
	this->local_orientation.Out(comp->mutable_orientation());
}

extern std::unordered_map<std::string, std::function<void(std::string)>> file_factories;

void Renderable::In(const proto::Component& source) {
	const proto::Renderable& comp = source.renderable();
	if (comp.has_mesh_name()) {
		this->mesh_name = comp.mesh_name();
		if (!MeshMap::Has(this->mesh_name)) {
			std::string ext = this->mesh_name.substr(this->mesh_name.find_last_of(".") + 1);
			if (file_factories.find(ext) != file_factories.end()) {
				file_factories[ext](this->mesh_name);
			}
		}
		this->mesh = MeshMap::Get(this->mesh_name);
	}
	if (comp.has_shader_name()) {
		this->shader_name = comp.shader_name();
		this->shader = ShaderMap::Get(this->shader_name);
	}
	if (comp.has_hidden()) {
		this->hidden = comp.hidden();
	}
	if (comp.has_position()) {
		auto position = comp.position();
		this->local_translation = glm::vec3(position.x(), position.y(), position.z());
	}
	if (comp.has_orientation()) {
		this->local_orientation.In(comp.orientation());
	}
}
} // namespace tec
