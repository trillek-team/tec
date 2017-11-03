// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <glm/glm.hpp>
#include <list>
#include <vector>
#include <memory>
#include <string>
#include "multiton.hpp"

namespace tec {
	class MeshFile;
	typedef Multiton<std::string, std::shared_ptr<MeshFile>> MeshMap;

	// Vertex data used for rendering or other purposes.
	struct VertexData {
		VertexData() : position(0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) { }
		VertexData(double x, double y, double z, double r, double g, double b, double u = 0.0, double v = 0.0) :
			position(x, y, z), color(r, g, b, 1.0f), uv(u, v) { }
		glm::vec3 position;
		glm::vec4 color;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec4 bone_weights; // Used for animation.
		glm::uvec4 bone_indices; // Used for animation.
	};

	struct MaterialGroup final {
		unsigned int start;
		unsigned int count;
		std::string material_name;
		std::list<std::string> textures;
	};

	struct ObjectGroup final {
		std::vector<unsigned int> indices;
		std::vector<MaterialGroup> material_groups;
	};

	struct Mesh final {
		~Mesh() {
			for (ObjectGroup* object : this->object_groups) {
				if (object) {
					delete object;
				}
			}
		}
		std::vector<VertexData> verts;
		std::vector<ObjectGroup*> object_groups;
	};

	class MeshFile {
	public:
		MeshFile() : name("test") { }
		virtual ~MeshFile() {
			for (Mesh* mesh : this->meshes) {
				if (mesh) {
					delete mesh;
				}
			}
		}

		/**
		 * \brief Creates a new mesh and adds it to this file.
		 *
		 * \return Mesh* The mesh that was added to this file.
		 */
		Mesh* CreateMesh() {
			this->meshes.push_back(new Mesh());
			return *(this->meshes.end() - 1);
		}

		/**
		 * \brief Adds a mesh to this file. Ownership is transferred.
		 *
		 * \param[in] Mesh* mesh The mesh to add to this file.
		 * \return void
		 */
		void AddMesh(Mesh* mesh) {
			this->meshes.push_back(mesh);
		}

		/**
		 * \brief Returns a specific mesh.
		 *
		 * \param[in] const unsigned size_t index The index of the mesh to retrieve.
		 * \return std::weak_ptr<MeshGroup> The requested mesh or null if the index is invalid.
		 */
		Mesh* GetMesh(const std::size_t index) {
			if (index < this->meshes.size()) {
				return this->meshes.at(index);
			}

			return nullptr;
		}

		/**
		 * \brief Returns the number of meshes in this file.
		 *
		 * \return size_t The number of meshes in this file.
		 */
		std::size_t GetMeshCount() const {
			return this->meshes.size();
		}

		const std::string GetName() const {
			return this->name;
		}
		void SetName(const std::string& name) {
			this->name = name;
		}

		bool IsDirty() const {
			return this->dirty;
		}
		/** \brief Mark dirty */
		void Invalidate() {
			this->dirty = true;
		}
		/** \brief Mark not dirty */
		void Validate() {
			this->dirty = false;
		}
	protected:
		std::vector<Mesh*> meshes;
		std::string name;
		bool dirty = false;
	};
}
