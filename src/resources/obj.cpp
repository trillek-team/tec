#include "resources/obj.hpp"

#include <fstream>
#include <memory>
#include <sstream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "spdlog/spdlog.h"
#include "client/graphics/texture-object.hpp"

namespace tec {
	/**
	 * \brief Cleans an input string by removing certain grouping characters.
	 *
	 * These characters include ", ', (, and ).
	 * \param[in] std::string str The string to clean.
	 * \return The cleaned string
	 */
	extern std::string CleanString(std::string str);

	bool OBJ::ParseMTL(const FilePath& fname) {
		auto _log = spdlog::get("console_log");
		if (!fname.isValidPath() || !fname.FileExists()) {
			_log->error("[OBJ] Can't open the file {}. Invalid path or missing file.", fname.toString());
			// Can't open the file!
			return false;
		}
		auto base_path = this->path.BasePath();

		std::ifstream f(fname.GetNativePath(), std::ios::in);
		if (!f.is_open()) {
			_log->error("[OBJ] Error opening file {}", fname.toString());
			return false;
		}

		std::shared_ptr<MTL> currentMTL;

		std::string line;
		while (std::getline(f, line)) {
			std::stringstream ss(line);
			std::string identifier;

			ss >> identifier;
			if (identifier == "newmtl") {
				std::string name;
				ss >> name;
				this->materials[name] = std::make_shared<MTL>();
				currentMTL = this->materials[name];
			}
			else if (identifier == "Ka") {
				float r, g, b;
				ss >> r; ss >> g; ss >> b;
				currentMTL->ka[0] = r; currentMTL->ka[1] = g; currentMTL->ka[2] = b;
			}
			else if (identifier == "Kd") {
				float r, g, b;
				ss >> r; ss >> g; ss >> b;
				currentMTL->kd[0] = r; currentMTL->kd[1] = g; currentMTL->kd[2] = b;
			}
			else if (identifier == "Ks") {
				float r, g, b;
				ss >> r; ss >> g; ss >> b;
				currentMTL->ks[0] = r; currentMTL->ks[1] = g; currentMTL->ks[2] = b;
			}
			else if ((identifier == "Tr") || (identifier == "d")) {
				float tr;
				ss >> tr;
				currentMTL->tr = tr;
			}
			else if (identifier == "Ns") {
				float ns;
				ss >> ns;
				currentMTL->hardness = ns;
			}
			else if (identifier == "illum") {
				int i;
				ss >> i;
				currentMTL->illum = i;
			}
			else if (identifier == "map_Kd") {
				std::string filename;
				ss >> filename;
				currentMTL->diffuseMap = filename;
				if (!TextureMap::Has(currentMTL->diffuseMap)) {
					auto pixbuf = PixelBuffer::Create(currentMTL->diffuseMap, (base_path / filename));
					auto tex = std::make_shared<TextureObject>(pixbuf);
					TextureMap::Set(currentMTL->diffuseMap, tex);
				}
			}
			else if (identifier == "map_Ka") {
				std::string filename;
				ss >> filename;
				currentMTL->ambientMap = filename;
				//TODO Load ambient map to a PixelBuffer
			}
			else if (identifier == "map_Bump") {
				std::string filename;
				ss >> filename;
				currentMTL->normalMap = filename;
				//TODO Load bump map to a PixelBuffer
			}
		}
		return true;
	}

	std::shared_ptr<OBJ> OBJ::Create(const FilePath& fname) {
		auto obj = std::make_shared<OBJ>();
		obj->SetFileName(fname);

		obj->SetName(fname.SubpathFrom("assets").toGenericString());

		if (obj->Parse()) {
			obj->PopulateMeshGroups();

			MeshMap::Set(obj->GetName(), obj);

			return obj;
		}

		spdlog::get("console_log")->warn("[OBJ] Error parsing file {}", fname.toString());
		return nullptr;
	}

	bool OBJ::Parse() {
		auto _log = spdlog::get("console_log");
		if (!this->path.isValidPath() || !this->path.FileExists()) {
			_log->error("[OBJ] Can't open the file {}. Invalid path or missing file.", path.toString());
			// Can't open the file!
			return false;
		}
		auto base_path = this->path.BasePath();

		std::ifstream f(this->path.GetNativePath(), std::ios::in);
		if (!f.is_open()) {
			_log->error("[OBJ] Error opening file {}", path.toString());
			return false;
		}

		std::shared_ptr<OBJGroup> currentVGroup;
		OBJGroup::FaceGroup* current_face_group = nullptr;

		std::string line;
		while (std::getline(f, line)) {
			std::stringstream ss(line);
			std::string identifier;

			ss >> identifier;
			if (identifier == "mtllib") {
				std::string fname;
				ss >> fname;
				ParseMTL(base_path / fname); // Path to MTL file
			}
			else if (identifier == "v") {
				glm::vec3 vert;
				ss >> vert.x; ss >> vert.y; ss >> vert.z;
				this->positions.push_back(vert);
			}
			else if (identifier == "vt") {
				glm::vec2 uv;
				ss >> uv.x; ss >> uv.y;
				uv.y = 1 - uv.y;
				this->uvs.push_back(uv);
			}
			else if (identifier == "vn") {
				glm::vec3 norm;
				ss >> norm.x; ss >> norm.y; ss >> norm.z;
				this->normals.push_back(norm);
			}
			else if ((identifier == "g") || (identifier == "o")) {
				std::string name;
				ss >> name;
				if (currentVGroup) {
					currentVGroup->face_groups.push_back(current_face_group);
					std::string last_mtlname = "";
					if (current_face_group) {
						last_mtlname = current_face_group->mtl;
					}
					current_face_group = new OBJGroup::FaceGroup();
					current_face_group->mtl = last_mtlname;
					this->vertexGroups.push_back(currentVGroup);
				}
				currentVGroup = std::make_shared<OBJGroup>();
				currentVGroup->name = name;
			}
			else if (identifier == "usemtl") {
				std::string mtlname;
				ss >> mtlname;
				if (current_face_group) { // We have a new material so start a new FaceGroup.
					currentVGroup->face_groups.push_back(current_face_group);
					current_face_group = nullptr;
				}
				current_face_group = new OBJGroup::FaceGroup();
				current_face_group->mtl = mtlname;
			}
			else if (identifier == "f") {
				Face face, face2;
				bool quad = false;
				std::string faceLine;
				std::getline(ss, faceLine);
				// Check if we have 3 vertex indices per face vertex.
				if (faceLine.find("/") != std::string::npos) {
					// Check if the UV is omitted and replace it with 0 if it is.
					while (faceLine.find("//") != std::string::npos) {
						faceLine = faceLine.replace(faceLine.find("//"), 2, " 0 ");
					}
					// Replace the / separators with spaces for stringstream output.
					std::replace(faceLine.begin(), faceLine.end(), '/', ' ');
					std::stringstream face_ss(faceLine);
					face_ss >> face.pos[0]; face_ss >> face.uv[0]; face_ss >> face.norm[0];
					face_ss >> face.pos[1]; face_ss >> face.uv[1]; face_ss >> face.norm[1];
					face_ss >> face.pos[2]; face_ss >> face.uv[2]; face_ss >> face.norm[2];
					if (!face_ss.eof()) {
						quad = true;
						face2.pos[0] = face.pos[2]; face2.uv[0] = face.uv[2]; face2.norm[0] = face.norm[2];
						face_ss >> face2.pos[1]; face_ss >> face2.uv[1]; face_ss >> face2.norm[1];
						face2.pos[2] = face.pos[0]; face2.uv[2] = face.uv[0]; face2.norm[2] = face.norm[0];
					}
				}
				else {
					std::stringstream face_ss(faceLine);
					// There is only 1 vertex index per face vertex.
					face_ss >> face.pos[0]; face_ss >> face.pos[1]; face_ss >> face.pos[2];
					if (!face_ss.eof()) {
						quad = true;
						face2.pos[0] = face.pos[2]; face_ss >> face2.pos[1]; face2.pos[2] = face.pos[0];
					}
				}
				if (current_face_group) {
					current_face_group->faces.push_back(face);
					if (quad) {
						current_face_group->faces.push_back(face2);
					}
				}
			}
		}

		if (currentVGroup) {
			currentVGroup->face_groups.push_back(current_face_group);
			this->vertexGroups.push_back(currentVGroup);
		}

		return true;
	}

	void OBJ::PopulateMeshGroups() {
		if (this->MeshFile::meshes.size() < this->vertexGroups.size()) {
			this->MeshFile::meshes.reserve(this->vertexGroups.size());
			for (std::size_t i = this->MeshFile::meshes.size(); i < this->vertexGroups.size(); ++i) {
				CreateMesh();
			}
		}

		for (std::size_t i = 0; i < this->vertexGroups.size(); ++i) {
			const OBJ::OBJGroup* vert_group = this->vertexGroups[i].get();
			Mesh* mesh = this->MeshFile::meshes[i];
			if (this->MeshFile::meshes[i]->object_groups.size() == 0) {
				this->MeshFile::meshes[i]->object_groups.push_back(new ObjectGroup());
			}
			ObjectGroup* objgroup = this->MeshFile::meshes[i]->object_groups[0];

			for (const OBJ::OBJGroup::FaceGroup* face_group : vert_group->face_groups) {
				if (objgroup->material_groups.size() == 0) {
					objgroup->material_groups.reserve(vert_group->face_groups.size());
				}
				MaterialGroup mat_group;
				mat_group.start = objgroup->indices.size();
				glm::vec4 diffuse_color;
				if (this->materials.find(face_group->mtl) != this->materials.end()) {
					std::shared_ptr<MTL> material = this->materials[face_group->mtl];
					std::string material_name = material->diffuseMap;
					material_name = material_name.substr(
						material_name.find_last_of("/") + 1,
						material_name.find_last_of(".") -
						material_name.find_last_of("/") - 1)
						+ "_material";
					mat_group.material_name = material_name;
					mat_group.textures.push_back(this->materials[face_group->mtl]->diffuseMap);
					diffuse_color = glm::vec4(material->kd, 1.0);
				}

				size_t j = mesh->verts.size();

				if (mesh->verts.size() < (face_group->faces.size() * 3 + mesh->verts.size())) {
					mesh->verts.resize(face_group->faces.size() * 3 + mesh->verts.size());
				}

				for (std::size_t k = 0; k < face_group->faces.size(); ++k) {
					const Face& face = face_group->faces[k];
					if (face.pos[0] > 0 && face.pos[0] <= this->positions.size()) {
						mesh->verts[j].position = this->positions[face.pos[0] - 1];
					}
					if (face.uv[0] > 0 && face.uv[0] <= this->normals.size()) {
						mesh->verts[j].uv = this->uvs[face.uv[0] - 1];
					}
					if (face.norm[0] > 0 && face.norm[0] <= this->normals.size()) {
						mesh->verts[j].normal = this->normals[face.norm[0] - 1];
					}
					mesh->verts[j].color = diffuse_color;
					objgroup->indices.push_back(j++);
					if (face.pos[1] > 0 && face.pos[1] <= this->positions.size()) {
						mesh->verts[j].position = this->positions[face.pos[1] - 1];
					}
					if (face.uv[1] > 0 && face.uv[1] <= this->normals.size()) {
						mesh->verts[j].uv = this->uvs[face.uv[1] - 1];
					}
					if (face.norm[1] > 0 && face.norm[1] <= this->normals.size()) {
						mesh->verts[j].normal = this->normals[face.norm[1] - 1];
					}
					mesh->verts[j].color = diffuse_color;
					objgroup->indices.push_back(j++);
					if (face.pos[2] > 0 && face.pos[2] <= this->positions.size()) {
						mesh->verts[j].position = this->positions[face.pos[2] - 1];
					}
					if (face.uv[2] > 0 && face.uv[2] <= this->normals.size()) {
						mesh->verts[j].uv = this->uvs[face.uv[2] - 1];
					}
					if (face.norm[2] > 0 && face.norm[2] <= this->normals.size()) {
						mesh->verts[j].normal = this->normals[face.norm[2] - 1];
					}
					mesh->verts[j].color = diffuse_color;
					objgroup->indices.push_back(j++);
				}
				mat_group.count = objgroup->indices.size() - mat_group.start;
				objgroup->material_groups.push_back(std::move(mat_group));
			}
		}
	}
}
