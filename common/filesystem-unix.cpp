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

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <CoreServices/CoreServices.h>
#endif // __APPLE

#ifndef PATH_MAX
#define PATH_MAX 1024 // same with Mac OS X's syslimits.h
#endif

namespace tec {
    bool FilePath::DirExists() const {
    	struct stat s;
    	int err = stat(path.c_str(), &s);
    	if (-1 == err) {
    		// does not exist or error
    		// if  errno == ENOENT --> not exist
    		return false;
    	} else {
    		return true;
    	}
    }

    bool FilePath::MkDir(const FilePath& path) {
    	if (!path.isValidPath()) {
    		return false;
    	}
    	int ret = mkdir(path.GetNativePath().c_str(), 0755);
    	if (ret == 0 || errno == EEXIST) {
    		return true;
    	}
    	return false;
    }

    bool FilePath::MkPath(const FilePath& path) {
    	if (path.path.size() == 1 && path.path.at(0) == PATH_SEPARATOR_C) { // '\'
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
    	if (len == 1 && path.at(0) == PATH_SEPARATOR_C) { // '\'
    		return FilePath(path);
    	}

    	size_t pos = path.find_last_of( PATH_SEPARATOR_C);
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

    	return path.at(0) == PATH_SEPARATOR_C;
    }

    FilePath FilePath::Subpath(size_t begin, size_t end) const {
    	FilePath ret;
    	std::istringstream f(this->path);
    	std::string s;
    	size_t count = 0;

    	while (count < end && std::getline(f, s, PATH_SEPARATOR_C)) {
    		if (count >= begin && count < end) {
    			ret /= s;
    		}
    		count++;
    	}
    	return ret;
    }

	void FilePath::OsNormalizePath() {
		std::replace(path.begin(), path.end(), WIN_PATH_SEPARATOR, UNIX_PATH_SEPARATOR);
        // Remove "C:", etc. This comparison it's safe on UTF-8 as isalpha would
        // check [a-zA-Z] and that's the format of Windows file units.
		if (path.size() > 2 && std::isalpha(path.at(0)) && path.at(1) == ':') {
			path.erase(0, 2);
		}
	}

    FilePath::NFilePath FilePath::GetNativePath() const {
        return path;
    }
}
