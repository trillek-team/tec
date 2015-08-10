#include "components/renderable.hpp"
#include "resources/mesh.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex-buffer-object.hpp"
#include "reflection.hpp"

namespace tec {
	Renderable::Renderable(std::shared_ptr<VertexBufferObject> buf,
		std::shared_ptr<Shader> shader) : buffer(buf), shader(shader) {
		if (this->buffer) {
			size_t group_count = this->buffer->GetVertexGroupCount();
			for (size_t i = 0; i < group_count; ++i) {
				this->vertex_groups.insert(this->buffer->GetVertexGroup(i));
			}
		}
	}

	ReflectionComponent Renderable::Reflection(Renderable* val) {
		ReflectionComponent refcomp;
		Property dprop(Property::DROPDOWN);
		dropdown_t key_func_mesh = std::make_pair(MeshMap::Keys, val->mesh_name);
		(refcomp.properties["Mesh Picker"] = dprop).Set(key_func_mesh);
		refcomp.properties["Mesh Picker"].update_func = [val] (Property& prop) {
			dropdown_t key_func = prop.Get<dropdown_t>();
			val->mesh_name = key_func.second;
			val->mesh = MeshMap::Get(val->mesh_name);
			val->buffer.reset();
		};
		dropdown_t key_func_shader = std::make_pair(ShaderMap::Keys, val->shader_name);
		(refcomp.properties["Shader Picker"] = dprop).Set(key_func_shader);
		refcomp.properties["Shader Picker"].update_func = [val] (Property& prop) {
			dropdown_t key_func = prop.Get<dropdown_t>();
			val->shader_name = key_func.second;
			val->shader = ShaderMap::Get(val->shader_name);
		};
		Property bprop(Property::BOOLEAN);
		(refcomp.properties["Hidden"] = bprop).Set(val->hidden);
		refcomp.properties["Hidden"].update_func = [val] (Property& prop) { val->hidden = prop.Get<bool>(); };

		return std::move(refcomp);
	}

	void Renderable::Out(proto::Component* target) {
		proto::Renderable* comp = target->mutable_renderable();
		comp->set_mesh_name(this->mesh_name);
		comp->set_shader_name(this->shader_name);
		comp->set_hidden(this->hidden);
	}

	extern std::map<std::string, std::function<void(std::string)>> file_factories;

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
	}
}
