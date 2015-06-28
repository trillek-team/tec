#include "components/renderable.hpp"
#include "resources/mesh.hpp"
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
}
