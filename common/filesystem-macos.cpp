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

#include <unistd.h>
#include <limits.h>

#include <mach-o/dyld.h>
#include <CoreServices/CoreServices.h>

#ifndef PATH_MAX
#define PATH_MAX 1024 // same with Mac OS X's syslimits.h
#endif

namespace tec {
    void FilePath::CacheUserSettingsPath() {
    	FilePath ret;

    	char *home = getenv("HOME");
    	ret = home;
    	ret /= "Library";
    	ret /= "Preferences";
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
    	auto path = GetUserSettingsPath();
    	if (path.empty()) {
    		return FilePath();
    	}
    	path /= "cache";
    	path += PATH_SEPARATOR ;

		FilePath::MkPath(path);
		FilePath::cache_folder = path.toString();
	}
    
    FilePath FilePath::GetProgramPath() {
    	char tmp[PATH_MAX];
    	uint32_t size = PATH_MAX;
    	if (_NSGetExecutablePath(tmp, &size) == 0) {
    		return FilePath(tmp);
    	} else {
    		// Too small buffer
    		char* tmp2 = new char[size]();
    		_NSGetExecutablePath(tmp2, &size);
    		return FilePath(tmp2);
    	}
    }
}
