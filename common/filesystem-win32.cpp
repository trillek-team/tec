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

#include <Windows.h>
#include <direct.h>
#include <Shlobj.h>
#include <comutil.h>

#pragma comment(lib, "comsuppw")

#ifndef PATH_MAX
#define PATH_MAX 1024 // same with Mac OS X's syslimits.h
#endif

namespace tec {
	const std::string FilePath::PATH_SEPARATOR = std::string("/");

	void FilePath::CacheUserSettingsPath() {
		FilePath ret;
		LPWSTR wszPath = NULL;

		if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &wszPath))) {
			throw std::runtime_error("cannot get %APPDATA% path");
		}

		_bstr_t bstrPath(wszPath);
		std::wstring wpath((wchar_t*)bstrPath);
		ret = tec::utf8_encode(wpath);
		CoTaskMemFree(wszPath);
		ret /= app_name;
		ret += PATH_SEPARATOR;

		FilePath::MkPath(ret);
		FilePath::settings_folder = ret.toString();
	}

	void FilePath::CacheUserDataPath() {
		auto path = FilePath::GetUserSettingsPath();
		path /= "data";
		path += PATH_SEPARATOR;

		FilePath::MkPath(path);
		FilePath::udata_folder = path.toString();
	}

	void FilePath::CacheUserCachePath() {
		LPWSTR wszPath = NULL;

		if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &wszPath))) {
			throw std::runtime_error("cannot get %LOCALAPPDATA% path");
		}

		_bstr_t bstrPath(wszPath);
		std::wstring wpath((wchar_t*)bstrPath);
		FilePath path(tec::utf8_encode(wpath));
		CoTaskMemFree(wszPath);
		path /= app_name;
		path += PATH_SEPARATOR;

		FilePath::MkPath(path);
		FilePath::cache_folder = path.toString();
	}


	bool FilePath::DirExists() const {
		struct _stat s;
		auto wtmp = this->GetNativePath();

		// Trailing slashes break _wstat
		wchar_t *p = &wtmp[wcslen(&wtmp[0]) - 1];
		if (*p == L'/' || *p == L'\\') {
			*p = 0;
		}
		int err = _wstat((wchar_t*)wtmp.c_str(), &s);
		if (-1 == err) {
			// does not exist or error
			// if  errno == ENOENT --> not exist
			return false;
		}
		else {
			return true;
		}
	}

	bool FilePath::MkDir(const FilePath& path) {
		if (!path.isValidPath()) {
			return false;
		}
		int ret = _wmkdir(path.GetNativePath().c_str());
		if (ret == 0 || errno == EEXIST) {
			return true;
		}
		return false;
	}

	bool FilePath::MkPath(const FilePath& path) {
		if (path.path.size() <= 3 && std::isalpha(path.path.at(0))) { // 'X:\'
			return true;
		}

		auto base = path.BasePath();
		//Recursively create the path
		if (!base.empty() && !base.DirExists()) {
			MkPath(base);
		}
		//When the path is done
		return MkDir(path);
	}

	FilePath FilePath::BasePath() const {
		size_t len = path.size();
		if (len <= 3 && std::isalpha(path.at(0))) { // 'X:\'
			return FilePath(path);
		}

		size_t pos = path.find_last_of(PATH_SEPARATOR_C);
		if (pos == len - 1) {
			// Path ended with a path separator
			pos = path.find_last_of(PATH_SEPARATOR_C, path.size() - 2);
		}
		if (pos == std::string::npos) {
			return FilePath();
		}

		return FilePath(path, 0, pos + 1);
	}

	bool FilePath::isAbsolutePath() const {
		if (path.empty()) {
			return false;
		}
		if (path.size() > 1) {
			return std::isalpha(path.at(0)) && path.at(1) == ':';
		}
		return false;
	}

	FilePath FilePath::Subpath(size_t begin, size_t end) const {
		FilePath ret;
		std::istringstream f(this->path);
		std::string s;
		size_t count = 0;

		auto absoulte = this->isAbsolutePath();
		while (count < end && std::getline(f, s, PATH_SEPARATOR_C)) {
			if (count >= begin && count < end) {
				if (count == 0 && absoulte) {
					ret = s;
				}
				else {
					ret /= s;
				}
			}
			count++;
		}

		return ret;
	}

	FilePath FilePath::GetProgramPath() {
		wchar_t buffer[MAX_PATH];
		if (0 == GetModuleFileNameW(NULL, buffer, MAX_PATH)) {
			return FilePath();
		}
		std::wstring wstr(buffer);
		return FilePath(wstr);
	}

	void FilePath::OsNormalizePath() {
		std::replace(path.begin(), path.end(), UNIX_PATH_SEPARATOR, WIN_PATH_SEPARATOR);
	}

	FilePath::NFilePath FilePath::GetNativePath() const {
		return tec::utf8_decode(path);
	}
}
