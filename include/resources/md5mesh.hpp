// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "filesystem.hpp"
#include "mesh.hpp"

namespace tec {
	class MD5Mesh final : public MeshFile {
	public:
		MD5Mesh() { }
		~MD5Mesh() { }

		/*****************************/
		/* MD5Mesh helper structures */
		/*****************************/
		struct Joint {
			Joint() : name(""), parent(-1), position(0.0f, 0.0f, 0.0f) { }

			/**
			 * \brief Compute the joint's quaternion W component.
			 *
			 * \return void
			 */
			void ComputeW();

			std::string name; // The name of the joint
			int parent; // index
			glm::vec3 position; // Transformed position.
			glm::quat orientation; // Quaternion
			glm::mat4 bind_pose;
			glm::mat4 bind_pose_inverse;
		};

		struct Vertex {
			Vertex() : startWeight(0), weight_count(0), uv(0.0f, 0.0f) { }
			glm::vec2 uv; // Texture coordinates
			int startWeight; // index
			unsigned int weight_count;
			glm::vec3 position; // Calculated position (cached for later use)
			glm::vec3 normal; // Calculated normal (cached for later use)
		};

		struct Triangle {
			Triangle() {
				this->verts[0] = 0; verts[1] = 0; verts[2] = 0;
			}
			int verts[3]; // index
		};

		struct Weight {
			Weight() : joint(0), bias(0.0f), position(0.0f, 0.0f, 0.0f) { }
			int joint; // index
			float bias; // 0-1
			glm::vec3 position;
		};

		// Holds information about each mesh inside the file.
		struct InternalMesh {
			std::string shader; // MTR or texture filename.
			std::vector<Vertex> verts;
			std::vector<Triangle> tris;
			std::vector<Weight> weights;
		};

		/**
		 * \brief Returns a resource with the specified name.
		 *
		 * The only used initialization property is "filename".
		 * \param[in] const std::vector<Property>& properties The creation properties for the resource.
		 * \return std::shared_ptr<MD5Mesh> The created MD5Mesh resource.
		 */
		static std::shared_ptr<MD5Mesh> Create(const FilePath& fname);

		/**
		 * \brief Loads the MD5Mesh file from disk and parses it.
		 *
		 * \return bool If the mesh was valid and loaded correctly.
		 */
		bool Parse();

		/**
		 * \brief Calculates the final vertex positions based on the bind-pose skeleton.
		 *
		 * There isn't a return as the processing will just do nothing if the
		 * parse data was default objects.
		 * \return void
		 */
		void CalculateVertexPositions();

		/**
		 * \brief Calculates the vertex normals based on the bind-pose skeleton and mesh tris.
		 *
		 * There isn't a return as the processing will just do nothing if the
		 * parse data was default objects.
		 * \return void
		 */
		void CalculateVertexNormals();

		/**
		 * \brief Updates the meshgroups index list based from the loaded mesh groups.
		 *
		 * There isn't a return as the processing will just do nothing if the
		 * parse data was default objects.
		 * \return void
		 */
		void UpdateIndexList();

		/**
		 * \brief Sets the mesh filename.
		 *
		 * This is just a shorthand function that can be called directly via script API.
		 * \param[in] const std::string& fname The mesh filename.
		 * \return bool True if initialization finished with no errors.
		 */
		void SetFileName(const FilePath& fname) {
			this->path = fname;
		}

		// Used for MD5Anim::CheckMesh().
		friend class MD5Anim;
	private:
		std::vector<InternalMesh> meshes_internal;
		FilePath path; // Path to MD5Mesh file
		std::vector<Joint> joints;
	};
}
