#pragma once
/**
 * OS Filesystem abstraction stuff
 */

#include "os.hpp"

#include <string>

namespace tec {
namespace fs {

#if defined(__unix__)
	const char PATH_SEPARATOR = '/'; /// OS Filesystem path separator
#else
	const char PATH_SEPARATOR = '\\'; /// OS Filesystem path separator
#endif

	/**
	* \brief Returns the path to the User settings folder
	*
	* Usually this paths are :
	* *nix : /home/USER/.config/APPNAME/
	* OSX  : /Users/USER/Library/Application Support/APPNAME/
	* WIN  : C:\Users\USER\AppData\Roaming\APPNAME\
	*
	* \return string with the path. Empty string if fails
	*/
	std::string GetUserSettingsPath();

	/**
	* \brief Returns the path to the User persitent data folder (for save files, for example)
	*
	* Usually this paths are :
	* *nix : /home/USER/.local/share/APPNAME/
	* OSX  : /Users/USER/Library/Application Support/APPNAME/data/
	* WIN  : C:\Users\USER\AppData\Roaming\APPNAME\data\
	*
	* \return string with the path. Empty string if fails
	*/
	std::string GetUserDataPath();

	/**
	* \brief Returns the path to the User cache folder
	*
	* Usually this paths are :
	* *nix : /home/USER/.cache/APPNAME/
	* OSX  : /Users/USER/Library/Application Support/APPNAME/cache/
	* WIN  : C:\Users\USER\AppData\Local\APPNAME\
	*
	* \return string with the path. Empty string if fails
	*/
	std::string GetUserCachePath();

}
}

