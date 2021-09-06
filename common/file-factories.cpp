
#include "file-factories.hpp"
#include <spdlog/spdlog.h>

namespace tec {
std::unordered_map<std::string, std::function<void(std::string)>> file_factories;

void InvokeFileFactory(const std::string& file_name) {
	std::string ext = Path(file_name).FileExtension();
	auto factory = file_factories.find(ext);
	if (factory != file_factories.end()) {
		factory->second(file_name);
	}
	else {
		spdlog::get("console_log")->error("[FileFactory] No handler for extension: {}", ext);
		throw std::exception();
	}
}
} // namespace tec
