#include "filesystem.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

#include <sys/types.h>
#include <sys/stat.h>

// Stuff to get OS libs for paths
#if defined(__unix__)
#include <unistd.h>
#include <limits.h>

#if defined(__APPLE__)
#include <CoreServices/CoreServices.h>
#endif // __APPLE

#elif defined(WIN32)
#include <Windows.h>
#include <direct.h>
#include <Shlobj.h>
#include <comutil.h>

#pragma comment(lib, "comsuppw")
#endif

#ifndef PATH_MAX
#define PATH_MAX 1024 // same with Mac OS X's syslimits.h
#endif

namespace tec {

const std::string app_name(u8"trillek"); // TODO Ask to tec::OS for the appname ?
const char UNIX_PATH_SEPARATOR = '/';    /// *NIX filesystem path separator
const char WIN_PATH_SEPARATOR = '\\';    /// Windows filesystem path separator
#if defined(__unix__)
	const std::string FilePath::PATH_SEPARATOR = std::string("\\");
#else
	const std::string FilePath::PATH_SEPARATOR = std::string("/");
#endif
	
std::string FilePath::settings_folder = "";
std::string FilePath::udata_folder = "";
std::string FilePath::cache_folder = "";

std::string FilePath::assets_base = "";
	

FilePath::FilePath() 
	: path("")
{ }

FilePath::FilePath(const std::string& other, std::size_t pos, std::size_t count) 
: path(other, pos, count) 
{ 
	this->NormalizePath();
}

FilePath::FilePath(const std::wstring& other, std::size_t pos, std::size_t count) {
	auto ustr = tec::utf8_encode(other);
	this->path = ustr;
	this->NormalizePath();
}

FilePath FilePath::GetUserSettingsPath() {
	// Try to use cached value
	if (! FilePath::settings_folder.empty()) {
		return FilePath(FilePath::settings_folder);
	}
	FilePath ret;

#if defined(__unix__)
#if defined(__APPLE__)
	FSRef ref;
	FSFindFolder(kUserDomain, kApplicationSupportFolderType, kCreateFolder, &ref);
	char home[PATH_MAX];
	// FSMakeFSRefUnicode ?? This functions looks that are deprecated
	FSRefMakePath(&ref, (UInt8 *)&home, PATH_MAX);
	ret = home;
	ret /= app_name;
	ret += PATH_SEPARATOR;
#else
	char* home = getenv(u8"XDG_CONFIG_HOME");
	if (home == nullptr) {
		home = getenv(u8"HOME");
		if (home == nullptr) {
			return FilePath();
		}
	}
	ret = home;
	ret /= u8".config";
	ret /= app_name;
	ret += PATH_SEPARATOR;

#endif // __APPLE
#elif defined(WIN32)
	LPWSTR wszPath = NULL;

	if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &wszPath))) {
		return FilePath();
	}

	_bstr_t bstrPath(wszPath);
	std::wstring wpath((wchar_t*)bstrPath);
	ret = tec::utf8_encode(wpath);
	CoTaskMemFree(wszPath);
	ret /= app_name;
	ret += PATH_SEPARATOR;

#endif
	FilePath::settings_folder = ret.toString();
	return ret;

} // End of GetUserSettingsPath

FilePath FilePath::GetUserDataPath() {
	// Try to use cached value
	if (! FilePath::udata_folder.empty()) {
		return FilePath(FilePath::udata_folder);
	}

#if defined(__unix__)
	char* home = getenv(u8"XDG_DATA_HOME");
	if (home == nullptr) {
		home = getenv(u8"HOME");
		if (home == nullptr) {
			return FilePath();
		}
	}
	FilePath ret(home);
	ret /= u8".local";
	ret /= u8"share";
	ret /= app_name;
	ret += PATH_SEPARATOR;
	FilePath::udata_folder = ret.toString();

	return ret;
#elif defined(WIN32) || defined(__APPLE__)
	auto path = FilePath::GetUserSettingsPath();
	if (path.empty()) {
		return path;
	}
	path /= u8"data";
	path += PATH_SEPARATOR;
	FilePath::udata_folder = path.toString();
	return path;
#else
	return FilePath();
#endif

} // End of GetUserDataPath

FilePath FilePath::GetUserCachePath() {
	// Try to use cached value
	if (! FilePath::cache_folder.empty()) {
		return FilePath(FilePath::cache_folder);
	}
	
#if defined(__unix__)
#if defined(__APPLE__)
	auto path = GetUserSettingsPath();
	if (path.empty()) {
		return FilePath();
	}
	path /= u8"cache";
	path += PATH_SEPARATOR ;
#else
	char* home = getenv(u8"XDG_CACHE_HOME");
	if (home == nullptr) {
		home = getenv(u8"HOME");
		if (home == nullptr) {
			return FilePath();
		}
	}
	FilePath path(home);
	path /= u8".cache";
	path /= app_name;
	path += PATH_SEPARATOR;

#endif // __APPLE
#elif defined(WIN32)
	LPWSTR wszPath = NULL;

	if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &wszPath))) {
		return FilePath();
	}

	_bstr_t bstrPath(wszPath);
	std::wstring wpath((wchar_t*)bstrPath);
	FilePath path(tec::utf8_encode(wpath));
	CoTaskMemFree(wszPath);
	path /= app_name;
	path += PATH_SEPARATOR;

#endif
	FilePath::cache_folder = path.toString();
	return path;
} // End of GetUserCachePath


bool FilePath::DirExists() const {
#if defined(WIN32)
	struct __stat64 s;
	auto wtmp = this->GetNativePath();
	int err = _wstat64((wchar_t*)wtmp.c_str() , &s);
#else
	struct stat s;
	int err = stat(path.c_str(), &s);
#endif
	if (-1 == err) {
		// does not exist or error
		// if  errno == ENOENT --> not exist
		return false;
	} else {
		return true;
	}
}

bool FilePath::FileExists() const {
	return std::ifstream(this->GetNativePath()).good();
}

bool FilePath::MkDir(const FilePath& path) {
	if (!path.isValidPath()) {
		return false;
	}
	#if defined(__unix__)
	int ret = mkdir(path.GetNativePath().c_str(), 0755);
	#else // Windows
	int ret = _wmkdir(path.GetNativePath().c_str());
	#endif
	if (ret != 0 || errno == EEXIST) {
		return true;
	}
	return false;
}

bool FilePath::MkPath(const FilePath& path) {
	// Build the path on a recursive way
	if (! path.isValidPath()) {
		return false;
	}
#if defined(WIN32)
	if (path.path.size() <= 3 && std::isalpha( path.path.at(0))) { // 'X:\'
		return true;
	}
#else
	if (path.path.size() == 1 && path.path.at(0) == PATH_SEPARATOR_C) { // '\'
		return true;
	}
#endif
	auto base = path.BasePath();
	if (! base.empty()) {
		size_t len = base.path.size();
		if (MkPath(base)) {
			return MkDir(path);
		}
	}
	return false;
}

std::string FilePath::FileName() const {
	std::size_t pos = path.find_last_of( FilePath::PATH_SEPARATOR_C);
	if (pos != std::string::npos) {
		return std::string(path, pos+1);
	}
	return "";
}

std::string FilePath::FileExtension() const {
	auto tmp = this->FileName();
	if (! tmp.empty()) {
		if (tmp.find(".") != std::string::npos) {
			return tmp.substr(tmp.find_last_of(".") + 1);
		}
	}

	return "";
}

FilePath FilePath::BasePath() const {
	size_t len = path.size();
#if defined(WIN32)
	if (len <= 3 && std::isalpha( path.at(0))) { // 'X:\'
		return FilePath(path);
	}
#else
	if (len == 1 && path.at(0) == PATH_SEPARATOR_C) { // '\'
		return FilePath(path);
	}
#endif

	size_t pos = path.find_last_of( PATH_SEPARATOR_C);
	if (pos == len -1) {
		// Path ended with a path separator
		pos = path.find_last_of( PATH_SEPARATOR_C, path.size()-2);
	}
	if (pos == std::string::npos) {
		return FilePath();
	}
	
	return FilePath(path, 0, pos+1);
}

bool FilePath::isAbsolutePath() const {
	if (path.empty()) {
		return false;
	}
#if defined(WIN32)
	if (path.size() > 1) {
		return std::isalpha( path.at(0)) && path.at(1) == ':';
	}
	return false;
#else
	return path.at(0) == PATH_SEPARATOR_C;
#endif
}

FilePath FilePath::Subpath(size_t begin, size_t end) const {
	FilePath ret;
	std::istringstream f(this->path);
	std::string s;
	size_t count = 0;
	auto absoulte = this->isAbsolutePath();
	while (count < end && std::getline(f, s, PATH_SEPARATOR_C)) {
		if (count >= begin && count < end) {
#if defined(WIN32)
			if (count == 0 && absoulte) {
				ret = s;
			}
			else {
				ret /= s;
			}
#else
			ret /= s;
#endif
		}
		count++;
	}

	return ret;
}

FilePath FilePath::SubpathFrom(const std::string& needle, bool include) const {
	FilePath ret;
	std::istringstream f(this->path);
	std::string s;
	bool found = false;
	while (std::getline(f, s, PATH_SEPARATOR_C)) {
		if (found) {
			ret /= s;
		} else if (s.compare(needle) == 0) {
			found = true;
			if (include) {
				ret /= s;
			}
		}
	}

	return ret;
}

FilePath FilePath::GetProgramPath() {

#if defined(__unix__)
#if defined(__APPLE__)
	char tmp[PATH_MAX];
	uint32_t size = PATH_MAX;
	if (_NSGetExecutablePath(tmp, &size) == 0) {
		return FilePath(tmp);
	} else {
		// Too small buffer
		char* tmp2 = new char[size]();
		_NSGetExecutablePath(tmp2, &size)
		return FilePath(tmp2);
	}
#elif defined(__linux__)
	char tmp[PATH_MAX];
	int bytes = readlink("/proc/self/exe", tmp, PATH_MAX);
	bytes = bytes < PATH_MAX -1 ? bytes : PATH_MAX -1;
	if (bytes >= 0) {
		tmp[bytes] = '\0';
	}

	return FilePath(tmp);
#endif
	// Other *NIX have his proper API or changes on procfs
	//  * Mac OS X: _NSGetExecutablePath() (man 3 dyld)
	//  * Linux: readlink /proc/self/exe
	//  * Solaris: getexecname()
	//  * FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
	//  * FreeBSD if it has procfs: readlink /proc/curproc/file (FreeBSD doesn't have procfs by default)
	//  * NetBSD: readlink /proc/curproc/exe
	//  * DragonFly BSD: readlink /proc/curproc/file

#elif defined(WIN32)
	//LPWSTR buffer; //or wchar_t * buffer;
	wchar_t buffer[MAX_PATH];
	if (0 == GetModuleFileNameW(NULL, buffer, MAX_PATH)) {
		return FilePath();
	}
	std::wstring wstr(buffer);
	return FilePath(wstr);
#endif
}

void FilePath::NormalizePath() {
#if defined(WIN32)
	std::replace(path.begin(), path.end(), UNIX_PATH_SEPARATOR, WIN_PATH_SEPARATOR);
#else
	std::replace(path.begin(), path.end(), WIN_PATH_SEPARATOR, UNIX_PATH_SEPARATOR);
	if (path.size() > 2 && std::isalpha(path.at(0)) && path.at(1) == ':') { // x: windows unit to remove
		// This comparation it's safe on UTF-8 as isalpha would check [a-zA-Z] and Windows units are only letters
		path.erase(0, 2);
	}
#endif
	// TODO Check multiple path separators (like "\\\\") and handled relative path stuff like "\foo\bar\..\shaders\debug.vert"
}

bool  FilePath::isValidPath() const {
	if (! this->isAbsolutePath()) { // No contains drive, so check if is a valid relative path
		// TODO improve this with <regex>
		if (path.size() >= 3 && path.at(0) == '.' && (path.at(1) == PATH_SEPARATOR_C || (path.at(1) == '.' && path.at(2) == PATH_SEPARATOR_C))) {
			return true;
		} else {
			return false;
		}
	} else {
		return ! empty();
	}
}

FilePath::NFilePath FilePath::GetNativePath() const {
#if defined(WIN32)
	return tec::utf8_decode(path);
#else
	return path;
#endif
}

std::string FilePath::toGenericString() const {
	std::string ret(path);
	std::replace(ret.begin(), ret.end(), WIN_PATH_SEPARATOR, UNIX_PATH_SEPARATOR);
	return ret;
}

FilePath FilePath::GetAssetsBasePath() {
	if (FilePath::assets_base.empty()) {
		// Search for the assets folder
		FilePath tmp(u8"./assets/");
		if (tmp.DirExists()) {
			FilePath::assets_base = tmp.toString();
		} else {
			tmp = FilePath::GetProgramPath().BasePath() / u8"assets/";
			if (tmp.DirExists()) {
				FilePath::assets_base = tmp.toString();
			} else {
				tmp = tmp.BasePath().BasePath() / u8"share/assets/";
				if (tmp.DirExists()) {
					FilePath::assets_base = tmp.toString();
				} else {
					tmp = FilePath::GetProgramPath().BasePath().BasePath() / (u8"share/" + app_name + "/assets/");
					if (tmp.DirExists()) {
						FilePath::assets_base = tmp.toString();
					}
				}
			}
		}
		// If assets_base is empty, then can't find the folder
	}
	
	return FilePath(FilePath::assets_base);
}

void FilePath::SetAssetsBasePath(FilePath new_base) {
	FilePath::assets_base = new_base.toString();
}

FilePath FilePath::GetAssetPath(const FilePath& asset) {
	auto tmp = FilePath::GetAssetsBasePath();
	tmp /= asset;
	return tmp;
}

FilePath FilePath::GetAssetPath(const std::string& asset) {
	auto tmp =FilePath::GetAssetsBasePath();
	tmp /= asset;
	return tmp;
}

FilePath FilePath::GetAssetPath(const char* asset) {
	auto tmp =FilePath::GetAssetsBasePath();
	tmp /= asset;
	return tmp;
}

}
