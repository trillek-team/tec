#pragma once
/**
 * OS Filesystem abstraction stuff
 */

#include "os.hpp"

#include <string>

namespace tec {
namespace fs {

#if defined(__unix__)
	const std::string PATH_SEPARATOR("/");  /// OS Filesystem path separator
	const char PATH_SEPARATOR_C = '/';      /// OS Filesystem path separator
#else
	const std::string PATH_SEPARATOR("\\"); /// OS Filesystem path separator
	const char PATH_SEPARATOR_C = '\\';     /// OS Filesystem path separator
#endif

#if defined(WIN32)
	typedef std::wstring NFilePath; /// Native string format for paths
#else
	typedef std::string NFilePath;  /// Native string format for paths
#endif

	typedef std::string FilePath; // If someday we decide to change this for work with paths

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
	FilePath GetUserSettingsPath();

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
	FilePath GetUserDataPath();

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
	FilePath GetUserCachePath();

	/**
	 * \brief Check if a directory exists
	 *
	 * \return True if the directory exists
	*/
	bool DirExists(const FilePath& path);

	/**
	 * \brief Check if a file exists
	 *
	 * \return True if the directory exists
	*/
	bool FileExists(const FilePath& path);

	/**
	 * \brief Try to create a directory route
	 *
	 * \return True if success or the dir exists
	 */
	bool MkDir(const FilePath& path);

	/**
	 * \brief Try to create a full path
	 *
	 * \param path Absolute path
	 * \return True if success.
	 */
	bool MkPath(const FilePath& path);

	/**
	 * \brief Extract a filename from a path
	 *
	 * \return Empty string if is an invalid path for a file
	 */
	FilePath FileName(const FilePath& path);

	/**
	 * \brief Return base path of a full path
	 *
	 * If is a path of a file, returns the path to the dir that contains the file
	 * If is a path of a directory, returns the path to the dir that contains the direcotory (like cd .. && pwd)
	 * \retirm Empty string if is an invalid path. Returned path would have the final slash
	 */
	FilePath BasePath(const FilePath& path);

	/**
	 * \brief Is an absolute or relative path ?
	 */
	bool isAbsolutePath(const FilePath& path);

	/**
	 * \brief Try to obtain the full path to the program binary file
	 *
	 * \return string with the full path. Empty string if fails
	 */
	FilePath GetProgramPath();

	/**
	 * \brief Normalize path to the OS format
	 *
	 * - Convert slashs to the correct OS slash
	 * - Remove drive unit if is a *NIX OS
	 * \return normalized path
	*/
	void NormalizePath(FilePath& path);

	/**
	 * \brief Returns a path on the native OS encoding
	 *
	 * - Normalize path
	 * - return wstring on Windows
	 * - return string on *NIX
	 * \return native string of the path
	*/
	NFilePath GetNativePath(const FilePath& path);

}
}
