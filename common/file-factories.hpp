#pragma once

#include <functional>
#include <unordered_map>

#include "filesystem.hpp"
#include "tec-types.hpp"

namespace tec {
std::unordered_map<std::string, std::function<void(std::string)>> file_factories;
template <typename T> void AddFileFactory() {
	file_factories[GetTypeEXT<T>()] = [](std::string fname) {
		FilePath path(fname);
		if (path.isAbsolutePath()) {
			T::Create(fname);
		}
		else {
			T::Create(FilePath::GetAssetPath(fname));
		}
	};
}
} // namespace tec
