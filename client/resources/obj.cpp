#include "obj.hpp"

#include <algorithm>
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
extern std::string CleanString(std::string str);

bool OBJ::ParseMTL(const Path& fname) {
	auto _log = spdlog::get("console_log");
	if (!fname || !fname.FileExists()) {
		_log->error("[OBJ] Can't open the file {}. Invalid path or missing file.", fname.toString());
		// Can't open the file!
		return false;
	}
	auto base_path = this->path.BasePath();

	auto f = fname.OpenStream();
	if (!f->is_open()) {
		_log->error("[OBJ] Error opening file {}", fname.toString());
		return false;
	}

	std::shared_ptr<MTL> currentMTL;

	std::string line;
	while (std::getline(*f, line)) {
		std::stringstream ss(line);
		std::string identifier;

		ss >> identifier;
		if (identifier == "newmtl") {
			std::string _name;
			ss >> _name;
			this->materials[_name] = std::make_shared<MTL>();
			currentMTL = this->materials[_name];
		}
		else if (identifier == "Ka") {
			float r, g, b;
			ss >> r;
			ss >> g;
			ss >> b;
			currentMTL->ka[0] = r;
			currentMTL->ka[1] = g;
			currentMTL->ka[2] = b;
		}
		else if (identifier == "Kd") {
			float r, g, b;
			ss >> r;
			ss >> g;
			ss >> b;
			currentMTL->kd[0] = r;
			currentMTL->kd[1] = g;
			currentMTL->kd[2] = b;
		}
		else if (identifier == "Ks") {
			float r, g, b;
			ss >> r;
			ss >> g;
			ss >> b;
			currentMTL->ks[0] = r;
			currentMTL->ks[1] = g;
			currentMTL->ks[2] = b;
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
				auto pixbuf = PixelBuffer::Create(currentMTL->diffuseMap, (base_path / filename), true);
				auto tex = std::make_shared<TextureObject>(pixbuf);
				TextureMap::Set(currentMTL->diffuseMap, tex);
			}
		}
		else if (identifier == "map_Ka") {
			std::string filename;
			ss >> filename;
			currentMTL->ambientMap = filename;
			if (!TextureMap::Has(currentMTL->ambientMap)) {
				auto pixbuf = PixelBuffer::Create(currentMTL->ambientMap, (base_path / filename));
				auto tex = std::make_shared<TextureObject>(pixbuf);
				TextureMap::Set(currentMTL->ambientMap, tex);
			}
		}
		else if (identifier == "map_Bump") {
			std::string filename;
			ss >> filename;
			currentMTL->normalMap = filename;
			if (!TextureMap::Has(currentMTL->normalMap)) {
				auto pixbuf = PixelBuffer::Create(currentMTL->normalMap, (base_path / filename));
				auto tex = std::make_shared<TextureObject>(pixbuf);
				TextureMap::Set(currentMTL->normalMap, tex);
			}
		}
	}
	return true;
}

std::shared_ptr<OBJ> OBJ::Create(const Path& fname) {
	auto obj = std::make_shared<OBJ>();
	obj->SetFileName(fname);

	obj->SetName(fname.Subpath(1).toString());

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
	if (!this->path || !this->path.FileExists()) {
		_log->error("[OBJ] Can't open the file {}. Invalid path or missing file.", path.toString());
		// Can't open the file!
		return false;
	}
	auto base_path = this->path.BasePath();

	auto f = this->path.OpenStream();
	if (!f->is_open()) {
		_log->error("[OBJ] Error opening file {}", path.toString());
		return false;
	}
	_log->debug("[OBJ] Parsing file {}", path.toString());

	std::ostringstream oss;
	oss << f->rdbuf();
	std::string buffer = oss.str();

	std::shared_ptr<OBJGroup> currentVGroup = std::make_shared<OBJGroup>();
	currentVGroup->name = "default";
	OBJGroup::FaceGroup* current_face_group = new OBJGroup::FaceGroup();
	current_face_group->mtl = "(default)";

	// Pre-pass to get counts to avoid constant reallocation
	unsigned int object_count = 0;
	std::vector<unsigned int> object_face_count;
	unsigned int vertex_count = 0, normal_count = 0, uv_count = 0;
	std::string line;
	std::size_t start = 0U;
	std::size_t end = buffer.find('\n');
	std::string identifier;
	while (end != std::string::npos) {
		line = buffer.substr(start, end - start);
		start = end + 1;
		end = buffer.find('\n', start);

		identifier = line.substr(0, line.find(' '));
		if (identifier == "mtllib") {
			std::string fname = line.substr(line.find(' ') + 1);
			ParseMTL(base_path / fname); // Path to MTL file
		}
		else if (identifier == "v") {
			vertex_count++;
		}
		else if (identifier == "vt") {
			uv_count++;
		}
		else if (identifier == "vn") {
			normal_count++;
		}
		else if (identifier == "o") {
			object_count++;
		}
		else if (identifier == "f") {
			if (object_face_count.size() < object_count) {
				object_face_count.push_back(0);
			}
			object_face_count[object_count - 1] += 3;
		}
	}
	this->positions.reserve(vertex_count);
	this->normals.reserve(normal_count);
	this->uvs.reserve(uv_count);

	start = 0U;
	end = buffer.find('\n');
	identifier = "";
	std::stringstream face_ss;
	std::stringstream ss;
	while (end != std::string::npos) {
		line = buffer.substr(start, end - start);
		start = end + 1;
		end = buffer.find('\n', start);
		ss.clear();
		ss.str(line);

		ss >> identifier;
		if (identifier == "v") {
			glm::vec3 vert;
			ss >> vert.x >> vert.y >> vert.z;
			this->positions.push_back(vert);
		}
		else if (identifier == "vt") {
			glm::vec2 uv;
			ss >> uv.x >> uv.y;
			uv.y = 1 - uv.y;
			this->uvs.push_back(uv);
		}
		else if (identifier == "vn") {
			glm::vec3 norm;
			ss >> norm.x >> norm.y >> norm.z;
			this->normals.push_back(norm);
		}
		else if (identifier == "o") { // this is OPTIONAL!!
			if (currentVGroup) {
				if (currentVGroup->face_groups.size() > 0) { // Empty groups are worth saving
					this->vertexGroups.push_back(currentVGroup);
				}
			}
			currentVGroup = std::make_shared<OBJGroup>();
			ss >> currentVGroup->name;
		}
		else if (identifier == "usemtl") { // this is OPTIONAL!!
			if (current_face_group) {
				if (current_face_group->faces.size()) {
					currentVGroup->face_groups.push_back(current_face_group);
				}
				else {
					delete current_face_group;
				}
			}
			current_face_group = nullptr;
			std::string mtlname;
			ss >> mtlname;
			for (OBJGroup::FaceGroup* face_group : currentVGroup->face_groups) {
				if (face_group->mtl == mtlname) {
					current_face_group = face_group;
				}
			}
			if (!current_face_group) {
				current_face_group = new OBJGroup::FaceGroup();
				current_face_group->faces.reserve(
						static_cast<std::size_t>(object_face_count[this->vertexGroups.size()]) * 2);
				current_face_group->mtl = mtlname;
			}
		}
		else if (identifier == "f") {
			Face face, face2;
			bool quad = false;
			std::string faceLine = line.substr(line.find(' ') + 1);
			// Check if we have 3 vertex indices per face vertex.
			if (faceLine.find("/") != std::string::npos) {
				// Check if the UV is omitted and replace it with 0 if it is.
				while (faceLine.find("//") != std::string::npos) {
					faceLine = faceLine.replace(faceLine.find("//"), 2, " 0 ");
				}
				// Replace the / separators with spaces for stringstream output.
				std::replace(faceLine.begin(), faceLine.end(), '/', ' ');
				face_ss.clear();
				face_ss.str(faceLine);
				face_ss >> face.pos[0] >> face.uv[0] >> face.norm[0];
				face_ss >> face.pos[1] >> face.uv[1] >> face.norm[1];
				face_ss >> face.pos[2] >> face.uv[2] >> face.norm[2];
				if (!face_ss.eof()) {
					quad = true;
					face2.pos[0] = face.pos[2];
					face2.uv[0] = face.uv[2];
					face2.norm[0] = face.norm[2];
					face_ss >> face2.pos[1] >> face2.uv[1] >> face2.norm[1];
					face2.pos[2] = face.pos[0];
					face2.uv[2] = face.uv[0];
					face2.norm[2] = face.norm[0];
				}
			}
			else {
				face_ss.clear();
				face_ss.str(faceLine);
				// There is only 1 vertex index per face vertex.
				face_ss >> face.pos[0];
				face_ss >> face.pos[1];
				face_ss >> face.pos[2];
				if (!face_ss.eof()) {
					quad = true;
					face2.pos[0] = face.pos[2];
					face_ss >> face2.pos[1];
					face2.pos[2] = face.pos[0];
				}
			}
			if (current_face_group) {
				current_face_group->faces.emplace_back(face);
				if (quad) {
					current_face_group->faces.emplace_back(face2);
				}
			}
		}
	}

	if (currentVGroup) {
		if (current_face_group) {
			if (current_face_group->faces.size()) {
				currentVGroup->face_groups.push_back(current_face_group);
			}
			else {
				delete current_face_group;
			}
		}
		this->vertexGroups.push_back(currentVGroup);
	}

	// Reduce its capacity to fit its size.
	for (std::shared_ptr<OBJGroup> obj_group : this->vertexGroups) {
		for (OBJGroup::FaceGroup* face_group : obj_group->face_groups) {
			face_group->faces.shrink_to_fit();
		}
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
		mesh->has_color = true;
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
			glm::vec4 diffuse_color{1.0};
			auto mat_itr = this->materials.find(face_group->mtl);
			if (mat_itr != this->materials.end()) {
				std::shared_ptr<MTL> material = mat_itr->second;
				auto diffuse_file = Path(material->diffuseMap);
				mat_group.material_name = diffuse_file.FileStem() + "_material";
				spdlog::get("console_log")
						->trace("[OBJ] Material group: Tex:{} -> Mat:{}",
								material->diffuseMap,
								mat_group.material_name);
				mat_group.textures.push_back(material->diffuseMap);
				diffuse_color = glm::vec4(material->kd, 1.0);
			}

			auto j = static_cast<unsigned int>(mesh->base_verts.size());

			if (mesh->vert_count() < (face_group->faces.size() * 3 + mesh->vert_count())) {
				mesh->resize(face_group->faces.size() * 3 + mesh->vert_count());
			}

			for (const Face& face : face_group->faces) {
				for (size_t index = 0; index < 3; index++) {
					if (face.pos[index] != 0 && face.pos[index] <= this->positions.size()) {
						mesh->base_verts[j].position = this->positions[face.pos[index] - 1];
					}
					if (face.uv[index] != 0 && face.uv[index] <= this->uvs.size()) {
						mesh->base_verts[j].uv = this->uvs[face.uv[index] - 1];
					}
					if (face.norm[index] != 0 && face.norm[index] <= this->normals.size()) {
						mesh->base_verts[j].normal = this->normals[face.norm[index] - 1];
					}
					mesh->vert_color[j] = diffuse_color;
					objgroup->indices.push_back(j++);
				}
			}
			mat_group.count = objgroup->indices.size() - mat_group.start;
			objgroup->material_groups.push_back(std::move(mat_group));
		}
	}
}
} // namespace tec
