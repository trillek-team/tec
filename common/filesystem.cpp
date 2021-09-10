#include "filesystem.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <sstream>

#if defined(WIN32)
#define TEXT(a) L##a
#endif

#ifndef TEXT
#define TEXT(a) a
#endif

namespace tec {

const char WIN_PATH_CHAR = '\\'; /// Windows file system path separator

Path Path::settings_folder;
Path Path::udata_folder;
Path Path::cache_folder;
Path Path::assets_base;
const Path Path::invalid_path("*");

Path::Path() : device{}, path{} {}

Path::Path(const std::string& other, std::size_t pos, std::size_t count) : path(other, pos, count) {
	SetDevice();
	NormalizePath();
}

Path::Path(const std::string_view& other) : path(other) {
	SetDevice();
	NormalizePath();
}

Path::Path(std::string&& other) : path(other) {
	SetDevice();
	NormalizePath();
}

Path::Path(const std::wstring& other) {
	path = tec::utf8_encode(other);
	SetDevice();
	NormalizePath();
}

void Path::SetDevice() {
	const std::regex device_pattern("^[a-zA-Z]+:");
	std::smatch m;
	if (std::regex_search(path, m, device_pattern)) {
		device = m[0];
		size_t start = device.size();
		if (path.size() > start) {
			// make sure the path is absolute if it has a device
			if (path[start] != PATH_CHAR) {
				path[--start] = PATH_CHAR;
			}
			path.erase(0, start);
		}
		else {
			path.assign(PATH_SEPARATOR);
		}
	}
}

bool Path::FileExists() const { return std::ifstream(this->GetNativePath()).good(); }

bool Path::MkPath(const Path& path) {
	if (path.path.size() == 1 && path.path[0] == PATH_CHAR) { // we are at the root
		return true;
	}

	auto base = path.BasePath();
	// Recursively create the path
	if (base && !base.DirExists()) {
		MkPath(base);
	}
	// When the path is done
	return MkDir(path);
}

std::string Path::FileName() const {
	std::size_t pos = path.find_last_of(Path::PATH_CHAR);
	if (pos != std::string::npos) {
		return std::string(path, pos + 1);
	}
	return path;
}

std::string Path::FileStem() const {
	auto tmp = this->FileName();
	if (!tmp.empty()) {
		if (tmp.find(".") != std::string::npos) {
			return tmp.substr(0, tmp.find_last_of("."));
		}
	}
	return "";
}

std::string Path::FileExtension() const {
	auto tmp = this->FileName();
	if (!tmp.empty()) {
		if (tmp.find(".") != std::string::npos) {
			return tmp.substr(tmp.find_last_of(".") + 1);
		}
	}

	return "";
}

Path Path::BasePath() const {
	size_t len = path.size();
	if (!len) {
		return Path();
	}
	if (path.back() == PATH_CHAR) {
		len--;
	}
	size_t pos = path.find_last_of(PATH_CHAR, len);
	if (pos == std::string::npos) {
		return Path(std::string(), ".");
	}
	if (pos == 0) {
		pos = 1; // we hit the root of absolute path
	}

	return Path(device, path.substr(0, pos));
}

bool Path::isAbsolutePath() const {
	if (path.empty()) {
		return false;
	}
	return path[0] == PATH_CHAR;
}

Path Path::Subpath(size_t begin, size_t end) const {
	Path ret;
	std::istringstream f(this->path);
	std::string s;
	size_t count = 0;
	if (begin == 0) {
		ret.device = device;
		if (this->isAbsolutePath()) {
			ret.path = PATH_SEPARATOR;
		}
	}
	while (count < end && std::getline(f, s, PATH_CHAR)) {
		if (count == begin && begin > 0) {
			ret = s;
		}
		else if (count >= begin && count < end) {
			ret /= s;
		}
		count++;
	}

	return ret;
}

Path Path::SubpathFrom(const std::string& needle, bool include) const {
	Path ret;
	std::istringstream f(this->path);
	std::string s;
	bool found = false;
	bool first = true;
	while (std::getline(f, s, PATH_CHAR)) {
		if (found) {
			if (first) {
				ret = s;
				first = false;
			}
			else {
				ret /= s;
			}
		}
		else if (s.compare(needle) == 0) {
			found = true;
			if (include) {
				ret = s;
				first = false;
			}
		}
	}
	return ret;
}

void Path::NormalizePath() {
	if (path.empty()) {
		return;
	}
	std::replace(path.begin(), path.end(), WIN_PATH_CHAR, PATH_CHAR);
	// Prune duplicate path separators (like "///")
	// and handle collapsing relative paths:
	// "/foo/bar/../shaders/debug.vert" -> "/foo/shaders/debug.vert"
	// "/foo/././shaders//debug.vert" -> "/foo/shaders/debug.vert"
	std::istringstream f(this->path);
	std::string s;
	std::string last;
	std::list<std::string> npath;
	bool absolute = false;
	bool leadslash = false;
	if (path[0] == PATH_CHAR) {
		// processed path must start with a "/"
		absolute = true;
	}
	if (path.size() > 1 && path.back() == PATH_CHAR) {
		// processed path must end with a "/"
		leadslash = true;
	}
	while (std::getline(f, s, PATH_CHAR)) {
		if (s.empty() || (s == ".")) {
			continue; // prunes empty segments from duplicate separators
		}
		while ((s.size() > 2) && (s.back() == '.' || s.back() == ' ')) {
			s.pop_back(); // elements can't end with dot or space except the special . or ..
		}
		if (s == "..") {
			if (!npath.empty()) {
				npath.pop_back();
				continue;
			}
			else if (absolute) { // can't backup past the root
				continue;
			}
		}
		npath.push_back(s);
	}
	path.clear(); // rebuild the path
	if (absolute) {
		path.push_back(PATH_CHAR);
	}
	else if (npath.empty()) { // relative path without elements is "."
		path.push_back('.');
		if (leadslash) {
			path.push_back(PATH_CHAR);
		}
	}
	if (!npath.empty()) {
		path.append(npath.front());
		npath.pop_front();
		for (auto& element : npath) {
			path.push_back(PATH_CHAR);
			path.append(element);
		}
		const std::regex special("[\\x00-\\x1f:>*?\"<|]"); // if these appear in the path, it's invalid
		if (std::regex_search(path, special)) {
			if (device.empty() || device.back() != '*') {
				device.push_back('*'); // add a flag to mark this path as invalid
			}
		}
		if (leadslash) {
			path.push_back(PATH_CHAR);
		}
	}

}

Path::NativePath Path::GetNativePath() const {
	if (!*this) {
		throw PathException("invalid path"); // this is a known invalid path!
	}
	auto ldevice = device;
	std::for_each(ldevice.begin(), ldevice.end(), [](char e) { return std::tolower(e); });
	auto realpath = path;
	if (ldevice.size() > 2) {
		if (ldevice == "assets:") {
			auto asset = Path::GetAssetsBasePath() / Path(path);
			realpath = asset.path;
			ldevice = asset.device;
		}
		else {
			throw PathException("root device not available"); // not implemented
		}
	}
#if defined(WIN32)
	std::wstring npath{L"\\\\?\\"};
	if (ldevice.empty()) {
		// clear the special prefix, let the OS deal with the path
		npath.clear();
	}
	else {
		npath.resize(npath.size() + ldevice.size());
		std::copy(ldevice.begin(), ldevice.end(), npath.begin() + 4);
	}
	npath.append(tec::utf8_decode(realpath));
	std::replace(npath.begin(), npath.end(), PATH_CHAR, WIN_PATH_CHAR);
	return npath;
#else
	if (ldevice.size() > 0) {
		throw PathException("root device not available"); // no VFS and devices not implemented on unix
	}
	return realpath;
#endif
}

std::unique_ptr<FILE> Path::OpenFile(PATH_OPEN_FLAGS open_mode) const {
	if (!*this) {
		throw PathException("invalid path"); // this is a known invalid path!
	}
	Path::NativePath mode_str = TEXT("r");
	bool seek_to_end = false;
	if (open_mode & FS_READWRITE) {
		if (!(open_mode & FS_CREATE) && !this->FileExists()) {
			throw PathException("File does not exist, FS_CREATE not specified");
		}
		mode_str = TEXT("r+");
		if (open_mode & FS_CREATE) {
			mode_str = TEXT("w+");
			if (open_mode & FS_APPEND) {
				mode_str = TEXT("a+");
			}
		}
		else if (open_mode & FS_APPEND) {
			seek_to_end = true;
		}
	}
	else if (open_mode & (FS_CREATE | FS_APPEND)) {
		throw std::invalid_argument("invalid flags in open_mode");
	}
	mode_str.push_back(TEXT('b'));
	auto npath = this->GetNativePath();
#if defined(WIN32)
	FILE* file = _wfopen(npath.c_str(), mode_str.c_str());
#else
	FILE* file = fopen(npath.c_str(), mode_str.c_str());
#endif
	if (!file) { // didn't open file!
		throw PathException("File does not exist");
	}
	if (file && seek_to_end) {
		fseek(file, 0, SEEK_END);
	}
	// fopen returns a nullptr if it failed
	return std::unique_ptr<FILE>(file);
}

std::unique_ptr<std::fstream> Path::OpenStream(PATH_OPEN_FLAGS open_mode) const {
	if (!*this) {
		throw PathException("invalid path"); // this is a known invalid path!
	}
	std::ios_base::openmode mode = std::ios_base::in;
	if (open_mode & FS_READWRITE) {
		if (!(open_mode & FS_CREATE) && !this->FileExists()) {
			throw PathException("File does not exist, FS_CREATE not specified");
		}
		mode |= std::ios_base::out;
		if (open_mode & FS_APPEND) {
			mode |= std::ios_base::app | std::ios_base::ate;
		}
	}
	else if (open_mode & (FS_CREATE | FS_APPEND)) {
		throw std::invalid_argument("invalid flags in open_mode");
	}
	else if (!this->FileExists()) { // readonly, but the file doesn't exist
		throw PathException("File does not exist");
	}
	mode |= std::ios_base::binary;
	auto npath = this->GetNativePath();
	return std::make_unique<std::fstream>(npath, mode);
}

void Path::LocateAssets() {
	// Search for the assets folder
	Path search = Path::GetWorkingDir();
	search /= "assets/";
	if (search.DirExists()) {
		Path::assets_base = search.toString();
		return;
	}
	Path program = Path::GetProgramPath().BasePath();
	search = program / "assets/";
	if (search.DirExists()) {
		Path::assets_base = search.toString();
		return;
	}
	search = program.BasePath() / "share/assets/";
	if (search.DirExists()) {
		Path::assets_base = search.toString();
		return;
	}
	search = program.BasePath() / "share" / app_name / "assets/";
	if (search.DirExists()) {
		Path::assets_base = search.toString();
		return;
	}
	search = program.BasePath().BasePath() / "assets/";
	if (search.DirExists()) {
		Path::assets_base = search.toString();
		return;
	}
}

Path Path::GetAssetsBasePath() {
	if (Path::assets_base.empty()) {
		Path::LocateAssets();
		// If assets_base is still empty, then can't find the folder
		if (Path::assets_base.empty()) {
			throw PathException();
		}
	}
	return Path(Path::assets_base);
}

void Path::SetAssetsBasePath(Path new_base) { Path::assets_base = new_base.toString(); }

Path Path::GetAssetPath(const Path& asset) {
	auto tmp = Path("assets:/");
	tmp /= asset;
	return tmp;
}

Path Path::GetAssetPath(const std::string& asset) {
	auto tmp = Path("assets:/");
	tmp /= asset;
	return tmp;
}

Path Path::GetAssetPath(const char* asset) {
	auto tmp = Path("assets:/");
	tmp /= asset;
	return tmp;
}

} // namespace tec
