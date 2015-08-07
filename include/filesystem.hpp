#pragma once
/**
 * OS Filesystem abstraction stuff
 */

#include "os.hpp"

#include <string>

namespace tec {
namespace fs {

#if defined(__unix__)
	const char PATH_SEPARATOR = "/"; /// OS Filesystem path separator
#else
	const char PATH_SEPARATOR = "\\"; /// OS Filesystem path separator
#endif

	/**
	* \brief Returns the path to the User settings folder
	*
	* Usually this paths are :
	* *nix : /home/USER/.config/APPNAME/
	* OSX  : /Users/USER/Library/Application Support/APPNAME/
	* WIN  : C:\Users\USER\AppData\Roaming\APPNAME\
	*
	* \return string with the full path. Empty string if fails
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
	* \return string with the full path. Empty string if fails
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
	* \return string with the full path. Empty string if fails
	*/
	std::string GetUserCachePath();

	/**
	 * \brief Check if a directory exists
	 *
	 * \return True if the directory exists
	*/
	bool DirExists(const std::string& path);

	/**
	 * \brief Check if a file exists
	 *
	 * \return True if the directory exists
	*/
	bool FileExists(const std::string& path);

	/**
	 * \brief Try to create a directory route
	 * TODO change int by a enum
	 * \return 0 if success. If fails, return a error code
	 */
	int MkDir(const std::string& path);

	/**
	 * \brief Try to obtain the full path to the program binary file
	 *
	 * \return string with the full path. Empty string if fails
	 */
	std::string GetProgramPath();

}
}

