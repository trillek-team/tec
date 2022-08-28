#include "filesystem.hpp"

#include <iostream>
#include <sstream>

#include <sys/stat.h>
#include <sys/types.h>

// Stuff to get OS libs for paths
#if defined(__APPLE__)
#include <CoreServices/CoreServices.h>
#include <mach-o/dyld.h>
#endif

#if defined(__unix__) || defined(__APPLE__)
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#elif defined(WIN32)
#ifndef UNICODE // get wide character definitions for things
#define UNICODE 1
#endif
#include <Shlobj.h> // for SHGetKnownFolderPath
#include <Windows.h>
#include <comutil.h> // CoTaskMemFree
#include <direct.h> // Posix like functions
// Windows.h does evil things with macros
#undef max
#undef min
#pragma comment(lib, "comsuppw")

#endif

#ifndef PATH_MAX
#define PATH_MAX 1024 // fall back if undefinied, based on OS X <sys/syslimits.h>
#endif

#ifndef TEXT
#define TEXT(a) a
#endif

namespace tec {

bool Path::DirExists() const {
#if defined(WIN32)
	auto npath = this->GetNativePath();
	// Trailing slash is not allowed
	if (npath.size() && npath.back() == L'\\') {
		npath.pop_back();
	}
	DWORD fa = GetFileAttributesW(npath.c_str());
	// is what we found actually a directory?
	return (fa != INVALID_FILE_ATTRIBUTES) && (0 != (fa & FILE_ATTRIBUTE_DIRECTORY));
#else
	struct stat s;
	auto npath = this->GetNativePath();
	int err = stat(npath.c_str(), &s);
	// does not exist, error, or is not a directory
	return (-1 != err) && (0 != (s.st_mode & S_IFDIR));
	// errno == ENOENT --> not exist
#endif
}

bool Path::MkDir(const Path& path) {
	if (!path) {
		return false;
	}
#if defined(__unix__) || defined(__APPLE__)
	int ret = mkdir(path.GetNativePath().c_str(), 0755);
	return ret == 0 || errno == EEXIST;
#elif defined(WIN32)
	int ret = CreateDirectoryW(path.GetNativePath().c_str(), NULL);
	return ret || (GetLastError() == ERROR_ALREADY_EXISTS);
#else
#error must implement Path::MkDir() for this OS
#endif
}

void Path::Listing(const Path& path) {
	if (!path) {
		return;
	}
#if defined(WIN32)
	WIN32_FIND_DATAW s;
	auto npath = path.GetNativePath();
	if (npath.back() != L'\\') {
		npath.push_back(L'\\');
	}
	npath.push_back(L'*');
	HANDLE search = FindFirstFileW(npath.c_str(), &s);
	if (search == INVALID_HANDLE_VALUE) {
		return;
	}
	do {
		char ftype = '-';
		if (s.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
			ftype = 'l';
		}
		if (s.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			ftype = 'd';
		}
		if (s.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) {
			ftype = 'v';
		}
		std::wcout << "[" << ftype << ":" << ((s.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) ? "A" : "-")
				   << ((s.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) ? "C" : "-")
				   << ((s.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) ? "E" : "-")
				   << ((s.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) ? "H" : "-")
				   << ((s.dwFileAttributes & FILE_ATTRIBUTE_READONLY) ? "R" : "-")
				   << ((s.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) ? "S" : "-") << "] " << path << "\\" << s.cFileName
				   << '\n';
		if (0 == lstrcmpW(s.cFileName, L".") || 0 == lstrcmpW(s.cFileName, L"..")) {
			continue;
		}
		Path::Listing(path / std::wstring(s.cFileName));
	} while (FindNextFileW(search, &s));
	FindClose(search);
	return;
#else
	auto npath = path.GetNativePath();
	if (npath.back() != L'/') {
		npath.push_back(L'/');
	}
	DIR* dir = opendir(npath.c_str());
	if (!dir) {
		return;
	}
	dirent* entry = readdir(dir);
	int dir_fd = dirfd(dir);
	struct stat s;
	while ((entry = readdir(dir))) {
		int err = fstatat(dir_fd, entry->d_name, &s, 0);
		if (0 == strcmp(entry->d_name, ".") || 0 == strcmp(entry->d_name, "..")) {
			continue;
		}
		if (!err) {
			err = faccessat(dir_fd, entry->d_name, R_OK | W_OK, 0);
			char ftype = '?';
			switch (s.st_mode & S_IFMT) {
			case S_IFIFO: ftype = 'f'; break;
			case S_IFCHR: ftype = 'c'; break;
			case S_IFDIR: ftype = 'd'; break;
			case S_IFBLK: ftype = 'b'; break;
			case S_IFREG: ftype = '-'; break;
			case S_IFLNK: ftype = 'l'; break;
			case S_IFSOCK: ftype = 's'; break;
			}
			std::cout << "[" << ftype << ':' << (err ? "ro" : "rw") << "] " << npath << "|" << entry->d_name << '\n';
			if (ftype == 'd') {
				Path::Listing(path / std::string(entry->d_name));
			}
		}
	}
	closedir(dir);
#endif
}

#if defined(WIN32)
std::string winstrerror(DWORD errcode) {
	LPSTR msgbuf = nullptr;
	FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			errcode,
			0,
			(LPSTR)&msgbuf,
			0x8000,
			NULL);
	if (!msgbuf) {
		return {};
	}
	std::string ret(msgbuf);
	LocalFree(msgbuf);
	return ret;
}
std::wstring winwstrerror(DWORD errcode) {
	LPWSTR msgbuf = nullptr;
	FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			errcode,
			0,
			(LPWSTR)&msgbuf,
			0x8000,
			NULL);
	if (!msgbuf) {
		return {};
	}
	std::wstring ret(msgbuf);
	LocalFree(msgbuf);
	return ret;
}
#endif

void Path::Remove(const Path& path) {
	if (!path) {
		throw PathException("invalid path");
	}
#if defined(WIN32)
	auto npath = path.GetNativePath();
	if (npath.back() == L'\\') {
		npath.pop_back();
	}
	DWORD fa = GetFileAttributesW(npath.c_str());
	if (fa == INVALID_FILE_ATTRIBUTES) {
		std::wcout << "Can not remove file: " << npath << " : " << winwstrerror(GetLastError()) << '\n';
		return;
	}
	const char* ftype = "file";
	if (fa & FILE_ATTRIBUTE_REPARSE_POINT) {
		ftype = "link";
	}
	int result = 0;
	if (fa & FILE_ATTRIBUTE_DIRECTORY) {
		ftype = "directory";
		result = RemoveDirectoryW(npath.c_str());
	}
	else {
		result = DeleteFileW(npath.c_str());
	}
	if (!result) {
		std::stringstream msg;
		msg << "Delete " << ftype << " failed: " << utf8_encode(npath) << " : " << winstrerror(GetLastError()) << '\n';
		throw PathException(msg.str());
	}
#else
	struct stat s;
	auto npath = path.GetNativePath();
	int err = stat(npath.c_str(), &s);
	mode_t ftype = (s.st_mode & S_IFMT);
	if (err) {
		throw PathException(strerror(errno));
	}
	if (!(ftype == S_IFREG || ftype == S_IFDIR || ftype == S_IFLNK || ftype == S_IFSOCK)) {
		// you really shouldn't be using the tec::Path API to deal with fifos/device nodes
		// even symlinks and sockets are a bit of a stretch.
		throw PathException("Not a removable object");
	}
	if ((s.st_mode & S_IFMT) == S_IFDIR) {
		if (rmdir(npath.c_str())) {
			throw PathException(strerror(errno));
		}
	}
	else if (unlink(npath.c_str())) {
		throw PathException(strerror(errno));
	}
#endif
}

size_t Path::RemoveAll(const Path& path) {
	if (!path) {
		throw PathException("invalid path");
	}
#if defined(WIN32)
	auto npath = path.GetNativePath();
	if (npath.back() == L'\\') {
		npath.pop_back();
	}
	DWORD fa = GetFileAttributesW(npath.c_str());
	if (fa == INVALID_FILE_ATTRIBUTES) {
		throw PathException(winstrerror(GetLastError()));
	}
	if (0 == (fa & FILE_ATTRIBUTE_DIRECTORY)) { // not a directory
		Remove(path);
		return 1;
	}
#else
	struct stat s;
	auto npath = path.GetNativePath();
	int err = stat(npath.c_str(), &s);
	if (err) {
		throw PathException(strerror(errno));
	}
	if ((s.st_mode & S_IFMT) != S_IFDIR) { // not a directory
		Remove(path);
		return 1;
	}
#endif
	Path::CheckRemoveAll(path);
	return RemoveAll_Internal(path);
}

size_t Path::RemoveAll_Internal(const Path& path) {
	size_t removed_count = 0;
#if defined(WIN32)
	WIN32_FIND_DATAW s;
	auto npath = path.GetNativePath();
	if (npath.back() != L'\\') {
		npath.push_back(L'\\');
	}
	npath.push_back(L'*');
	HANDLE search = FindFirstFileW(npath.c_str(), &s);
	if (search == INVALID_HANDLE_VALUE) {
		throw PathException("can not access path");
	}
	do {
		if (0 == lstrcmpW(s.cFileName, L".") || 0 == lstrcmpW(s.cFileName, L"..")) {
			continue;
		}
		auto rmpath = path / std::wstring(s.cFileName);
		auto rmnpath = rmpath.GetNativePath();
		if (s.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			removed_count += Path::RemoveAll_Internal(rmpath);
		}
		else {
			std::wcout << "removing " << rmnpath << '\n';
			// remove file
			if (!DeleteFileW(rmnpath.c_str())) {
				std::stringstream msg;
				msg << "Delete file failed: " << utf8_encode(rmnpath) << " : " << winstrerror(GetLastError()) << '\n';
				throw PathException(msg.str());
			}
			removed_count++;
		}
	} while (FindNextFileW(search, &s));
	FindClose(search);
	npath.pop_back(); // remove the slash star off the end
	npath.pop_back();
	std::wcout << "removing directory " << npath << '\n';
	// remove directory
	if (!RemoveDirectoryW(npath.c_str())) {
		std::stringstream msg;
		msg << "Delete file failed: " << utf8_encode(npath) << " : " << winstrerror(GetLastError()) << '\n';
		throw PathException(msg.str());
	}
	removed_count++;
#else
	auto npath = path.GetNativePath();
	if (npath.back() != L'/') {
		npath.push_back(L'/');
	}
	DIR* dir = opendir(npath.c_str());
	if (!dir) {
		throw PathException("can not access path");
	}
	dirent* entry = readdir(dir);
	int dir_fd = dirfd(dir);
	struct stat s;
	while ((entry = readdir(dir))) {
		int err = fstatat(dir_fd, entry->d_name, &s, 0);
		if (0 == strcmp(entry->d_name, ".") || 0 == strcmp(entry->d_name, "..")) {
			continue;
		}
		if (!err) {
			mode_t ftype = (s.st_mode & S_IFMT);
			auto rmpath = path / std::string(entry->d_name);
			if (ftype == S_IFDIR) {
				// recurse to remove directory and all files in it
				removed_count += Path::RemoveAll_Internal(rmpath);
			}
			else if (ftype == S_IFREG || ftype == S_IFLNK || ftype == S_IFSOCK) {
				// remove file
				std::cout << "removing " << rmpath.GetNativePath() << '\n';
				if (unlinkat(dir_fd, entry->d_name, 0)) {
					throw PathException(strerror(errno));
				}
				removed_count++;
			}
		}
	}
	closedir(dir);
	std::cout << "removing directory " << npath << '\n';
	if (rmdir(npath.c_str())) {
		throw PathException(strerror(errno));
	}
	removed_count++;
#endif
	return removed_count;
}

void Path::CheckRemoveAll(const Path& path) {
	if (!path) {
		throw PathException("invalid path");
	}
	if (path.path.size() == 1 && path.path[0] == '/') {
		throw PathException("can not RemoveAll at root");
	}
#if defined(WIN32)
	WIN32_FIND_DATAW s;
	auto npath = path.GetNativePath();
	if (npath.back() != L'\\') {
		npath.push_back(L'\\');
	}
	npath.push_back(L'*');
	HANDLE search = FindFirstFileW(npath.c_str(), &s);
	if (search == INVALID_HANDLE_VALUE) {
		throw PathException("Search failed");
	}
	do {
		if (0 == lstrcmpW(s.cFileName, L".") || 0 == lstrcmpW(s.cFileName, L"..")) {
			continue; // don't check these (that would recurse!)
		}
		if (s.dwFileAttributes & (FILE_ATTRIBUTE_DEVICE | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY)) {
			throw PathException("Non-removable object");
		}
		DWORD open_flags = 0;
		if (s.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
			open_flags |= FILE_FLAG_OPEN_REPARSE_POINT;
		}
		if (s.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			open_flags |= FILE_FLAG_BACKUP_SEMANTICS;
		}
		auto checkpath = path / std::wstring(s.cFileName);
		auto ncheckpath = checkpath.GetNativePath();
		HANDLE file = CreateFileW(ncheckpath.c_str(), DELETE, 0, NULL, OPEN_EXISTING, open_flags, NULL);
		if (file == INVALID_HANDLE_VALUE) {
			throw PathException("Non-removable object: access check failed");
		}
		CloseHandle(file);
		if (s.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			Path::CheckRemoveAll(checkpath);
		}
	} while (FindNextFileW(search, &s));
	FindClose(search);
	return;
#else
	auto npath = path.GetNativePath();
	if (npath.back() != L'/') {
		npath.push_back(L'/');
	}
	DIR* dir = opendir(npath.c_str());
	if (!dir) {
		throw PathException("Search failed");
	}
	dirent* entry = readdir(dir);
	int dir_fd = dirfd(dir);
	struct stat s;
	while ((entry = readdir(dir))) {
		int err = fstatat(dir_fd, entry->d_name, &s, 0);
		if (0 == strcmp(entry->d_name, ".") || 0 == strcmp(entry->d_name, "..")) {
			continue; // don't check these (that would recurse!)
		}
		if (!err) {
			err = faccessat(dir_fd, entry->d_name, R_OK | W_OK, 0);
			if (err) {
				throw PathException("Non-removable object: access check failed");
			}
			mode_t ftype = (s.st_mode & S_IFMT);
			if (!(ftype == S_IFREG || ftype == S_IFDIR || ftype == S_IFLNK || ftype == S_IFSOCK)) {
				throw PathException("Not a removable object");
			}
			if (ftype == S_IFDIR) {
				auto checkpath = path / std::string(entry->d_name);
				Path::CheckRemoveAll(checkpath);
			}
		}
	}
	closedir(dir);
#endif
}

Path Path::GetUserSettingsPath() {
	// Try to use cached value
	if (Path::settings_folder) {
		return Path::settings_folder;
	}
	Path ret;
#if defined(__APPLE__)
	char* home = getenv("HOME");
	if (home == nullptr) {
		return Path::invalid_path;
	}
	ret = std::string(home);
	ret /= "Library";
	ret /= "Preferences";
	ret /= app_name;
	// $HOME/Library/Preferences/trillek/
#elif defined(__unix__)
	char* home = getenv("XDG_CONFIG_HOME");
	if (home == nullptr) {
		home = getenv("HOME");
		if (home == nullptr) {
			return Path::invalid_path;
		}
	}
	ret = std::string(home);
	ret /= ".config";
	ret /= app_name;
	// $HOME/.config/trillek/
#elif defined(WIN32)
	LPWSTR wszPath = nullptr;
	if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &wszPath))) {
		return Path::invalid_path;
	}
	std::wstring wpath((wchar_t*)wszPath);
	CoTaskMemFree(wszPath);
	ret = tec::utf8_encode(wpath);
	ret /= app_name;
	// %APPDATA%/trillek/
#endif
	// add the application name
	ret.path += PATH_SEPARATOR;
	Path::MkPath(ret);
	Path::settings_folder = ret;
	return ret;

} // End of GetUserSettingsPath

Path Path::GetUserDataPath() {
	// Try to use cached value
	if (Path::udata_folder) {
		return Path::udata_folder;
	}
	Path ret;
#if defined(__APPLE__)
	char* home = getenv("HOME");
	if (home == nullptr) {
		return Path::invalid_path;
	}
	// OSX $HOME/Library/Application\ Support/trillek/data/
	ret = std::string(home);
	ret /= "Library";
	ret /= "Application Support";
	ret /= app_name;
	ret /= "data";
#elif defined(__unix__)
	char* home = getenv("XDG_DATA_HOME");
	if (home == nullptr) {
		home = getenv("HOME");
		if (home == nullptr) {
			return Path::invalid_path;
		}
	}
	ret = Path(std::string(home));
	// $HOME/.local/share/trillek/
	ret /= ".local/share";
	ret /= app_name;
#elif defined(WIN32)
	ret = Path::GetUserSettingsPath();
	if (!ret) {
		return Path::invalid_path;
	}
	// Win %APPDATA%/trillek/data/
	ret /= "data";
#else
#error must implement Path::GetUserDataPath() for this OS
#endif
	ret.path += PATH_SEPARATOR;
	Path::udata_folder = ret;
	Path::MkPath(ret);
	return ret;
} // End of GetUserDataPath

Path Path::GetUserCachePath() {
	// Try to use cached value
	if (Path::cache_folder) {
		return Path::cache_folder;
	}
	Path ret;
#if defined(__APPLE__)
	char* home = getenv("HOME");
	if (home == nullptr) {
		return Path::invalid_path;
	}
	ret = std::string(home);
	ret /= "Library";
	ret /= "Application Support";
	ret /= app_name;
	ret /= "cache";
	// $HOME/Library/Application Support/trillek/cache/
#elif defined(__unix__)
	char* home = getenv("XDG_CACHE_HOME");
	if (home == nullptr) {
		home = getenv("HOME");
		if (home == nullptr) {
			return Path::invalid_path;
		}
	}
	ret = Path(std::string(home));
	ret /= ".cache";
	ret /= app_name;
	// $HOME/.cache/trillek/
#elif defined(WIN32)
	LPWSTR wszPath = nullptr;
	if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &wszPath))) {
		return Path::invalid_path;
	}
	ret = Path(std::wstring(wszPath));
	CoTaskMemFree(wszPath);
	ret /= app_name;
	// %LOCALAPPDATA%/trillek/
#endif
	ret.path += PATH_SEPARATOR;
	Path::cache_folder = ret;
	Path::MkPath(ret);
	return ret;
} // End of GetUserCachePath

Path Path::GetProgramPath() {
#if defined(__APPLE__)
	std::string tmp;
	tmp.resize(PATH_MAX);
	uint32_t size = tmp.size();
	// Mac OS X: _NSGetExecutablePath() see: man 3 dyld
	if (_NSGetExecutablePath(tmp.data(), &size) == 0) {
		tmp.resize(tmp.find_first_of('\0'));
		return Path(tmp);
	}
	else {
		// Too small buffer
		tmp.resize(size);
		_NSGetExecutablePath(tmp.data(), &size);
		return Path(tmp);
	}
#elif defined(__linux__)
	// yes this is __linux__ not __unix__ because it's kernel specific, but other unix-like are similar
	std::string tmp;
	tmp.resize(PATH_MAX);
	// Linux: readlink /proc/self/exe
	ssize_t bytes = readlink("/proc/self/exe", tmp.data(), tmp.size());
	if (bytes == -1) {
		return Path::invalid_path;
	}
	tmp.resize(bytes);
	return Path(tmp);
#elif defined(WIN32)
	std::wstring wstr;
	wstr.resize(1024);
	// GetModuleFileNameW(HMODULE hModule, LPWSTR lpFilename, DWORD nSize)
	// hModule -> module handle or NULL for current
	// lpFilename -> pointer to buffer to hold wide characters
	// nSize -> number of *characters* (not bytes) in buffer.
	// return -> length of copied string, not including nul char.
	// truncated names will return nSize with GetLastError() == ERROR_INSUFFICIENT_BUFFER
	DWORD set_size = static_cast<DWORD>(wstr.size());
	while ((wstr.size() == set_size) && (set_size <= 0x8000)) {
		wstr.resize(GetModuleFileNameW(nullptr, wstr.data(), set_size));
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
			set_size += 1024;
			wstr.resize(set_size);
		}
	}
	return Path(wstr);
#else
//#elif defined(__some_fancy_new_os__)
#error must implement Path::GetProgramPath() for this OS
	// Other Unix-like OS should provide this via an API or changes in procfs
	//  * Solaris: getexecname()
	//  * FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
	//  * FreeBSD with procfs: readlink /proc/curproc/file
	//    (FreeBSD doesn't typically have or mount procfs by default)
	//  * NetBSD: readlink /proc/curproc/exe
	//  * DragonFly BSD: readlink /proc/curproc/file
#endif
}

// Try to get current working directory (i.e. where the program was called from)
Path Path::GetWorkingDir() {
	Path::NativePath cwd;
	cwd.resize(FILENAME_MAX);
	constexpr Path::NativePath::value_type nullchar = 0;
#if defined(WIN32)
	if (!GetCurrentDirectoryW((DWORD)cwd.size(), cwd.data())) {
#else
	if (!getcwd(cwd.data(), cwd.size())) {
#endif
		// The working directory is kind of important
		return Path::invalid_path;
	}
	cwd.resize(std::min(cwd.find_first_of(nullchar), cwd.size()));
	// Search for the assets folder
	return Path(cwd);
}

} // namespace tec
