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
	while (str.find_first_of('(') != std::string::npos) {
		str.erase(str.find_first_of('('), 1);
	}
	while (str.find_first_of(')') != std::string::npos) {
		str.erase(str.find_first_of(')'), 1);
	}
	while (str.find_first_of('"') != std::string::npos) {
		str.erase(str.find_first_of('"'), 1);
	}
	while (str.find_first_of('\'') != std::string::npos) {
		str.erase(str.find_first_of('\''), 1);
	}

	return str;
}

void ComputeWNeg(glm::quat& q) {
	glm::vec3 imaginary_part{q.x, q.y, q.z};
	imaginary_part *= imaginary_part;
	float t = 1.0f - imaginary_part.x - imaginary_part.y - imaginary_part.z;

	q.w = 0;
	if (t > 0.0f) {
		q.w = -sqrtf(t);
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
	ss >> j.name >> j.parent;
	ss >> j.position.x >> j.position.y >> j.position.z;
	ss >> j.orientation.x >> j.orientation.y >> j.orientation.z;

	ComputeWNeg(j.orientation);
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
	ss >> index >> v.uv.x >> v.uv.y >> v.startWeight >> v.weight_count;
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
	MD5Mesh::Triangle t{};
	int index = 0;
	ss >> index >> t.verts[0] >> t.verts[1] >> t.verts[2];
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
	ss >> index >> w.joint >> w.bias >> w.position.x >> w.position.y >> w.position.z;
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
	if (!this->path.isValidPath() || !this->path.FileExists()) {
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
	_log->debug("[MD5Mesh] Parsing file {}", path.toString());

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
			unsigned int njoints;
			ss >> njoints;
			this->joints.reserve(njoints);
		}
		else if (identifier == "numMeshes") {
			unsigned int nmeshes;
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
						auto pixbuf = PixelBuffer::Create(mesh.shader, filename, true);
						auto tex = std::make_shared<TextureObject>(pixbuf);
						TextureMap::Set(mesh.shader, tex);
					}
				}
				else if (identifier == "numverts") {
					unsigned int nverts;
					ss >> nverts;
					mesh.verts.reserve(nverts);
				}
				else if (identifier == "vert") {
					mesh.verts.push_back(ParseVertex(ss));
				}
				else if (identifier == "numtris") {
					unsigned int ntris;
					ss >> ntris;
					mesh.tris.reserve(ntris);
				}
				else if (identifier == "tri") {
					mesh.tris.push_back(ParseTriangle(ss));
				}
				else if (identifier == "numweights") {
					unsigned int nweights;
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
			CreateMesh()->has_weight = true;
		}
	}

	for (std::size_t i = 0; i < this->meshes_internal.size(); ++i) {
		Mesh* mesh = this->meshes[i];
		InternalMesh& int_mesh = this->meshes_internal[i];
		if (mesh->vert_count() < int_mesh.verts.size()) {
			mesh->resize(int_mesh.verts.size());
		}
		for (std::size_t j = 0; j < int_mesh.verts.size(); ++j) {
			vertex::Vertex& vdata = mesh->base_verts[j];
			vertex::WeightData& vweight = mesh->vert_weight[j];
			Vertex& vert = int_mesh.verts[j];

			// Compute vertex position based on joint position.
			for (std::uint8_t k = 0; k < vert.weight_count; ++k) {
				auto weight_index = static_cast<std::size_t>(vert.startWeight) + static_cast<std::size_t>(k);
				Weight& weight = int_mesh.weights[weight_index];
				Joint& weight_joint = this->joints[weight.joint];

				/* Calculate transformed vertex for this weight */
				glm::vec3 wv = weight_joint.orientation * (weight.position);

				/* the sum of all weight->bias should be 1.0 */
				vdata.position += (weight_joint.position + wv) * weight.bias;
				vweight.bone_indices[k] = static_cast<glm::u32>(weight.joint);
				vweight.bone_weights[k] = weight.bias;
			}

			// Cache the calculated position for later
			vert.position = vdata.position;

			// Copy the texture coordinates
			vdata.uv = vert.uv;
		}
	}
}

void MD5Mesh::CalculateVertexNormals() {
	if (this->meshes.size() < this->meshes_internal.size()) {
		this->meshes.reserve(this->meshes_internal.size());
		for (std::size_t i = this->meshes.size(); i < this->meshes_internal.size(); ++i) {
			CreateMesh()->has_weight = true;
		}
	}

	for (std::size_t i = 0; i < this->meshes_internal.size(); ++i) {
		Mesh* mesh = this->meshes[i];
		InternalMesh& int_mesh = this->meshes_internal[i];
		if (mesh->vert_count() < int_mesh.verts.size()) {
			mesh->resize(int_mesh.verts.size());
			// If we need to resize here then we will need to calculate the vertex positions
			// again.
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
			mesh->base_verts[j].normal = normal;

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
		size_t slash = material_name.find_last_of("/");
		slash = (slash == std::string::npos) ? 0 : (slash + 1); // skip past any slash
		size_t dot = material_name.find_last_of(".");
		material_name = material_name.substr(slash, dot - slash) + "_material";
		if (objgroup->indices.size() < int_mesh.tris.size()) {
			objgroup->indices.reserve(int_mesh.tris.size() * 3);
		}
		for (auto tri : int_mesh.tris) {
			objgroup->indices.push_back(tri.verts[0]);
			objgroup->indices.push_back(tri.verts[1]);
			objgroup->indices.push_back(tri.verts[2]);
		}
		MaterialGroup mat_group = {0, static_cast<unsigned int>(objgroup->indices.size()), material_name};
		mat_group.textures.push_back(int_mesh.shader);
		objgroup->material_groups.push_back(std::move(mat_group));
	}
}
} // namespace tec
