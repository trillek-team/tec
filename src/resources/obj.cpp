#include "resources/obj.hpp"

#include <fstream>
#include <memory>
#include <sstream>
#include <algorithm>

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
	extern std::string CleanString(std::string str);

	bool OBJ::ParseMTL(const FilePath& fname) {
		if (!fname.isValidPath() || !fname.FileExists()) {
			// Can't open the file!
			return false;
		}
		auto base_path = this->path.BasePath();

		std::ifstream f(fname.GetNativePath(), std::ios::in);
		if (!f.is_open()) {
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

		return nullptr;
	}

	bool OBJ::Parse() {
		if (!this->path.isValidPath() || ! this->path.FileExists()) {
			// Can't open the file!
			return false;
		}
		auto base_path = this->path.BasePath();

		std::ifstream f(this->path.GetNativePath(), std::ios::in);
		if (!f.is_open()) {
			return false;
		}

		std::shared_ptr<VertexGroup> currentVGroup;

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
					this->vertexGroups.push_back(currentVGroup);
				}
				currentVGroup = std::make_shared<VertexGroup>();
				currentVGroup->name = name;
			}
			else if (identifier == "usemtl") {
				std::string mtlname;
				ss >> mtlname;
				if (currentVGroup) {
					currentVGroup->mtl = mtlname;
				}
			}
			else if (identifier == "f") {
				Face face;
				std::string faceLine;
				std::getline(ss, faceLine);
				// Check if we have 3 vertex indicies per face vertex.
				if (faceLine.find("/") != std::string::npos) {
					// Check if the UV is ommited and replace it with 0 if it is.
					while (faceLine.find("//") != std::string::npos) {
						faceLine = faceLine.replace(faceLine.find("//"), 2, " 0 ");
					}
					// Replace the / separators with spaces for stringstream ouput.
					std::replace(faceLine.begin(), faceLine.end(), '/', ' ');
					std::stringstream face_ss(faceLine);
					face_ss >> face.pos[0]; face_ss >> face.uv[0]; face_ss >> face.norm[0];
					face_ss >> face.pos[1]; face_ss >> face.uv[1]; face_ss >> face.norm[1];
					face_ss >> face.pos[2]; face_ss >> face.uv[2]; face_ss >> face.norm[2];
				}
				else {
					std::stringstream face_ss(faceLine);
					// There is only 1 vertex index per face vertex.
					face_ss >> face.pos[0]; face_ss >> face.pos[1]; face_ss >> face.pos[2];
				}
				if (currentVGroup) {
					currentVGroup->faces.push_back(face);
				}
			}
		}

		if (currentVGroup) {
			this->vertexGroups.push_back(currentVGroup);
		}

		return true;
	}

	void OBJ::PopulateMeshGroups() {
		if (this->mesh_groups.size() < this->vertexGroups.size()) {
			this->mesh_groups.resize(this->vertexGroups.size());
			for (auto& mgruop : this->mesh_groups) {
				if (!mgruop) {
					mgruop = std::make_shared<MeshGroup>();
				}
			}
		}

		for (size_t v = 0; v < this->vertexGroups.size(); ++v) {
			auto vgroup = this->vertexGroups[v];
			auto mgruop = this->mesh_groups[v];
			if (mgruop->verts.size() < (vgroup->faces.size() * 3)) {
				mgruop->verts.resize(vgroup->faces.size() * 3);
			}
			if (this->materials.find(vgroup->mtl) != this->materials.end()) {
				auto material_name = this->materials[vgroup->mtl]->diffuseMap;
				mgruop->material_name = material_name.substr(
					material_name.find_last_of("/") + 1,
					material_name.find_last_of(".") -
					material_name.find_last_of("/") - 1)
					+ "_material";
			}
			for (size_t i = 0, j = 0; i < vgroup->faces.size(); ++i) {
				Face face;
				if (vgroup->faces[i].pos[0] > 0 && vgroup->faces[i].pos[0] <= this->positions.size()) {
					mgruop->verts[j].position = this->positions[vgroup->faces[i].pos[0] - 1];
				}
				if (vgroup->faces[i].uv[0] > 0 && vgroup->faces[i].uv[0] <= this->normals.size()) {
					mgruop->verts[j].uv = this->uvs[vgroup->faces[i].uv[0] - 1];
				}
				if (vgroup->faces[i].norm[0] > 0 && vgroup->faces[i].norm[0] <= this->normals.size()) {
					mgruop->verts[j].normal = this->normals[vgroup->faces[i].norm[0] - 1];
				}
				mgruop->indicies.push_back(j++);
				if (vgroup->faces[i].pos[1] > 0 && vgroup->faces[i].pos[1] <= this->positions.size()) {
					mgruop->verts[j].position = this->positions[vgroup->faces[i].pos[1] - 1];
				}
				if (vgroup->faces[i].uv[1] > 0 && vgroup->faces[i].uv[1] <= this->normals.size()) {
					mgruop->verts[j].uv = this->uvs[vgroup->faces[i].uv[1] - 1];
				}
				if (vgroup->faces[i].norm[1] > 0 && vgroup->faces[i].norm[1] <= this->normals.size()) {
					mgruop->verts[j].normal = this->normals[vgroup->faces[i].norm[1] - 1];
				}
				mgruop->indicies.push_back(j++);
				if (vgroup->faces[i].pos[2] > 0 && vgroup->faces[i].pos[2] <= this->positions.size()) {
					mgruop->verts[j].position = this->positions[vgroup->faces[i].pos[2] - 1];
				}
				if (vgroup->faces[i].uv[2] > 0 && vgroup->faces[i].uv[2] <= this->normals.size()) {
					mgruop->verts[j].uv = this->uvs[vgroup->faces[i].uv[2] - 1];
				}
				if (vgroup->faces[i].norm[2] > 0 && vgroup->faces[i].norm[2] <= this->normals.size()) {
					mgruop->verts[j].normal = this->normals[vgroup->faces[i].norm[2] - 1];
				}
				mgruop->indicies.push_back(j++);
			}
			if (this->materials.find(vgroup->mtl) != this->materials.end()) {
				mgruop->textures.push_back(this->materials[vgroup->mtl]->diffuseMap);
			}
		}
	}
}
