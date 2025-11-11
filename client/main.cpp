#include <iostream>
#include <numeric>
#include <regex>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <thread>

#include "application.hpp"
#include "default-config.hpp"
#include "file-factories.hpp"
#include "resources/md5anim.hpp"
#include "resources/md5mesh.hpp"
#include "resources/obj.hpp"
#include "resources/script-file.hpp"
#include "resources/vorbis-stream.hpp"

namespace tec {
void RegisterFileFactories() {
	AddFileFactory<MD5Mesh>();
	AddFileFactory<MD5Anim>();
	AddFileFactory<OBJ>();
	AddFileFactory<VorbisStream>();
	AddFileFactory<ScriptFile>();
}
void BuildTestVoxelVolume();

void InitializeLogger(spdlog::level::level_enum log_level) {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
	auto log = std::make_shared<spdlog::logger>("console_log", begin(sinks), end(sinks));
	log->set_level(log_level);
	log->set_pattern("%v"); // [%l] [thread %t] %v"); // Format on stdout
	spdlog::register_logger(log);
	log->info("Asset path: {}", Path::GetAssetsBasePath());
}

auto ParseLogLevel(int argc, char* argv[]) {
	auto log_level = spdlog::level::info;
	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "-v") {
			log_level = spdlog::level::debug;
		}
		else if (std::string(argv[i]) == "-vv") {
			log_level = spdlog::level::trace;
		}
	}
	return log_level;
}
} // namespace tec

int main(int argc, char* argv[]) {
	tec::InitializeLogger(tec::ParseLogLevel(argc, argv));
	tec::Application app;
	tec::RegisterFileFactories();
	app.Run();
	return 0;
}
