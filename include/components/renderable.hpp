#include <memory>
#include <set>
#include <map>

namespace tec {
	class Shader;
	class VertexBufferObject;
	struct VertexGroup;

	struct Renderable {
		Renderable(std::shared_ptr<VertexBufferObject> buf,
			std::shared_ptr<Shader> shader = nullptr);
		Renderable() {

		}
		bool hidden = false;
		std::set<VertexGroup*> vertex_groups;
		std::shared_ptr<VertexBufferObject> buffer;
		std::shared_ptr<Shader> shader;
	};
}
