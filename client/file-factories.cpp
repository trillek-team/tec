#include <functional>
#include <unordered_map>

#include "resources/md5mesh.hpp"
#include "resources/obj.hpp"
#include "resources/md5anim.hpp"
#include "resources/vorbis-stream.hpp"
#include "resources/script-file.hpp"

#include "types.hpp"
#include "filesystem.hpp"

namespace tec {
	std::unordered_map<std::string, std::function<void(std::string)>> file_factories;
	template <typename T>
	void AddFileFactory() {
		file_factories[GetTypeEXT<T>()] = [] (std::string fname) {
			FilePath path(fname);
			if (path.isAbsolutePath()) {
				T::Create(fname);
			}
			else {
				T::Create(FilePath::GetAssetPath(fname));
			}
		};
	}

	void InitializeFileFactories() {
		AddFileFactory<MD5Mesh>();
		AddFileFactory<MD5Anim>();
		AddFileFactory<OBJ>();
		AddFileFactory<VorbisStream>();
		AddFileFactory<ScriptFile>();
	}
}
