#include "filesystem.hpp"

#include <algorithm>

// Stuff to get OS libs for paths
#ifdef __unix__
#ifdef __APPLE__
#include <CoreServices/CoreServices.h>
#endif

#include <sys/stat.h>

#elif defined(WIN32)
#include <Shlobj.h>
#include <comutil.h>

#pragma comment(lib, "comsuppw")
#endif

namespace tec {
namespace fs {

const std::string app_name("trillek");

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
	FSRefMakePath(&ref, (UInt8 *)&home, PATH_MAX);
	std::string path(home);
	path += PATH_SEPARATOR;
	path += app_name;
	path += PATH_SEPARATOR;
#else
	// Try to use a secure version of getenv
#if defined(_GNU_SOURCE)
	char* home = secure_getenv("XDG_CONFIG_HOME");
	if (home == nullptr) {
		home = secure_getenv("HOME");
		if (home == nullptr) {
			return "";
		}
	}
#else
	char* home = getenv("XDG_CONFIG_HOME");
	if (home == nullptr) {
		home = getenv("HOME");
		if (home == nullptr) {
			return "";
		}
	}
#endif
	std::string path(home);
	path += PATH_SEPARATOR ;
	path += ".config";
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
	std::string path((char*)bstrPath);
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
	// Try to use a secure version of getenv
#if defined(_GNU_SOURCE)
	char* home = secure_getenv("XDG_DATA_HOME");
	if (home == nullptr) {
		home = secure_getenv("HOME");
		if (home == nullptr) {
			return "";
		}
	}
#else
	char* home = getenv("XDG_DATA_HOME");
	if (home == nullptr) {
		home = getenv("HOME");
		if (home == nullptr) {
			return "";
		}
	}
#endif
	std::string path(home);
	path += PATH_SEPARATOR;
	path += ".local/share";
	path += PATH_SEPARATOR;
	path += app_name;
	path += PATH_SEPARATOR;

#elif defined(WIN32) || defined(__APPLE__)
	std::string path = GetUserSettingsPath();
	if (path.empty()) {
		return "";
	}
	path += "data";
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
	path += "cache";
	path += PATH_SEPARATOR ;
#else
	// Try to use a secure version of getenv
#if defined(_GNU_SOURCE)
	char* home = secure_getenv("XDG_CACHE_HOME");
	if (home == nullptr) {
		home = secure_getenv("HOME");
		if (home == nullptr) {
			return "";
		}
	}
#else
	char* home = getenv("XDG_CACHE_HOME");
	if (home == nullptr) {
		home = getenv("HOME");
		if (home == nullptr) {
			return "";
		}
	}
#endif
	std::string path(home);
	path += PATH_SEPARATOR;
	path += ".cache";
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
	std::string path((char*)bstrPath);
	CoTaskMemFree(wszPath);
	path += PATH_SEPARATOR;
	path += app_name;
	path += PATH_SEPARATOR;

#endif
	cache_folder = path;
	return path;
} // End of GetUserCachePath

}
}

