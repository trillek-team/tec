// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <memory>
#include <set>
#include <map>
#include <functional>

#include <components.pb.h>

namespace tec {
	class Shader;
	class MeshFile;
	class VertexBufferObject;
	struct VertexGroup;

	struct Renderable {
		Renderable(std::shared_ptr<VertexBufferObject> buf,
			std::shared_ptr<Shader> shader = nullptr);
		Renderable() {

		}

		void Out(proto::Component* target);
		void In(const proto::Component& source);

		std::set<VertexGroup*> vertex_groups;
		std::shared_ptr<VertexBufferObject> buffer;
		std::string mesh_name;
		std::shared_ptr<MeshFile> mesh;
		std::string shader_name;
		std::shared_ptr<Shader> shader;
		bool hidden{ false };
	};
}
