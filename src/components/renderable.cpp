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
		Property sprop(Property::STRING);
		(refcomp.properties["Mesh Name"] = sprop).Set(val->mesh_name);
		refcomp.properties["Mesh Name"].update_func = [val] (Property& prop) { val->mesh_name = prop.Get<std::string>(); };
		Property dprop(Property::DROPDOWN);
		std::vector<std::pair<std::string, bool>> meshes;
		for (auto itr = MeshMap::Begin(); itr != MeshMap::End(); ++itr) {
			meshes.push_back(std::make_pair(itr->first, (itr->first == val->mesh_name) ? true : false));
		}
		(refcomp.properties["Mesh Picker"] = dprop).Set(meshes);
		refcomp.properties["Mesh Picker"].update_func = [val] (Property& prop) {
			std::vector<std::pair<std::string, bool>> mesh_list = prop.Get<std::vector<std::pair<std::string, bool>>>();
			for (size_t item = 0; item < mesh_list.size(); ++item) {
				if (mesh_list[item].second) {
					val->mesh_name = mesh_list[item].first;
					val->mesh = MeshMap::Get(val->mesh_name);
					val->buffer->Load(val->mesh);
					val->vertex_groups.clear();
					size_t group_count = val->buffer->GetVertexGroupCount();
					for (size_t i = 0; i < group_count; ++i) {
						val->vertex_groups.insert(val->buffer->GetVertexGroup(i));
					}
					break;
				}
			}
		};

		(refcomp.properties["Shader Name"] = sprop).Set(val->shader_name);
		refcomp.properties["Shader Name"].update_func = [val] (Property& prop) { val->shader_name = prop.Get<std::string>(); };
		Property bprop(Property::BOOLEAN);
		(refcomp.properties["Hidden"] = bprop).Set(val->hidden);
		refcomp.properties["Hidden"].update_func = [val] (Property& prop) { val->hidden = prop.Get<bool>(); };
		return std::move(refcomp);
	}

	void Renderable::Out(proto::Renderable* target) {
		target->set_mesh_name(this->mesh_name);
		target->set_shader_name(this->shader_name);
		target->set_hidden(this->hidden);
	}

	void Renderable::In(const proto::Renderable& source) {
		if (source.has_mesh_name()) {
			this->mesh_name = source.mesh_name();
			this->mesh = MeshMap::Get(this->mesh_name);
		}
		if (source.has_shader_name()) {
			this->shader_name = source.shader_name();
			this->shader = ShaderMap::Get(this->shader_name);
		}
		if (source.has_hidden()) {
			this->hidden = source.hidden();
		}
	}
}
