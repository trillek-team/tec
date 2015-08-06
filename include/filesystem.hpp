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
	* \brief Returns the path to the User settings
	*
	* \return string with the path. Empty string if fails
	*/
	std::string GetUserSettingsPath();

}
}

