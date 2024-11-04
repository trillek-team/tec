#pragma once
/**
 * OS File system abstraction stuff
 */

#include "string.hpp"

#include <algorithm>
#include <fmt/core.h>
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
protected:
	std::string message;

public:
	PathException() : message{"PathException"} {}
	PathException(const char* msg) : message{"PathException: "} { message.append(msg); }
	PathException(const std::string& msg) : message{"PathException: "} { message.append(msg); }
	virtual const char* what() const noexcept override { return message.c_str(); }
};

enum PATH_OPEN_FLAGS : int {
	FS_DEFAULT = 0,
	FS_READONLY = 0,
	FS_READWRITE = 1,
	FS_CREATE = 2, // files are normally not created
	FS_APPEND = 4, // append to the end of the file
};
inline PATH_OPEN_FLAGS operator|(PATH_OPEN_FLAGS a, PATH_OPEN_FLAGS b) { return (PATH_OPEN_FLAGS)((int)a | (int)b); }
inline PATH_OPEN_FLAGS operator+(PATH_OPEN_FLAGS a, PATH_OPEN_FLAGS b) { return (PATH_OPEN_FLAGS)((int)a | (int)b); }

class Path final {
public:
#if defined(WIN32)
	constexpr static char PATH_NATIVE_CHAR = '\\'; /// OS File system path separator
	typedef std::wstring NativePath; /// Native string format for paths
#else
	constexpr static char PATH_NATIVE_CHAR = '/'; /// OS File system path separator
	typedef std::string NativePath; /// Native string format for paths
#endif
	constexpr static char PATH_CHAR = '/'; /// The tec Path separator

	/**
	* \brief a value indicating "far as possible"
	*/
	static const std::size_t npos = std::string::npos;

	static const Path assets; /// The `assets:/` pseudo root
	static const Path scripts; /// `assets:/scripts/` where to find scripts
	static const Path shaders; /// `assets:/shaders/` where to find shaders

	/**
	* \brief Builds an empty Path
	*/
	explicit Path();

	/**
	* \brief Builds a Path from a string_view or substring there of
	*
	* \param other A string_view with a path
	*/
	explicit Path(const std::string_view& other);

	/**
	* \brief Builds a Path from a string or substring
	*
	* \param other A string with a path
	* \param pos Begin of the range to get a slice (default = 0)
	* \param count How many bytes to grab from other (default = size of other)
	*/
	explicit Path(const std::string& other, std::size_t pos = 0, std::size_t count = std::string::npos);

	explicit Path(std::string&& other);

	/**
	* \brief This template builds a Path from a string literal
	*/
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
	* Usually these paths are:
	* - *nix : `/home/$USER/.config/APPNAME/`
	* - OSX  : `/Users/$USER/Library/Application Support/APPNAME/`
	* - WIN  : `%APPDATA%\APPNAME\` i.e. `C:\Users\%USERNAME%\AppData\Roaming\trillek\`
	*
	* on *nix platforms, XDG_CONFIG_HOME will be respected if set.
	* \return The full Path. Empty/invalid if it fails
	*/
	static Path GetUserSettingsPath();

	/**
	* \brief Returns the path to the User persistent data folder (for save files, for example)
	*
	* Usually these paths are:
	* - *nix: `/home/$USER/.local/share/APPNAME/`
	* - OS X: `/Users/$USER/Library/Application Support/APPNAME/data/`
	* - WIN : `%APPDATA%\APPNAME\` i.e. `C:\Users\%USERNAME%\AppData\Roaming\trillek\data\`
	*
	* on *nix platforms, XDG_DATA_HOME will be respected if set.
	* \return The full Path. Empty/invalid if it fails
	*/
	static Path GetUserDataPath();

	/**
	* \brief Returns the path to the User cache folder
	*
	* Usually these paths are:
	* - *nix: `/home/$USER/.cache/APPNAME/`
	* - OS X: `/Users/$USER/Library/Application Support/APPNAME/cache/`
	* - WIN : `%LOCALAPPDATA%\APPNAME\` i.e. `C:\Users\%USERNAME%\AppData\Local\trillek\`
	*
	* on *nix platforms, XDG_CACHE_HOME will be respected if set.
	* \return The full Path. Empty/invalid if it fails
	*/
	static Path GetUserCachePath();

	/**
	* \brief Check if a directory exists
	*
	* \return true if the directory exists
	*/
	bool DirExists() const;

	/**
	* \brief Check if a file exists
	*
	* \return true if the file exists and is accessible
	*/
	bool FileExists() const;

	/**
	* \brief Try to create a directory
	*
	* \return true if success or the dir exists
	*/
	static bool MkDir(const Path& path);

	/**
	* \brief Try to create the directory chain of a full path
	*
	* \param path Absolute Path
	* \return True if success.
	*/
	static bool MkPath(const Path& path);

	// show a debug file listing
	static void Listing(const Path& path);

	/**
	* \brief Remove the file or directory at the given Path
	* \param path The Path to delete, if it's a directory, it must be empty.
	* This method will throw a tec::PathException if the file does not exist,
	* if the path is a non-empty directory, or if access otherwise prevents removing the file
	*/

	static void Remove(const Path& path);
	/**
	* \brief Remove everything at the given Path
	* \param path The Path to delete, this may be a file or directory.
	* \return the number of items deleted
	* This method will throw a tec::PathException if the path does not exist,
	* or if permissions otherwise prevent removing any file.
	*
	* This function attempts to verify that it can delete all files before actually doing so,
	* If any file fails the initial access checks, then no files or directories will be removed.
	* This method will also not permit deleting certain kinds of special files.
	*
	* Note: do not depend on the access check.
	* A set of files can pass the initial access check, but still fail to delete,
	* in which case RemoveAll will throw an exception and an unspecified number of files could be removed.
	*/
	static size_t RemoveAll(const Path& path);

	/**
	* \brief Extract the filename from a Path
	*
	* \return Empty string if is an invalid path for a file
	*/
	std::string FileName() const;

	/**
	* \brief Extract a filename without the extension from a Path to a file
	*
	* \return Empty string if is an invalid path for a file
	*/
	std::string FileStem() const;

	/**
	* \brief Extract an extension from a Path to a file
	*
	* \return Empty string if isn't a file or does not have an extension
	*/
	std::string FileExtension() const;

	/**
	* \brief Return base path of a full path
	*
	* - If this is a path to a file, returns a Path to the directory that contains the file.
	* - If this is a path to a directory, returns a Path to the containing directory (similar to `cd .. && pwd`)
	* \return Containing Path or Empty if it was an invalid Path. Returned path will have a final slash
	*/
	Path BasePath() const;

	/**
	* \brief Is this an absolute or relative Path?
	*/
	bool isAbsolutePath() const;

	/**
	* \brief Get the relative part of an absolute path
	* \return a relative path
	* - If this path is already relative, this has no effect.
	*/
	Path Relative() const;

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
	* \param include true to include the needle element in the result
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
	* \return Absolute Path to the current executable. Empty if this fails.
	*/
	static Path GetProgramPath();

	/**
	* \brief Open the file at this path as a C-style stream for C functions
	* \param open_mode what access is required, default is readonly access to existing files.
	* Will throw a PathException if the file does not exist, or if the open_mode is invalid.
	* \return a std::unique_ptr wrapped FILE pointer with a deleter for RAII.
	*/
	std::unique_ptr<FILE> OpenFile(PATH_OPEN_FLAGS open_mode = FS_DEFAULT) const;

	/**
	* \brief Open the file at this Path as a std::fstream
	* \param open_mode what access is required, default is readonly access to existing files.
	* Will throw a PathException if the file does not exist, or if the open_mode is invalid.
	* \return a pointer to a std::fstream.
	*/
	std::unique_ptr<std::fstream> OpenStream(PATH_OPEN_FLAGS open_mode = FS_DEFAULT) const;

	/**
	* \brief Attempts locate the assets directory
	* This function will try to search for the first valid directory, probing in this order:
	* - ./assets/
	* - EXE_PATH/assets/
	* - EXE_PATH/../assets/
	* - EXE_PATH/../share/assets/
	*
	* Were EXE_PATH is the value as returned by GetProgramPath().
	* The first valid path is cached, and can then be later read with GetAssetsBasePath().
	*/
	static void LocateAssets();

	/**
	* \brief Return the absolute Path to the assets directory
	*
	* - If this wasn't set, then it will first internally call LocateAssets()
	* - If you intend to load an asset, use `Path::assets / foo` or `Path("assets:/foo.bar")`
	*/
	static Path GetAssetsBasePath();

	/**
	* \brief Sets the directory root in which to search for assets
	* Calling this function while (or after) any assets are loading may have undesired results.
	*/
	static void SetAssetsBasePath(Path);

	/**
	* \brief Returns the string representation of this Path
	*/
	std::string toString() const { return device + path; }

	/**
	* \return true if this Path is empty
	*/
	bool empty() const { return device.empty() && path.empty(); }

	/**
	* \return true if this Path is valid.
	*/
	operator bool() const { return !path.empty() && !(!device.empty() && device.back() == '*'); }

	Path& operator=(const Path& rhs) {
		this->device = rhs.device;
		this->path = rhs.path; //Not necessary to normalize
		return *this;
	}

	Path& operator=(const std::string& str) {
		device.clear();
		path = str;
		SetDevice();
		NormalizePath();
		return *this;
	}

	Path& operator=(std::string&& str) {
		device.clear();
		path = std::move(str);
		SetDevice();
		NormalizePath();
		return *this;
	}

	Path& operator=(const std::wstring& wstr) {
		device.clear();
		path = utf8_encode(wstr);
		SetDevice();
		NormalizePath();
		return *this;
	}

	Path& operator=(const std::string_view& strv) {
		device.clear();
		path.assign(strv);
		SetDevice();
		NormalizePath();
		return *this;
	}

	template <size_t N> Path& operator=(const char (&stra)[N]) { return (*this = std::string_view(stra)); }

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
				// appending an invalid path or a path with a device is almost certainly bad
				throw PathException("Append invalid");
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
	/**
	* \brief Stores the root device name.
	* - Root devices are used to map files within the engine's virtual file system.
	* - On Windows platforms, this will be where the drive letter is stored for native paths.
	* - on *nix and OS X, the root device must always be unset for native paths.
	*/
	std::string device;
	std::string path; /// Stores path as an UTF8 string

	/**
	* \brief Internally build a Path from a device and path string.
	*/
	Path(const std::string& _device, const std::string& _path) : device{_device}, path{_path} {}

	/**
	* \brief Remove and save the device name from the path string if one exists.
	* - A path with a root device will always be considered absolute, and made to start with a `/`
	*/
	void SetDevice();

	/**
	* \brief Normalize this Path to the internal format
	*
	* - Converts separators to the generic format: `/`.
	* - Removes duplicate separators.
	* - Compacts all relative elements. i.e. `./thing` -> `thing`
	* - Removes trailing dots: `thing...` -> `thing`. This is for compatibility with Windows APIs
	* - Compacts upreferences, respecting the root. i.e. `/up/../../foo` -> `/foo`
	*/
	void NormalizePath();

	/**
	* \brief Returns a stringified path using the native OS encoding and representation.
	*
	* - return wstring using \ on Windows
	* - return string using / on *NIX
	* - throws PathException if this is a virtual path that can not be mapped to a native path.
	* \return native string of the path
	*/
	Path::NativePath GetNativePath() const;

	/**
	* \brief Get the current working directory for this process
	*/
	static Path GetWorkingDir();

	/**
	* \brief Verify if we should be able to remove all items at a Path, without actually removing them
	*/
	static void CheckRemoveAll(const Path& path);
	static size_t RemoveAll_Internal(const Path& path);

	// Cached paths
	static Path settings_folder;
	static Path udata_folder;
	static Path cache_folder;
	static Path assets_base;
	static const Path invalid_path;
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

/**
* \brief fmtcore formatter for Paths for use with spdlog (or similar)
* Examples
* \code
* Path p{"/root/foo.bar"};
* log->info("-> {}", p);    // -> /root/foo.bar
* log->info("-> {:f}", p);  // -> foo.bar
* log->info("-> {:fx}", p); // -> foo
* log->info("-> {:x}", p);  // -> bar
* log->info("-> {:r}", p);  // -> root/foo.bar
* log->info("-> {:qr}", p); // -> "root/foo.bar"
* \endcode
*/
template <> struct fmt::formatter<tec::Path> {
	bool filename = false;
	bool fileex = false;
	bool relative = false;
	bool quote = false;
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		auto it = ctx.begin(), end = ctx.end();
		if (it != end && *it != '}') {
			switch (*it) {
			case 'f': filename = true; return ++it;
			case 'x': fileex = true; return ++it;
			case 'r': relative = true; return ++it;
			case 'q': quote = true; return ++it;
			default: static_assert("invalid Path format");
			}
		}
		return it;
	}
	template <typename FormatContext> auto format(const tec::Path& p, FormatContext& ctx) const -> decltype(ctx.out()) {
		auto what = p;
		if (relative) {
			what = p.Relative();
		}
		std::string s;
		if (filename) {
			if (fileex) {
				s = what.FileStem();
			}
			else {
				s = what.FileName();
			}
		}
		else if (fileex) {
			s = what.FileExtension();
		}
		else {
			s = what.toString();
		}
		if (quote) {
			return fmt::format_to(ctx.out(), "\"{}\"", s);
		}
		return fmt::format_to(ctx.out(), "{}", s);
	}
};
