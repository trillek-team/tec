#include "filesystem.hpp"

#include "string.hpp"

#include <algorithm>
#include <fstream>
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

namespace tec {
namespace fs {

const std::string app_name(u8"trillek"); // TODO Ask to tec::OS for the appname ?
const char UNIX_PATH_SEPARATOR = '/';    /// *NIX filesystem path separator
const char WIN_PATH_SEPARATOR = '\\';	 /// Windows filesystem path separator


std::string GetUserSettingsPath() {
	static std::string settings_folder;

	// Try to use cached value
	if (!settings_folder.empty()) {
		return settings_folder;
	}
#if defined(__unix__)
#if defined(__APPLE__)
	FSRef ref;
	FSFindFolder(kUserDomain, kApplicationSupportFolderType, kCreateFolder, &ref);
	char home[PATH_MAX];
	// FSMakeFSRefUnicode ?? This functions looks that are deprecated
	FSRefMakePath(&ref, (UInt8 *)&home, PATH_MAX);
	std::string path(home);
	path += PATH_SEPARATOR;
	path += app_name;
	path += PATH_SEPARATOR;
#else
	char* home = getenv(u8"XDG_CONFIG_HOME");
	if (home == nullptr) {
		home = getenv(u8"HOME");
		if (home == nullptr) {
			return "";
		}
	}
	std::string path(home);
	path += PATH_SEPARATOR ;
	path += u8".config";
	path += PATH_SEPARATOR;
	path += app_name;
	path += PATH_SEPARATOR;

#endif // __APPLE
#elif defined(WIN32)
	LPWSTR wszPath = NULL;

	if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &wszPath))) {
		return "";
	}

	_bstr_t bstrPath(wszPath);
	std::wstring wpath((wchar_t*)bstrPath);
	std::string path = tec::utf8_encode(wpath);
	CoTaskMemFree(wszPath);
	path += PATH_SEPARATOR;
	path += app_name;
	path += PATH_SEPARATOR;

#endif
	settings_folder = path;
	return path;

} // End of GetUserSettingsPath


std::string GetUserDataPath() {
	static std::string udata_folder;

	// Try to use cached value
	if (!udata_folder.empty()) {
		return udata_folder;
	}
#if defined(__unix__)
	char* home = getenv(u8"XDG_DATA_HOME");
	if (home == nullptr) {
		home = getenv(u8"HOME");
		if (home == nullptr) {
			return "";
		}
	}
	std::string path(home);
	path += PATH_SEPARATOR;
	path += u8".local/share";
	path += PATH_SEPARATOR;
	path += app_name;
	path += PATH_SEPARATOR;

#elif defined(WIN32) || defined(__APPLE__)
	std::string path = GetUserSettingsPath();
	if (path.empty()) {
		return "";
	}
	path += u8"data";
	path += PATH_SEPARATOR;
#endif
	udata_folder = path;
	return path;

} // End of GetUserDataPath

std::string GetUserCachePath() {
	static std::string cache_folder;

	// Try to use cached value
	if (!cache_folder.empty()) {
		return cache_folder;
	}
#if defined(__unix__)
#if defined(__APPLE__)
	std::string path = GetUserSettingsPath();
	if (path.empty()) {
		return "";
	}
	path += u8"cache";
	path += PATH_SEPARATOR ;
#else
	char* home = getenv(u8"XDG_CACHE_HOME");
	if (home == nullptr) {
		home = getenv(u8"HOME");
		if (home == nullptr) {
			return "";
		}
	}
	std::string path(home);
	path += PATH_SEPARATOR;
	path += u8".cache";
	path += PATH_SEPARATOR;
	path += app_name;
	path += PATH_SEPARATOR;

#endif // __APPLE
#elif defined(WIN32)
	LPWSTR wszPath = NULL;

	if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &wszPath))) {
		return "";
	}

	_bstr_t bstrPath(wszPath);
	std::wstring wpath((wchar_t*)bstrPath);
	std::string path = tec::utf8_encode(wpath);
	CoTaskMemFree(wszPath);
	path += PATH_SEPARATOR;
	path += app_name;
	path += PATH_SEPARATOR;

#endif
	cache_folder = path;
	return path;
} // End of GetUserCachePath


bool DirExists(const std::string& path) {

#if defined(WIN32)
	struct __stat64 s;
	auto wtmp = GetNativePath(path);
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
		/*
		if ((s.st_mode & _S_IFDIR) > 0) {
			// Is a dir
			return true;
		}
		*/
	}
}

bool FileExists(const std::string& path) {
#if defined(__linux__)
	return std::ifstream(path).good();
#else
	return std::ifstream(GetNativePath(path)).good();
#endif
}

/**
 * Does the dirty job of creating a dir
 */
inline bool _MkDir(const std::string& path) {
#if defined(__unix__)
	int ret = mkdir(GetNativePath(path).c_str(), 0755);
#else // Windows
	int ret = _wmkdir(GetNativePath(path).c_str());
#endif
	if (ret != 0 && errno == EEXIST) {
		return true;
	}
	return ret != 0;
}

bool MkDir(const std::string& path) {
	std::string tmp(path);
	NormalizePath(tmp);
	return _MkDir(tmp);
}

bool MkPath(const std::string& path) {
	// Build the path on a recursive way
#if defined(WIN32)
		if (path.size() <= 3 && std::isalpha( path.at(0))) { // 'X:\'
			return true;
		}
#else
		if (path.size() == 1 && path.at(0) == PATH_SEPARATOR_C) { // '\'
			return true;
		}
#endif
	auto base = BasePath(path);
	if (! base.empty()) {
		size_t len = base.size();
		if (MkPath(base)) {
			return _MkDir(path);
		}
	}
	return false;
}


std::string FileName(const std::string& path) {
	std::string tmp(path);
	NormalizePath(tmp);
	size_t pos = tmp.find_last_of( PATH_SEPARATOR_C);
	if (pos != std::string::npos) {
		std::string name(tmp, pos+1);
		return name;
	}
	return "";
}

std::string BasePath(const std::string& path) {
	std::string tmp(path);
	NormalizePath(tmp);
	size_t len = tmp.size();
#if defined(WIN32)
	if (len <= 3 && std::isalpha( tmp.at(0))) { // 'X:\'
		return tmp;
	}
#else
	if (len == 1 && tmp.at(0) == PATH_SEPARATOR_C) { // '\'
		return tmp;
	}
#endif

	size_t pos = tmp.find_last_of( PATH_SEPARATOR_C);
	if (pos == len -1) {
		// Path ended with a path separator
		pos = tmp.find_last_of( PATH_SEPARATOR_C, tmp.size()-2);
	}
	if (pos == std::string::npos) {
		return "";
	}

	tmp.erase(pos+1);
	return tmp;
}

bool isAbsolutePath(const FilePath& path) {
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

std::string GetProgramPath() {

#if defined(__unix__)
	const size_t LEN = 512;
#if defined(__APPLE__)
	char tmp[LEN];
	uint32_t size = LEN;
	if (_NSGetExecutablePath(tmp, &size) == 0) {
		std::string path(tmp);
		return path;
	} else {
		// Too small buffer
		char* tmp2 = new char[size]();
		_NSGetExecutablePath(tmp2, &size)
		std::string path(tmp2);
		return path;
	}
#elif defined(__linux__)
	char szTmp[LEN];
	char tmp[LEN];
	snprintf(szTmp, LEN, "/proc/%d/exe", getpid());
	int bytes = readlink(szTmp, tmp, LEN);
	bytes = bytes < LEN-1 ? bytes : LEN-1;
	if (bytes >= 0) {
		tmp[bytes] = '\0';
	}

	std::string path(tmp);
	return path;
#endif
	/* Other *NIX have his proper API or changes on procfs
	 * Mac OS X: _NSGetExecutablePath() (man 3 dyld)
	 * Linux: readlink /proc/self/exe
	 * Solaris: getexecname()
	 * FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
	 * FreeBSD if it has procfs: readlink /proc/curproc/file (FreeBSD doesn't have procfs by default)
	 * NetBSD: readlink /proc/curproc/exe
	 * DragonFly BSD: readlink /proc/curproc/file
	 */

#elif defined(WIN32)
	//LPWSTR buffer; //or wchar_t * buffer;
	wchar_t buffer[MAX_PATH];
	if (0 == GetModuleFileNameW(NULL, buffer, MAX_PATH)) {
		return "";
	}
	std::wstring str(buffer);
	return tec::utf8_encode(str);
#endif
}

void NormalizePath(std::string& path) {
#if defined(WIN32)
	std::replace(path.begin(), path.end(), UNIX_PATH_SEPARATOR, WIN_PATH_SEPARATOR);
#else
	std::replace(path.begin(), path.end(), WIN_PATH_SEPARATOR, UNIX_PATH_SEPARATOR);
	if (std::isalpha(path.at(0)) && path.at(1) == ':') { // x: windows unit to remove
		// This comparation it's safe on UTF-8 as isalpha would check [a-zA-Z] and Windows units are only letters
		path.erase(0, 2);
	}
#endif
}

NFilePath GetNativePath(const std::string& path) {
	std::string tmp(path);
	NormalizePath(tmp);
#if defined(WIN32)
	return tec::utf8_decode(tmp);
#else
	return tmp;
#endif
}

}
}

