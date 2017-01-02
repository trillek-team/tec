// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "filesystem.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>

// Stuff to get OS libs for paths
#if defined(__unix__)
#include <unistd.h>
#include <limits.h>

#if defined(__APPLE__)
#include <mach-o/dyld.h>
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

const std::string app_name("trillek"); // TODO Ask to tec::OS for the app name ?
const char UNIX_PATH_SEPARATOR = '/';    /// *NIX file system path separator
const char WIN_PATH_SEPARATOR = '\\';    /// Windows file system path separator
#if defined(__unix__)
	const std::string FilePath::PATH_SEPARATOR = std::string("\\");
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
	if (FilePath::settings_folder.empty()) {
		FilePath::CacheUserSettingsPath();
	}
	return FilePath(FilePath::settings_folder);
}

FilePath FilePath::GetUserDataPath() {
	// Try to use cached value
	if (FilePath::udata_folder.empty()) {
		FilePath::CacheUserDataPath();
	}

	return FilePath(FilePath::udata_folder);
}

FilePath FilePath::GetUserCachePath() {
	// Try to use cached value
	if (FilePath::cache_folder.empty()) {
		FilePath::CacheUserCachePath();
	}

	return FilePath(FilePath::cache_folder);
}

bool FilePath::FileExists() const {
	return std::ifstream(this->GetNativePath()).good();
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

FilePath FilePath::SubpathFrom(const std::string& needle, bool include) const {
	FilePath ret;
	std::istringstream f(this->path);
	std::string s;
	bool found = false; bool first = true;
	while (std::getline(f, s, PATH_SEPARATOR_C)) {
		if (found) {
			ret = first ? s : ret / s;
			first = false;
		} else if (s.compare(needle) == 0) {
			found = true;
			if (include) {
				ret = s;
				first = false;
			}
		}
	}
	return ret;
}

// GetProgramPath:
// Other *NIX have his proper API or changes on procfs
//  * Mac OS X: _NSGetExecutablePath() (man 3 dyld)
//  * Linux: readlink /proc/self/exe
//  * Solaris: getexecname()
//  * FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
//  * FreeBSD if it has procfs: readlink /proc/curproc/file (FreeBSD doesn't have procfs by default)
//  * NetBSD: readlink /proc/curproc/exe
//  * DragonFly BSD: readlink /proc/curproc/file

void FilePath::NormalizePath() {
	FilePath::OsNormalizePath();

	// Prune duplicate path separators (like "\\\\")
	path.erase(std::unique(path.begin(), path.end(), [] (const char& a, const char& b) {
		return a == PATH_SEPARATOR_C && b == PATH_SEPARATOR_C;
	}), path.end());

	// TODO: handle relative paths like "\foo\bar\..\shaders\debug.vert"
	// (that isn't a relative path though...)
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

std::string FilePath::toGenericString() const {
	std::string ret(path);
	std::replace(ret.begin(), ret.end(), WIN_PATH_SEPARATOR, UNIX_PATH_SEPARATOR);
	return ret;
}

FilePath FilePath::GetAssetsBasePath() {
	if (FilePath::assets_base.empty()) {
		// Try to get current working directory (IE where the program was called)
		char cwd[FILENAME_MAX] = {0};
		#if defined(WIN32)
			if (! _getcwd(cwd, sizeof(cwd))) {
		#else
			if (! getcwd(cwd, sizeof(cwd))) {
		#endif

			#if defined(WIN32)
			  #pragma warning(push)
			  #pragma warning(disable: 4996)
			#endif
			 // Fall back to relative path if getcwd fails
			std::strncpy(cwd, "./", 2);
			cwd[2] = '\0';
			#if defined(WIN32)
			  #pragma warning(pop)
			#endif
		}

		// Search for the assets folder
		FilePath tmp(cwd);
		tmp /= "assets/";
		if (tmp.DirExists()) {
			FilePath::assets_base = tmp.toString();
		} else {
			tmp = FilePath::GetProgramPath().BasePath() / "assets/";
			if (tmp.DirExists()) {
				FilePath::assets_base = tmp.toString();
			} else {
				tmp = tmp.BasePath().BasePath() / "share/assets/";
				if (tmp.DirExists()) {
					FilePath::assets_base = tmp.toString();
				} else {
					tmp = FilePath::GetProgramPath().BasePath().BasePath() / ("share/" + app_name + "/assets/");
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
