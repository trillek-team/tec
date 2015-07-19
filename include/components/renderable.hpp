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
			Property sprop(Property::STRING);
			(refcomp.properties2["Mesh Name"] = sprop).Set<std::string>(val->mesh_name);
			refcomp.properties2["Mesh Name"].update_func = [val] (Property& prop) { val->mesh_name = prop.Get<std::string>(); };
			(refcomp.properties2["Shader Name"] = sprop).Set<std::string>(val->shader_name);
			refcomp.properties2["Shader Name"].update_func = [val] (Property& prop) { val->shader_name = prop.Get<std::string>(); };
			Property bprop(Property::BOOLEAN);
			(refcomp.properties2["Hidden"] = bprop).Set<bool>(val->hidden);
			refcomp.properties2["Hidden"].update_func = [val] (Property& prop) { val->hidden = prop.Get<bool>(); };
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
