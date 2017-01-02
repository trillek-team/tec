// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "filesystem.hpp"

#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 1024 // same with Mac OS X's syslimits.h
#endif

namespace tec {
    void FilePath::CacheUserSettingsPath() {
        char *home = getenv("XDG_CONFIG_HOME");
        if (home == nullptr) {
            home = getenv("HOME");
            if (home == nullptr) {
                throw std::runtime_error("cannot get $XDG_CONFIG_HOME or $HOME");
            }
        }

        FilePath ret;
        ret = home;
        ret /= ".config";
        ret /= app_name;
        ret += PATH_SEPARATOR;

        FilePath::MkPath(ret);
        FilePath::settings_folder = ret.toString();
    }

    void FilePath::CacheUserDataPath() {
		char *home = getenv("XDG_DATA_HOME");
		if (home == nullptr) {
			home = getenv("HOME");
			if (home == nullptr) {
				throw std::runtime_error("cannot get $XDG_DATA_HOME or $HOME");
			}
		}

		FilePath ret;
        ret = home;
		ret /= ".local";
		ret /= "share";
		ret /= app_name;
		ret += PATH_SEPARATOR;

		FilePath::udata_folder = ret.toString();
		FilePath::MkPath(ret);
	}

    void FilePath::CacheUserCachePath() {
    	char* home = getenv("XDG_CACHE_HOME");
    	if (home == nullptr) {
    		home = getenv("HOME");
    		if (home == nullptr) {
    			throw std::runtime_error("cannot get $XDG_CACHE_HOME or $HOME");
    		}
    	}
    	FilePath path(home);
    	path /= ".cache";
    	path /= app_name;
    	path += PATH_SEPARATOR;

		FilePath::MkPath(path);
		FilePath::cache_folder = path.toString();
	}

    FilePath FilePath::GetProgramPath() {
    	char tmp[PATH_MAX];
    	int bytes = readlink("/proc/self/exe", tmp, PATH_MAX);
    	bytes = bytes < PATH_MAX -1 ? bytes : PATH_MAX -1;
    	if (bytes >= 0) {
    		tmp[bytes] = '\0';
    	}

    	return FilePath(tmp);
    }
}
