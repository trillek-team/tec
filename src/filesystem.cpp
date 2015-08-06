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


std::string GetUserSettingsPath() {
	const std::string app_name("trillek");
	static std::string settings_file;

	// Try to use cached value
	if (!settings_file.empty()) {
		return settings_file;
	}
#if defined(__unix__)
#if defined(__APPLE__)
	FSRef ref;
	FSFindFolder(kUserDomain, kApplicationSupportFolderType, kCreateFolder, &ref);
	char home[PATH_MAX];
	FSRefMakePath(&ref, (UInt8 *)&home, PATH_MAX);
	std::string path(home);
	path += PATH_SEPARATOR + app_name + PATH_SEPARATOR;
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
	path += PATH_SEPARATOR + ".config" + PATH_SEPARATOR + app_name + path + PATH_SEPARATOR;

#endif // __APPLE
#elif defined(WIN32)
	LPWSTR wszPath = NULL;

	if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &wszPath))) {
		return "";
	}

	_bstr_t bstrPath(wszPath);
	std::string path((char*)bstrPath);
	CoTaskMemFree(wszPath);
	path += PATH_SEPARATOR + app_name + PATH_SEPARATOR;

#endif
	settings_file = path;
	return path;

} // End of GetUserSettingsPath


}
}

