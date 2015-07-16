#include <memory>
#include <set>
#include <map>
#include "entity.hpp"

#include "../proto/components.pb.h"

namespace tec {
	class Shader;
	class Mesh;
	class VertexBufferObject;
	struct VertexGroup;

	struct Renderable {
		Renderable(std::shared_ptr<VertexBufferObject> buf,
			std::shared_ptr<Shader> shader = nullptr);
		Renderable() {

		}

		static ReflectionComponent Reflection(Renderable* val) {
			ReflectionComponent refcomp;
			refcomp.properties["mesh_name"] = val->mesh_name;
			refcomp.properties["shader_name"] = val->shader_name;
			refcomp.properties["hidden"] = std::to_string(val->hidden);
			return std::move(refcomp);
		}

		void Out(proto::Renderable* target);

		void In(const proto::Renderable& source);

		std::set<VertexGroup*> vertex_groups;
		std::shared_ptr<VertexBufferObject> buffer;
		std::string mesh_name;
		std::shared_ptr<Mesh> mesh;
		std::string shader_name;
		std::shared_ptr<Shader> shader;
		bool hidden = false;
	};
}
