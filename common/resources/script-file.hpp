#pragma once

#include <fstream>
#include <list>
#include <memory>
#include <streambuf>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "filesystem.hpp"
#include "multiton.hpp"

namespace tec {
class ScriptFile;
typedef Multiton<std::string, std::shared_ptr<ScriptFile>> ScriptMap;

class ScriptFile {
public:
	ScriptFile() = default;

	/**
	* \brief Returns a resource with the specified name.
	*
	* The only used initialization property is "filename".
	* \param[in] const std::vector<Property>& properties The creation properties for the resource.
	* \return std::shared_ptr<ScriptFile> The created ScriptFile resource.
	*/
	static std::shared_ptr<ScriptFile> Create(const FilePath& fname) {
		auto scriptfile = std::make_shared<ScriptFile>();
		scriptfile->SetName(fname.SubpathFrom("assets").toGenericString());

		if (scriptfile->Load(fname)) {
			ScriptMap::Set(scriptfile->GetName(), scriptfile);
			return scriptfile;
		}
		spdlog::get("console_log")->warn("[ScriptFile] Error loading script file {}", fname.toString());

		return nullptr;
	}

	/**
	* \brief Returns a resource with the specified name.
	*
	* \param[in] const FilePath filename The filename of the image file to load.
	* \return bool True if initialization finished with no errors.
	*/
	bool Load(const FilePath& filename) {
		auto _log = spdlog::get("console_log");
		if (filename.isValidPath() && filename.FileExists()) {
			std::fstream file(filename.GetNativePath(), std::ios_base::in);
			if (file.is_open()) {
				file.seekg(0, std::ios::end);
				script.reserve(static_cast<std::size_t>(file.tellg())); // Allocate string to the file size
				file.seekg(0, std::ios::beg);

				script = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				_log->trace("[Script] Read script file {} of {} bytes", filename.FileName(), script.length());
			}
			else {
				_log->warn("[Script] Error loading scriptfile {}", filename.FileName());
				return false;
			}
		}
		else {
			_log->warn("[Script] Error loading scriptfile {}. Invalid path.", filename.FileName());
			return false;
		}
		_log->trace("[Script] Loaded scriptfile {}", filename.FileName());
		return true;
	}

	/**
	* \brief Factory method that creates a Script and stores it in the
	* ScriptMap under name. It will optionally load a script file with the given filename.
	* \param const std::string name The name to store the PixelBuffer under.
	* \param const FilePath filename The optional filename of the script to load.
	* \return std::shared_ptr<ScriptFile> The created PixelBuffer.
	*/
	static std::shared_ptr<ScriptFile> Create(const std::string name, const FilePath& filename = FilePath()) {
		auto scr = std::make_shared<ScriptFile>();
		ScriptMap::Set(name, scr);
		if (!filename.empty()) {
			scr->Load(filename);
		}
		return scr;
	}

	ScriptFile(const ScriptFile&) = delete;
	ScriptFile& operator=(const ScriptFile&) = delete;

	ScriptFile(ScriptFile&& other) noexcept : name(std::move(other.name)), script(std::move(other.script)) {}

	ScriptFile& operator=(ScriptFile&& other) noexcept {
		this->name = std::move(other.name);
		this->script = std::move(other.script);
		return *this;
	}

	/**
	* \brief Returns a reference to the script text for reading
	*/
	const std::string& GetScript() const { return this->script; }

	/**
	* \brief Returns a reference to the script text
	*/
	std::string& GetScript() { return this->script; }

	const std::string GetName() const { return this->name; }
	void SetName(const std::string& _name) { this->name = _name; }

	bool IsDirty() const { return this->dirty; }
	/** \brief Mark dirty */
	void Invalidate() { this->dirty = true; }
	/** \brief Mark not dirty */
	void Validate() { this->dirty = false; }

protected:
	std::string name;
	std::string script;
	bool dirty{false};
};
} // namespace tec
