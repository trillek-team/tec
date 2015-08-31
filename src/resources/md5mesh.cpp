#include "resources/md5mesh.hpp"

#include <fstream>
#include <memory>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	std::shared_ptr<MD5Mesh> MD5Mesh::Create(const std::string fname) {
		auto mesh = std::make_shared<MD5Mesh>();
		mesh->fname = fname;

		mesh->SetName(fname);

		if (mesh->Parse()) {
			mesh->CalculateVertexPositions();
			mesh->CalculateVertexNormals();
			mesh->UpdateIndexList();

			MeshMap::Set(fname, mesh);

			return mesh;
		}

		return nullptr;
	}

	bool MD5Mesh::Parse() {
		std::ifstream f(this->fname, std::ios::in);

		std::string file_path;

		if (this->fname.find("/") != std::string::npos) {
			file_path = this->fname.substr(0, this->fname.find_last_of("/") + 1);
		}
		else if (this->fname.find("\"") != std::string::npos) {
			file_path = this->fname.substr(0, this->fname.find_last_of("\"") + 1);
		}

		if (!f.is_open()) {
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
				this->meshes.reserve(nmeshes);
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
						mesh.shader = file_path + mesh.shader;
						if (!TextureMap::Has(mesh.shader)) {
							auto pixbuf = PixelBuffer::Create(mesh.shader, mesh.shader);
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
						this->meshes.push_back(std::move(mesh));
						break;
					}
				}
			}
		}

		return true;
	}

	void MD5Mesh::CalculateVertexPositions() {
		if (this->Mesh::meshes.size() < this->meshes.size()) {
			this->Mesh::meshes.reserve(this->meshes.size());
			for (size_t i = this->Mesh::meshes.size(); i < this->meshes.size(); ++i) {
				this->Mesh::CreateMesh();
			}
		}

		for (size_t i = 0; i < this->meshes.size(); ++i) {
			MFMesh* mesh = this->Mesh::meshes[i];
			if (mesh->verts.size() < this->meshes[i].verts.size()) {
				mesh->verts.resize(this->meshes[i].verts.size());
			}
			for (size_t j = 0; j < this->meshes[i].verts.size(); ++j) {
				VertexData vdata;

				// Compute vertex position based on joint position.
				for (size_t k = 0; k < this->meshes[i].verts[j].weight_count; ++k) {
					Weight& weight = this->meshes[i].weights[this->meshes[i].verts[j].startWeight + k];

					/* Calculate transformed vertex for this weight */
					glm::vec3 wv = this->joints[weight.joint].orientation * weight.position;

					/* the sum of all weight->bias should be 1.0 */
					vdata.position += (this->joints[weight.joint].position + wv) * weight.bias;
					vdata.bone_indicies[k] = weight.joint;
					vdata.bone_weights[k] = weight.bias;
				}

				// Cache the calculated position for later
				this->meshes[i].verts[j].position = vdata.position;

				// Copy the texture coordinates
				vdata.uv = this->meshes[i].verts[j].uv;

				mesh->verts[j] = vdata;
			}
		}
	}

	void MD5Mesh::CalculateVertexNormals() {
		if (this->Mesh::meshes.size() < this->meshes.size()) {
			this->Mesh::meshes.reserve(this->meshes.size());
			for (size_t i = this->Mesh::meshes.size(); i < this->meshes.size(); ++i) {
				this->Mesh::CreateMesh();
			}
		}

		for (size_t i = 0; i < this->meshes.size(); ++i) {
			MFMesh* mesh = this->Mesh::meshes[i];
			if (mesh->verts.size() < this->meshes[i].verts.size()) {
				mesh->verts.resize(this->meshes[i].verts.size());
				// If we need to resize here then we will need to calculate the vertex positions again.
				CalculateVertexPositions();
			}
			// Loop through all triangles and calculate the normal of each triangle
			for (size_t j = 0; j < this->meshes[i].tris.size(); ++j) {
				glm::vec3 v0 = this->meshes[i].verts[this->meshes[i].tris[j].verts[0]].position;
				glm::vec3 v1 = this->meshes[i].verts[this->meshes[i].tris[j].verts[1]].position;
				glm::vec3 v2 = this->meshes[i].verts[this->meshes[i].tris[j].verts[2]].position;

				glm::vec3 normal = glm::cross(v2 - v0, v1 - v0);

				this->meshes[i].verts[this->meshes[i].tris[j].verts[0]].normal += normal;
				this->meshes[i].verts[this->meshes[i].tris[j].verts[1]].normal += normal;
				this->meshes[i].verts[this->meshes[i].tris[j].verts[2]].normal += normal;
			}

			// Now normalize all the normals
			for (size_t j = 0; j < this->meshes[i].verts.size(); ++j) {
				Vertex& vert = this->meshes[i].verts[j];

				glm::vec3 normal = glm::normalize(vert.normal);
				mesh->verts[j].normal = normal;

				// Reset the normal to calculate the bind-pose normal in joint space
				vert.normal = glm::vec3(0);

				// Put the bind-pose normal into joint-local space
				// so the animated normal can be computed faster later
				for (size_t j = 0; j < vert.weight_count; ++j) {
					const Weight& weight = this->meshes[i].weights[vert.startWeight + j];
					vert.normal += (normal * this->joints[weight.joint].orientation) * weight.bias;
				}
			}
		}
	}

	void MD5Mesh::UpdateIndexList() {
		if (this->Mesh::meshes.size() < this->meshes.size()) {
			this->Mesh::meshes.reserve(this->meshes.size());
			for (size_t i = this->Mesh::meshes.size(); i < this->meshes.size(); ++i) {
				this->Mesh::CreateMesh();
			}
		}

		for (size_t i = 0; i < this->meshes.size(); ++i) {
			const MD5Mesh::InternalMesh& mesh = this->meshes[i];
			if (this->Mesh::meshes[i]->object_groups.size() == 0) {
				this->Mesh::meshes[i]->object_groups.push_back(new ObjectGroup());
			}
			ObjectGroup* objgroup = this->Mesh::meshes[i]->object_groups[0];
			std::string material_name = mesh.shader;
			material_name = material_name.substr(
				material_name.find_last_of("/") + 1,
				material_name.find_last_of(".") -
				material_name.find_last_of("/") - 1)
				+ "_material";
			if (objgroup->indicies.size() < mesh.tris.size()) {
				objgroup->indicies.reserve(mesh.tris.size() * 3);
			}
			for (size_t j = 0; j < mesh.tris.size(); ++j) {
				objgroup->indicies.push_back(mesh.tris[j].verts[0]);
				objgroup->indicies.push_back(mesh.tris[j].verts[1]);
				objgroup->indicies.push_back(mesh.tris[j].verts[2]);
			}
			MaterialGroup mat_group = {0, objgroup->indicies.size(), material_name};
			mat_group.textures.push_back(mesh.shader);
			objgroup->material_groups.push_back(std::move(mat_group));
		}
	}
}
