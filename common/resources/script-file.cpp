#include "script-file.hpp"

namespace tec {

std::shared_ptr<ScriptFile> ScriptFile::Create(const Path& fname) {
	auto scriptfile = std::make_shared<ScriptFile>();
	scriptfile->SetName(fname.SubpathFrom("scripts", true).toString());

	if (scriptfile->Load(fname)) {
		ScriptMap::Set(scriptfile->GetName(), scriptfile);
		return scriptfile;
	}
	spdlog::get("console_log")->warn("[ScriptFile] Error loading script file {}", fname.toString());

	return nullptr;
}

bool ScriptFile::Load(const Path& _filename) {
	auto _log = spdlog::get("console_log");
	if (!_filename.isValidPath() || !_filename.FileExists()) {
		_log->warn("[Script] Error loading scriptfile {}. Invalid path.", _filename.FileName());
		return false;
	}
	auto file = _filename.OpenStream();
	if (!file->is_open()) {
		_log->warn("[Script] Error loading scriptfile {}", _filename.FileName());
		return false;
	}
	file->seekg(0, std::ios::end);
	script.reserve(static_cast<std::size_t>(file->tellg())); // Allocate string to the file size
	file->seekg(0, std::ios::beg);

	script = std::string((std::istreambuf_iterator<char>(*file)), std::istreambuf_iterator<char>());
	_log->trace("[Script] Read script file {} of {} bytes", _filename.FileName(), script.length());
	this->filename = _filename;

	_log->debug("[Script] Loaded scriptfile {}", _filename.FileName());
	return true;
}

} // namespace tec
