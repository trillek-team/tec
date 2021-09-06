#include "mesh.hpp"

namespace tec {

MeshFile::~MeshFile() {
	for (Mesh* mesh : this->meshes) {
		if (mesh) {
			delete mesh;
		}
	}
}

} // namespace tec
