#include "components/renderable.hpp"
#include "resources/mesh.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex-buffer-object.hpp"

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
