// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "md5mesh.hpp"

#include <fstream>
#include <memory>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include "graphics/texture-object.hpp"

namespace tec {
	/**
	 * \brief Cleans an input string by removing certain grouping characters.
	 *
	 * These characters include ", ', (, and ).
	 * \param[in] std::string str The string to clean.
	 * \return The cleaned string
	 */
	std::string CleanString(std::string str) {
		while (str.find("(") != std::string::npos) {
			str.replace(str.find("("), 1, "");
		}
		while (str.find(")") != std::string::npos) {
			str.replace(str.find(")"), 1, "");
		}
		while (str.find("\"") != std::string::npos) {
			str.replace(str.find("\""), 1, "");
		}
		while (str.find("\'") != std::string::npos) {
			str.replace(str.find("\'"), 1, "");
		}

		return str;
	}

	void MD5Mesh::Joint::ComputeW() {
		float t = 1.0f -
			(this->orientation[0] * this->orientation[0]) -
			(this->orientation[1] * this->orientation[1]) -
			(this->orientation[2] * this->orientation[2]);

		if (t < 0.0f) {
			this->orientation[3] = 0.0f;
		}
		else {
			this->orientation[3] = -sqrt(t);
		}
	}

	/**
	 * \brief Parses a joint line.
	 *
	 * joints {
	 *   "name" parent(pos.x pos.y pos.z) (orient.x orient.y orient.z)
	 *   ...
	 * }
	 * \param[in] std::stringstream& ss The stream that contains the joint to parse.
	 * \return The parsed joint or a default one if the parsing failed.
	 */
	MD5Mesh::Joint ParseJoint(std::stringstream& ss) {
		MD5Mesh::Joint j;
		ss >> j.name;
		ss >> j.parent;
		ss >> j.position[0]; ss >> j.position[1]; ss >> j.position[2];
		ss >> j.orientation[0]; ss >> j.orientation[1]; ss >> j.orientation[2];
		j.ComputeW();

		glm::mat4x4 boneTranslation = glm::translate(glm::mat4(1.0f), j.position);
		glm::mat4x4 boneRotation = glm::toMat4(j.orientation);

		j.bind_pose = (boneTranslation * boneRotation);
		j.bind_pose_inverse = glm::inverse(j.bind_pose);

		return j;
	}

	/**
	 * \brief Parses a vertex line.
	 *
	 * vert vertIndex ( s t ) startWeight countWeight
	 * \param[in] std::stringstream& ss The stream that contains the vertex to parse.
	 * \return The parsed vertex or a default one if the parsing failed.
	 */
	MD5Mesh::Vertex ParseVertex(std::stringstream& ss) {
		MD5Mesh::Vertex v;
		int index;
		ss >> index;
		ss >> v.uv[0]; ss >> v.uv[1];
		ss >> v.startWeight; ss >> v.weight_count;
		return v;
	}

	/**
	 * \brief Parses a triangle line.
	 *
	 * tri triIndex vertIndex[0] vertIndex[1] vertIndex[2]
	 * \param[in] std::stringstream& ss The stream that contains the triangle to parse.
	 * \return The parsed triangle or a default one if the parsing failed.
	 */
	MD5Mesh::Triangle ParseTriangle(std::stringstream& ss) {
		MD5Mesh::Triangle t;
		int index;
		ss >> index;
		ss >> t.verts[0]; ss >> t.verts[1]; ss >> t.verts[2];
		return t;
	}

	/**
	 * \brief Parses a weight line.
	 *
	 * weight weightIndex joint bias ( pos.x pos.y pos.z )
	 * \param[in] std::stringstream& ss The stream that contains the weight to parse.
	 * \return The parsed weight or a default one if the parsing failed.
	 */
	MD5Mesh::Weight ParsesWeight(std::stringstream& ss) {
		MD5Mesh::Weight w;
		int index;
		ss >> index;
		ss >> w.joint;
		ss >> w.bias;
		ss >> w.position[0]; ss >> w.position[1]; ss >> w.position[2];
		return w;
	}

	std::shared_ptr<MD5Mesh> MD5Mesh::Create(const FilePath& fname) {
		auto mesh = std::make_shared<MD5Mesh>();
		mesh->SetFileName(fname);
		mesh->SetName(fname.SubpathFrom("assets").toGenericString());

		if (mesh->Parse()) {
			mesh->CalculateVertexPositions();
			mesh->CalculateVertexNormals();
			mesh->UpdateIndexList();
			MeshMap::Set(mesh->GetName(), mesh);
			return mesh;
		}
		spdlog::get("console_log")->warn("[MD5Mesh] Error parsing file {}", fname.toString());

		return nullptr;
	}

	bool MD5Mesh::Parse() {
		auto _log = spdlog::get("console_log");
		if (!this->path.isValidPath() || ! this->path.FileExists()) {
			_log->error("[MD5Mesh] Can't open the file {}. Invalid path or missing file.", path.toString());
			// Can't open the file!
			return false;
		}
		auto base_path = this->path.BasePath();
		
		std::ifstream f(this->path.GetNativePath(), std::ios::in);
		if (!f.is_open()) {
			_log->error("[MD5Mesh] Error opening file {}", path.toString());
			return false;
		}

		std::string line;
		while (std::getline(f, line)) {
			std::stringstream ss(line);
			std::string identifier;

			ss >> identifier;
			if (identifier == "MD5Version") {
				int version;
				ss >> version;

				if (version != 10) {
					return false;
				}
			}
			else if (identifier == "numJoints") {
				int njoints;
				ss >> njoints;
				this->joints.reserve(njoints);
			}
			else if (identifier == "numMeshes") {
				int nmeshes;
				ss >> nmeshes;
				this->meshes_internal.reserve(nmeshes);
			}
			else if (identifier == "joints") {
				while (std::getline(f, line)) {
					if (line.find("\"") != std::string::npos) {
						ss.str(CleanString(line));
						Joint joint(ParseJoint(ss));
						this->joints.push_back(std::move(joint));
					}
					// Check if the line contained the closing brace. This is done after parsing
					// as the line might have the ending brace on it.
					if (line.find("}") != std::string::npos) {
						break;
					}
				}
			}
			else if (identifier == "mesh") {
				InternalMesh mesh;
				while (std::getline(f, line)) {
					std::string line2 = CleanString(line);
					ss.str(line2);
					ss.clear();
					identifier.clear();

					ss >> identifier;

					if (identifier == "shader") {
						ss >> mesh.shader;
						auto filename = base_path / mesh.shader;
						if (!TextureMap::Has(mesh.shader)) {
							auto pixbuf = PixelBuffer::Create(mesh.shader, filename);
							auto tex = std::make_shared<TextureObject>(pixbuf);
							TextureMap::Set(mesh.shader, tex);
						}
					}
					else if (identifier == "numverts") {
						int nverts;
						ss >> nverts;
						mesh.verts.reserve(nverts);
					}
					else if (identifier == "vert") {
						mesh.verts.push_back(ParseVertex(ss));
					}
					else if (identifier == "numtris") {
						int ntris;
						ss >> ntris;
						mesh.tris.reserve(ntris);
					}
					else if (identifier == "tri") {
						mesh.tris.push_back(ParseTriangle(ss));
					}
					else if (identifier == "numweights") {
						int nweights;
						ss >> nweights;
						mesh.weights.reserve(nweights);
					}
					else if (identifier == "weight") {
						mesh.weights.push_back(ParsesWeight(ss));
					}
					// Check if the line contained the closing brace. This is done after parsing
					// as the line might have the ending brace on it.
					if (line.find("}") != std::string::npos) {
						this->meshes_internal.push_back(std::move(mesh));
						break;
					}
				}
			}
		}

		return true;
	}

	void MD5Mesh::CalculateVertexPositions() {
		if (this->meshes.size() < this->meshes_internal.size()) {
			this->meshes.reserve(this->meshes_internal.size());
			for (std::size_t i = this->meshes.size(); i < this->meshes_internal.size(); ++i) {
				CreateMesh();
			}
		}

		for (std::size_t i = 0; i < this->meshes_internal.size(); ++i) {
			Mesh* mesh = this->meshes[i];
			InternalMesh& int_mesh = this->meshes_internal[i];
			if (mesh->verts.size() < int_mesh.verts.size()) {
				mesh->verts.resize(int_mesh.verts.size());
			}
			for (std::size_t j = 0; j < int_mesh.verts.size(); ++j) {
				VertexData vdata;

				// Compute vertex position based on joint position.
				for (std::uint8_t k = 0; k < int_mesh.verts[j].weight_count; ++k) {
					Weight& weight = int_mesh.weights[static_cast<std::size_t>(int_mesh.verts[j].startWeight) + static_cast<std::size_t>(k)];

					/* Calculate transformed vertex for this weight */
					glm::vec3 wv = this->joints[weight.joint].orientation * weight.position;

					/* the sum of all weight->bias should be 1.0 */
					vdata.position += (this->joints[weight.joint].position + wv) * weight.bias;
					vdata.bone_indices[k] = static_cast<glm::u32>(weight.joint);
					vdata.bone_weights[k] = static_cast<float>(weight.bias);
				}

				// Cache the calculated position for later
				int_mesh.verts[j].position = vdata.position;

				// Copy the texture coordinates
				vdata.uv = int_mesh.verts[j].uv;

				mesh->verts[j] = vdata;
			}
		}
	}

	void MD5Mesh::CalculateVertexNormals() {
		if (this->meshes.size() < this->meshes_internal.size()) {
			this->meshes.reserve(this->meshes_internal.size());
			for (std::size_t i = this->meshes.size(); i < this->meshes_internal.size(); ++i) {
				CreateMesh();
			}
		}

		for (std::size_t i = 0; i < this->meshes_internal.size(); ++i) {
			Mesh* mesh = this->meshes[i];
			InternalMesh& int_mesh = this->meshes_internal[i];
			if (mesh->verts.size() < int_mesh.verts.size()) {
				mesh->verts.resize(int_mesh.verts.size());
				// If we need to resize here then we will need to calculate the vertex positions again.
				CalculateVertexPositions();
			}
			// Loop through all triangles and calculate the normal of each triangle
			for (std::size_t j = 0; j < int_mesh.tris.size(); ++j) {
				glm::vec3 v0 = int_mesh.verts[int_mesh.tris[j].verts[0]].position;
				glm::vec3 v1 = int_mesh.verts[int_mesh.tris[j].verts[1]].position;
				glm::vec3 v2 = int_mesh.verts[int_mesh.tris[j].verts[2]].position;

				glm::vec3 normal = glm::cross(v2 - v0, v1 - v0);

				int_mesh.verts[int_mesh.tris[j].verts[0]].normal += normal;
				int_mesh.verts[int_mesh.tris[j].verts[1]].normal += normal;
				int_mesh.verts[int_mesh.tris[j].verts[2]].normal += normal;
			}

			// Now normalize all the normals
			for (std::size_t j = 0; j < int_mesh.verts.size(); ++j) {
				Vertex& vert = int_mesh.verts[j];

				glm::vec3 normal = glm::normalize(vert.normal);
				mesh->verts[j].normal = normal;

				// Reset the normal to calculate the bind-pose normal in joint space
				vert.normal = glm::vec3(0);

				// Put the bind-pose normal into joint-local space
				// so the animated normal can be computed faster later
				for (std::size_t k = 0; k < vert.weight_count; ++k) {
					const Weight& weight = int_mesh.weights[vert.startWeight + k];
					vert.normal += (normal * this->joints[weight.joint].orientation) * weight.bias;
				}
			}
		}
	}

	void MD5Mesh::UpdateIndexList() {
		if (this->meshes.size() < this->meshes_internal.size()) {
			this->meshes.reserve(this->meshes_internal.size());
			for (std::size_t i = this->meshes.size(); i < this->meshes_internal.size(); ++i) {
				CreateMesh();
			}
		}

		for (std::size_t i = 0; i < this->meshes_internal.size(); ++i) {
			const InternalMesh& int_mesh = this->meshes_internal[i];
			if (this->meshes[i]->object_groups.size() == 0) {
				this->meshes[i]->object_groups.push_back(new ObjectGroup());
			}
			ObjectGroup* objgroup = this->meshes[i]->object_groups[0];
			std::string material_name = int_mesh.shader;
			material_name = material_name.substr(
				material_name.find_last_of("/") + 1,
				material_name.find_last_of(".") -
				material_name.find_last_of("/") - 1)
				+ "_material";
			if (objgroup->indices.size() < int_mesh.tris.size()) {
				objgroup->indices.reserve(int_mesh.tris.size() * 3);
			}
			for (std::size_t j = 0; j < int_mesh.tris.size(); ++j) {
				objgroup->indices.push_back(int_mesh.tris[j].verts[0]);
				objgroup->indices.push_back(int_mesh.tris[j].verts[1]);
				objgroup->indices.push_back(int_mesh.tris[j].verts[2]);
			}
			MaterialGroup mat_group = {0, static_cast<unsigned int>(objgroup->indices.size()), material_name};
			mat_group.textures.push_back(int_mesh.shader);
			objgroup->material_groups.push_back(std::move(mat_group));
		}
	}
}
