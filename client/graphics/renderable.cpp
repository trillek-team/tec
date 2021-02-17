#include "renderable.hpp"

#include "resources/mesh.hpp"
#include "shader.hpp"
#include "vertex-buffer-object.hpp"

namespace tec {
Renderable::Renderable(std::shared_ptr<VertexBufferObject> buf, std::shared_ptr<Shader> shader) :
	buffer(buf), shader(shader) {
	if (this->buffer) {
		if (this->vertex_groups.size() == 0) {
			std::size_t group_count = this->buffer->GetVertexGroupCount();
			for (std::size_t i = 0; i < group_count; ++i) {
				this->vertex_groups.insert(this->buffer->GetVertexGroup(i));
			}
		}
	}
}

void Renderable::Out(proto::Component* target) {
	proto::Renderable* comp = target->mutable_renderable();
	comp->set_mesh_name(this->mesh_name);
	comp->set_shader_name(this->shader_name);
	comp->set_hidden(this->hidden);
	this->local_translation.Out(comp->mutable_position());
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
		this->local_translation.In(comp.position());
	}
	if (comp.has_orientation()) {
		this->local_orientation.In(comp.orientation());
	}
}
} // namespace tec
