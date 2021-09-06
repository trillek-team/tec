#include "filesystem.hpp"

// Stuff to get OS libs for paths
#if defined(__APPLE__)
#include <CoreServices/CoreServices.h>
#include <mach-o/dyld.h>
#include <unistd.h>

#elif defined(__unix__)
#include <limits.h>
#include <unistd.h>

#elif defined(WIN32)
#ifndef UNICODE
#define UNICODE 1
#endif
#include <Shlobj.h>
#include <Windows.h>
#include <comutil.h>
#include <direct.h>
#undef max
#undef min

#pragma comment(lib, "comsuppw")
#endif

#ifndef PATH_MAX
#define PATH_MAX 1024 // same with Mac OS X's syslimits.h
#endif

#ifndef TEXT
#define TEXT(a) a
#endif

namespace tec {

Path Path::GetUserSettingsPath() {
	// Try to use cached value
	if (!Path::settings_folder.empty()) {
		return Path(Path::settings_folder);
	}
	Path ret;

#if defined(__APPLE__)
	char* home = getenv("HOME");
	ret = home;
	ret /= "Library";
	ret /= "Preferences";
	ret /= app_name;
	ret.path += PATH_SEPARATOR;

#elif defined(__unix__)
	char* home = getenv("XDG_CONFIG_HOME");
	if (home == nullptr) {
		home = getenv("HOME");
		if (home == nullptr) {
			return Path();
		}
	}
	ret = home;
	ret /= ".config";
	ret /= app_name;
	ret.path += PATH_SEPARATOR;

#elif defined(WIN32)
	LPWSTR wszPath = nullptr;

	if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &wszPath))) {
		return Path();
	}

	_bstr_t bstrPath(wszPath);
	std::wstring wpath((wchar_t*)bstrPath);
	ret = tec::utf8_encode(wpath);
	CoTaskMemFree(wszPath);
	ret /= app_name;
	ret.path += PATH_SEPARATOR;

#endif
	Path::MkPath(ret);
	Path::settings_folder = ret.toString();
	return ret;

} // End of GetUserSettingsPath

Path Path::GetUserDataPath() {
	// Try to use cached value
	if (!Path::udata_folder.empty()) {
		return Path(Path::udata_folder);
	}

#if defined(__unix__)
	char* home = getenv("XDG_DATA_HOME");
	if (home == nullptr) {
		home = getenv("HOME");
		if (home == nullptr) {
			return Path();
		}
	}
	Path ret = Path(std::string(home));
	ret /= ".local/share";
	ret /= app_name;
	ret.path += PATH_SEPARATOR;
	Path::udata_folder = ret.toString();

	Path::MkPath(ret);
	return ret;
#elif defined(WIN32) || defined(__APPLE__)
	auto path = Path::GetUserSettingsPath();
	if (path.empty()) {
		return path;
	}
	path /= "data";
	path.path += PATH_SEPARATOR;
	Path::udata_folder = path.toString();

	Path::MkPath(path);
	return path;
#else
	return Path();
#endif

} // End of GetUserDataPath

Path Path::GetUserCachePath() {
	// Try to use cached value
	if (!Path::cache_folder.empty()) {
		return Path(Path::cache_folder);
	}

#if defined(__APPLE__)
	auto path = GetUserSettingsPath();
	if (path.empty()) {
		return Path();
	}
	path /= "cache";
	path.path += PATH_SEPARATOR;
#elif defined(__unix__)
	char* home = getenv("XDG_CACHE_HOME");
	if (home == nullptr) {
		home = getenv("HOME");
		if (home == nullptr) {
			return Path();
		}
	}
	Path path = Path(std::string(home));
	path /= ".cache";
	path /= app_name;
	path.path += PATH_SEPARATOR;
#elif defined(WIN32)
	LPWSTR wszPath = nullptr;

	if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &wszPath))) {
		return Path();
	}

	_bstr_t bstrPath(wszPath);
	std::wstring wpath((wchar_t*)bstrPath);
	Path path(tec::utf8_encode(wpath));
	CoTaskMemFree(wszPath);
	path /= app_name;
	path.path += PATH_SEPARATOR;

#endif
	Path::cache_folder = path.toString();
	Path::MkPath(path);
	return path;
} // End of GetUserCachePath

Path Path::GetProgramPath() {
#if defined(__APPLE__)
	std::string tmp;
	tmp.resize(PATH_MAX);
	uint32_t size = tmp.size();
	if (_NSGetExecutablePath(tmp.data(), &size) == 0) {
		tmp.resize(size);
		return Path(tmp);
	}
	else {
		// Too small buffer
		tmp.resize(size);
		_NSGetExecutablePath(tmp.data(), &size);
		return Path(tmp);
	}
#elif defined(__linux__)
	std::string tmp;
	tmp.resize(PATH_MAX);
	ssize_t bytes = readlink("/proc/self/exe", tmp.data(), tmp.size());
	if (bytes == -1) {
		return Path();
	}
	tmp.resize(bytes);
	return Path(tmp);
#elif defined(WIN32)
	std::wstring wstr;
	wstr.resize(MAX_PATH);
	wstr.resize(GetModuleFileNameW(nullptr, wstr.data(), (DWORD)wstr.size()));

	return Path(wstr);
#else
	// Other *NIX have his proper API or changes on procfs
	//  * Mac OS X: _NSGetExecutablePath() (man 3 dyld)
	//  * Linux: readlink /proc/self/exe
	//  * Solaris: getexecname()
	//  * FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
	//  * FreeBSD if it has procfs: readlink /proc/curproc/file (FreeBSD doesn't have procfs by
	//  default)
	//  * NetBSD: readlink /proc/curproc/exe
	//  * DragonFly BSD: readlink /proc/curproc/file
#endif
}

} // namespace tec
