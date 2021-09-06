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
	static std::shared_ptr<ScriptFile> Create(const Path& fname);

	/**
	* \brief Returns a resource with the specified name.
	*
	* \param[in] const Path filename The filename of the image file to load.
	* \return bool True if initialization finished with no errors.
	*/
	bool Load(const Path& _filename);

	/**
	* \brief Factory method that creates a Script and stores it in the
	* ScriptMap under name. It will optionally load a script file with the given filename.
	* \param const std::string name The name to store the PixelBuffer under.
	* \param const Path filename The optional filename of the script to load.
	* \return std::shared_ptr<ScriptFile> The created PixelBuffer.
	*/
	static std::shared_ptr<ScriptFile> Create(const std::string name, const Path& filename = Path()) {
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
	Path filename;
	bool dirty{false};
};
} // namespace tec
