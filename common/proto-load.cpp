
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

bool SaveFromString(const FilePath& fname, std::string contents) {
	std::fstream output(fname.GetNativePath(), std::ios::out);
	if (!output.good())
		throw std::runtime_error("can't open ." + fname.toString());

	output.write(contents.c_str(), contents.length());
	output.close();
	return true;
}

// Loads a given entity json file
bool LoadProtoPack(const FilePath& fname, proto::Entity& entity) {
	auto _log = spdlog::get("console_log");
	if (!fname.isValidPath() || !fname.FileExists()) {
		_log->error("bad path or missing protopack file: {}", fname.toString());
		return false;
	}
	std::string json_string = LoadAsString(fname);
	// FIXME? allow loading binary protopacks as well maybe?
	auto status = google::protobuf::util::JsonStringToMessage(json_string, &entity);
	if (!status.ok()) {
		_log->error("LoadProtoPack: parsing file {} failed: {}", fname.toString(), status.ToString());
		return false;
	}
	return true;
}

// Loads a given json file into the system
void ProtoLoadEntity(const FilePath& fname) {
	std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
	if (LoadProtoPack(fname, data->entity)) {
		EventSystem<EntityCreated>::Get()->Emit(data);
	}
}

void ProtoLoad(std::string filename) {
	auto _log = spdlog::get("console_log");
	FilePath fname = FilePath::GetAssetPath(filename);
	if (!fname.isValidPath() || !fname.FileExists()) {
		_log->error("[ProtoLoad] Bad path or missing file: {}\n", fname.FileName());
		return;
	}
	std::string json_string = LoadAsString(fname);
	proto::EntityFileList elist;
	auto status = google::protobuf::util::JsonStringToMessage(json_string, &elist);
	if (!status.ok()) {
		_log->error("[ProtoLoad] parsing file {} failed: {}", fname.toString(), status.ToString());
		return;
	}
	_log->debug("[ProtoLoad] :\n {}", elist.DebugString());
	for (int i = 0; i < elist.entity_file_list_size(); i++) {
		FilePath entity_filename = FilePath::GetAssetPath(elist.entity_file_list(i));
		ProtoLoadEntity(entity_filename);
	}
}

} // namespace tec
