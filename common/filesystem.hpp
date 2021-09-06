#pragma once
/**
 * OS File system abstraction stuff
 */

#include "string.hpp"

#include <algorithm>
#include <fstream>
#include <memory>
#include <ostream>
#include <stdio.h>
#include <string>
#include <string_view>

// allow unique_ptr to manage FILE lifetime
template <> struct std::default_delete<FILE> {
	void operator()(FILE* file) { fclose(file); }
};

namespace tec {
#if defined(WIN32)
constexpr std::string_view PATH_NATIVE_SEPARATOR = "\\"; /// OS File system path separator
#else
constexpr std::string_view PATH_NATIVE_SEPARATOR = "/"; /// OS File system path separator
#endif
constexpr std::string_view PATH_SEPARATOR = "/"; /// tec Path separator

const std::string_view app_name("trillek");

class PathException : public std::exception {
public:
	const char* message;
};

enum PATH_OPEN_FLAGS {
	FS_DEFAULT = 0,
	FS_READONLY = 0,
	FS_READWRITE = 1,
	FS_CREATE = 2, // files are normally not created
	FS_APPEND = 4, // append to the end of the file
};

class Path final {
public:
#if defined(WIN32)
	constexpr static char PATH_NATIVE_CHAR = '\\'; /// OS File system path separator
	typedef std::wstring NFilePath; /// Native string format for paths
#else
	constexpr static char PATH_NATIVE_CHAR = '/'; /// OS File system path separator
	typedef std::string NFilePath; /// Native string format for paths
#endif
	constexpr static char PATH_CHAR = '/'; /// tec Path separator

	static const std::size_t npos = std::string::npos;

	/**
	* \brief Builds a empty path
	*/
	explicit Path();

	/**
	* \brief Builds a path from a string_view or substring there of
	*
	* \param other A string_view with a path
	* \param pos Begin of the range to get a slice (default = 0)
	* \param count How many bytes to grab from other (default = size of other)
	*/
	explicit Path(const std::string_view& other);

	/**
	* \brief Builds a path from a string or substring
	*
	* \param other A string with a path
	* \param pos Begin of the range to get a slice (default = 0)
	* \param count How many bytes to grab from other (default = size of other)
	*/
	explicit Path(const std::string& other, std::size_t pos = 0, std::size_t count = std::string::npos);

	explicit Path(std::string&& other);

	template <size_t N> Path(const char (&other)[N]) : Path(std::string_view(other)) {}

	/**
	* \brief Builds a path from a wstring
	*
	* \param other A wstring containing a path
	*/
	explicit Path(const std::wstring& other);

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
	static Path GetUserSettingsPath();

	/**
	* \brief Returns the path to the User persistent data folder (for save files, for example)
	*
	* Usually this paths are :
	* *nix : /home/USER/.local/share/APPNAME/
	* OSX  : /Users/USER/Library/Application Support/APPNAME/data/
	* WIN  : C:\Users\USER\AppData\Roaming\APPNAME\data\
	*
	* \return string with the full path. Empty string if fails
	*/
	static Path GetUserDataPath();

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
	static Path GetUserCachePath();

	/**
	* \brief Check if a directory exists
	*
	* \return True if the directory exists
	*/
	bool DirExists() const;

	/**
	* \brief Check if a file exists
	*
	* \return True if the directory exists
	*/
	bool FileExists() const;

	/**
	* \brief Try to create a directory route
	*
	* \return True if success or the dir exists
	*/
	static bool MkDir(const Path& path);

	/**
	* \brief Try to create a full path
	*
	* \param path Absolute path
	* \return True if success.
	*/
	static bool MkPath(const Path& path);

	/**
	* \brief Extract a filename from a path
	*
	* \return Empty string if is an invalid path for a file
	*/
	std::string FileName() const;

	/**
	* \brief Extract a filename without the extension from a path to a file
	*
	* \return Empty string if is an invalid path for a file
	*/
	std::string FileStem() const;

	/**
	* \brief Extract a extension from a path to a file
	*
	* \return Empty string if isn't a file or not have extension
	*/
	std::string FileExtension() const;

	/**
	* \brief Return base path of a full path
	*
	* If is a path of a file, returns the path to the dir that contains the file
	* If is a path of a directory, returns the path to the dir that contains the directory (like cd .. && pwd)
	* \return Empty string if is an invalid path. Returned path would have the final slash
	*/
	Path BasePath() const;

	/**
	* \brief Is an absolute or relative path ?
	*/
	bool isAbsolutePath() const;

	/**
	* \brief Return a subpath
	*
	* \param begin First element (each element is separated by a path separator)
	* \param end Last element (default Path::npos)
	*
	* \code
	* Path f("assets/foo/bar/mesh.obj");
	* f.Subpath(2, Path::npos); // ==> "bar/mesh.obj"
	* f.Subpath(0, 2); // ==> "assets/foo"
	* \endcode
	*/
	Path Subpath(std::size_t begin, std::size_t end = Path::npos) const;

	/**
	* \brief Return a subpath
	*
	* \param needle path element to search
	* \param include Includes the needle element on the output ?
	*
	* \code
	* Path f("./assets/foo/bar/mesh.obj");
	* f.SubpathFrom("assets");       // ==> "foo/bar/mesh.obj"
	* f.SubpathFrom("assets", true); // ==> "assets/foo/bar/mesh.obj"
	* \endcode
	*/
	Path SubpathFrom(const std::string& needle, bool include = false) const;

	/**
	* \brief Try to obtain the full path to the program binary file
	*
	* \return string with the full path. Empty string if fails
	*/
	static Path GetProgramPath();

	/**
	* \brief Normalize path to the OS format
	*
	* - Convert slashes to the correct OS slash
	* - Remove drive unit if is a *NIX OS
	* \return normalized path
	*/
	void NormalizePath();

	/**
	* \brief Check if the path is a valid absolute o relative path
	*/
	bool isValidPath() const;

	/**
	* \brief Open this path as a C-style stream for C functions
	* \param open_mode what access is required, default is readonly access to existing files
	*/
	std::unique_ptr<FILE> OpenFile(PATH_OPEN_FLAGS open_mode = FS_DEFAULT) const;

	/**
	* \brief Open this path as an iostream
	* \param open_mode what access is required, default is readonly access to existing files
	*/
	std::unique_ptr<std::fstream> OpenStream(PATH_OPEN_FLAGS open_mode = FS_DEFAULT) const;

	/**
	* \brief Returns a path on the native OS encoding
	*
	* - Normalize path
	* - return wstring using \ on Windows
	* - return string using / on *NIX
	* \return native string of the path
	*/
	Path::NFilePath GetNativePath() const;

	/**
	* \brief Attempts locate the assets directory
	* This function will try to search for the first valid directory path, probing with this paths:
	* - ./assets/
	* - EXE_PATH/assets/
	* - EXE_PATH/../assets/
	* - EXE_PATH/../share/assets/
	* Were EXE_PATH is the value as returned by GetProgramPath()
	* The first valid path is saved, and can then be read with GetAssetsBasePath()
	*/
	static void LocateAssets();

	/**
	* \brief Return the base directory where to search for assets
	*
	* If isn't set, then it will first call LocateAssets()
	*/
	static Path GetAssetsBasePath();

	/**
	* \brief returns the full path to an asset
	*
	* \param asset Relative path to asset base folder that identify a asset file (for example "shaders/foo.vert")
	*/
	static Path GetAssetPath(const Path& asset);

	/**
	* \brief returns the full path to an asset
	*
	* \param asset String path-like that identify a asset file (for example "shaders/foo.vert")
	*/
	static Path GetAssetPath(const std::string& asset);

	/**
	* \brief returns the full path to an asset
	*
	* \param asset String path-like that identify a asset file (for example "shaders/foo.vert")
	*/
	static Path GetAssetPath(const char* asset);

	/**
	* \brief Sets the base directory where search the assets
	*/
	static void SetAssetsBasePath(Path);

	/**
	* \brief Returns the string representation of a path
	*/
	std::string toString() const { return device + path; }

	/**
	* \brief Return if this Path is empty
	*/
	bool empty() const { return device.empty() && path.empty(); }

	Path& operator=(const Path& rhs) {
		this->device = rhs.device;
		this->path = rhs.path; //Not necessary to normalize
		return *this;
	}

	Path& operator=(const std::string& str) {
		path = str;
		SetDevice();
		NormalizePath();
		return *this;
	}

	Path& operator=(const std::string&& str) {
		path = std::move(str);
		SetDevice();
		NormalizePath();
		return *this;
	}

	Path& operator=(const std::wstring& wstr) {
		path = utf8_encode(wstr);
		SetDevice();
		NormalizePath();
		return *this;
	}

	Path& operator=(const std::string_view& strv) {
		path.assign(strv);
		SetDevice();
		NormalizePath();
		return *this;
	}

	Path& operator=(const char* cstr) {
		path.assign(cstr);
		SetDevice();
		NormalizePath();
		return *this;
	}

	/**
	* \brief Append a subdirectory or file
	*/
	Path& operator/=(const Path& rhs) {
		if (path.empty()) {
			this->device = rhs.device;
			if (rhs.empty() || rhs.path.front() != PATH_CHAR) {
				this->path.append(PATH_SEPARATOR).append(rhs.path);
			}
			else {
				this->path += rhs.path;
			}
		}
		else {
			if (!rhs.device.empty()) {
				throw PathException();
			}
			if (path.back() != PATH_CHAR && (rhs.empty() || rhs.path.front() != PATH_CHAR)) {
				this->path.append(PATH_SEPARATOR).append(rhs.path);
			}
			else {
				this->path += rhs.path;
			}
		}
		this->NormalizePath();
		return *this;
	}

	/**
	* \brief Append a subdirectory or file
	*/
	Path& operator/=(const std::string_view rhs) {
		this->operator/=(Path(rhs));
		return *this;
	}

	/**
	* \brief Append a subdirectory or file
	*/
	Path& operator/=(const char* rhs) {
		this->operator/=(Path(std::string(rhs)));
		return *this;
	}

	/**
	* \brief Append a subdirectory or file
	*/
	Path& operator/=(const std::string& rhs) {
		this->operator/=(Path(rhs));
		return *this;
	}

	/**
	* \brief Append a subdirectory or file
	*/
	Path& operator/=(const std::wstring& rhs) {
		this->operator/=(Path(rhs));
		return *this;
	}

private:
	std::string device;
	std::string path; /// Stores path as an UTF8 string
	void SetDevice();
	Path(const std::string& _device, const std::string& _path) : device{_device}, path{_path} {}

	// Cached paths
	static std::string settings_folder;
	static std::string udata_folder;
	static std::string cache_folder;

	static std::string assets_base;

}; // End of FileSystem

/**
* \brief Append a subdirectory or file
*/
inline Path operator/(const Path& lhs, const Path& rhs) { return Path(lhs) /= rhs; }

/**
* \brief Append a subdirectory or file
*/
template <size_t N> inline Path operator/(const Path& lhs, const char (&cstr)[N]) {
	return Path(lhs) /= Path(std::string(cstr, 0, N));
}

/**
* \brief Append a subdirectory or file
*/
inline Path operator/(const Path& lhs, const std::string_view strv) { return Path(lhs) /= Path(strv); }

/**
* \brief Append a subdirectory or file
*/
inline Path operator/(const Path& lhs, std::string& str) { return Path(lhs) /= Path(str); }

/**
* \brief Append a subdirectory or file
*/
inline Path operator/(const Path& lhs, const std::wstring& wstr) { return Path(lhs) /= Path(wstr); }

/**
* \brief Output to a stream
*/
template <typename charT, typename traits>
std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const Path& path) {
	auto tmp = path.toString();
	return os << tmp.c_str();
}

template <> inline std::ostream& operator<<(std::ostream& os, const Path& path) { return os << path.toString(); }

/**
* \brief Input from a stream
*/
template <typename charT, typename traits>
std::basic_istream<charT, traits>& operator>>(std::basic_istream<charT, traits>& is, Path& path) {
	std::string tmp;
	is >> tmp;
	path = Path(tmp);
}
} // namespace tec
