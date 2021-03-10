
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>

#include <google/protobuf/util/json_util.h>

#include "proto-load.hpp"

namespace tec {

std::string LoadAsString(const FilePath& fname) {
	std::fstream input(fname.GetNativePath(), std::ios::in | std::ios::binary);
	if (!input.good())
		throw std::runtime_error("can't open ." + fname.toString());

	std::string in;
	input.seekg(0, std::ios::end);
	in.reserve(static_cast<std::size_t>(input.tellg()));
	input.seekg(0, std::ios::beg);
	std::copy((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>(), std::back_inserter(in));
	input.close();
	return in;
}

// Loads a given entity json file
bool LoadProtoPack(const FilePath& fname, proto::Entity& entity) {
	auto _log = spdlog::get("console_log");
	if (fname.isValidPath() && fname.FileExists()) {
		std::string json_string = LoadAsString(fname);
		// FIXME? allow loading binary protopacks as well maybe?
		auto status = google::protobuf::util::JsonStringToMessage(json_string, &entity);
		if (!status.ok()) {
			_log->error("LoadProtoPack: parsing file {} failed: {}", fname.toString(), status.ToString());
			return false;
		}
		return true;
	}

	_log->error("failed to load protopack file {}", fname.toString());
	return false;
}

// Loads a given json file into the system
void ProtoLoadEntity(const FilePath& fname) {
	std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
	if (LoadProtoPack(fname, data->entity)) {
		data->entity_id = data->entity.id();
		EventSystem<EntityCreated>::Get()->Emit(data);
	}
}

} // namespace tec
