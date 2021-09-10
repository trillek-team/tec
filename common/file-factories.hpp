#pragma once

#include <functional>
#include <unordered_map>

#include "filesystem.hpp"
#include "multiton.hpp"
#include "tec-types.hpp"

namespace tec {
extern std::unordered_map<std::string, std::function<void(std::string)>> file_factories;

void InvokeFileFactory(const std::string& file_name);

template <typename T> std::shared_ptr<T> GetResource(const std::string& res_name) {
	if (res_name.empty()) {
		return std::shared_ptr<T>();
	}
	if (!Multiton<std::string, std::shared_ptr<T>>::Has(res_name)) {
		InvokeFileFactory(res_name);
	}
	return Multiton<std::string, std::shared_ptr<T>>::Get(res_name);
}

template <typename T> void AddFileFactory() {
	file_factories[GetTypeEXT<T>()] = [](std::string fname) {
		Path path(fname);
		if (path.isAbsolutePath()) {
			T::Create(path);
		}
		else {
			T::Create(Path::assets / fname);
		}
	};
}
} // namespace tec
