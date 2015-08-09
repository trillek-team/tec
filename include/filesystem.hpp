#pragma once
/**
 * OS Filesystem abstraction stuff
 */

#include "string.hpp"

#include <string>
#include <ostream>

namespace tec {
	class FilePath final {
public:

	const static std::string PATH_SEPARATOR;  /// OS Filesystem path separator
#if defined(__unix__)
		 //("/"); 
		const static char PATH_SEPARATOR_C = '/';      /// OS Filesystem path separator
#else
		const static std::string PATH_SEPARATOR; //("\\"); /// OS Filesystem path separator
		const static char PATH_SEPARATOR_C = '\\';     /// OS Filesystem path separator
#endif

#if defined(WIN32)
		typedef std::wstring NFilePath; /// Native string format for paths
#else
		typedef std::string NFilePath;  /// Native string format for paths
#endif

		/**
		 * \brief Builds a empty path
		 */
		explicit FilePath();
		
		/**
		 * \brief Builds a path from a string or substring
		 * 
		 * \param other A string with a path
		 * \param pos Begin of the range to get a slice (default = 0)
		 * \param count How many bytes to grab from other (default = size of other)
		 */
		FilePath( const std::string& other, std::size_t pos = 0, std::size_t count = std::string::npos);
		
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
		static FilePath GetUserSettingsPath();

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
		static FilePath GetUserDataPath();

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
		static FilePath GetUserCachePath();

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
		static bool MkDir(const FilePath& path);

		/**
		 * \brief Try to create a full path
		 *
		 * \param path Absolute path
		 * \return True if success.
		 */
		static bool MkPath(const FilePath& path);

		/**
		 * \brief Extract a filename from a path
		 *
		 * \return Empty string if is an invalid path for a file
		 */
		FilePath FileName() const;

		/**
		 * \brief Return base path of a full path
		 *
		 * If is a path of a file, returns the path to the dir that contains the file
		 * If is a path of a directory, returns the path to the dir that contains the direcotory (like cd .. && pwd)
		 * \retirm Empty string if is an invalid path. Returned path would have the final slash
		 */
		FilePath BasePath() const;

		/**
		 * \brief Is an absolute or relative path ?
		 */
		bool isAbsolutePath() const;

		/**
		 * \brief Try to obtain the full path to the program binary file
		 *
		 * \return string with the full path. Empty string if fails
		 */
		static FilePath GetProgramPath();

		/**
		 * \brief Normalize path to the OS format
		 *
		 * - Convert slashs to the correct OS slash
		 * - Remove drive unit if is a *NIX OS
		 * \return normalized path
		 */
		void NormalizePath();

		/**
		 * \brief Returns a path on the native OS encoding
		 *
		 * - Normalize path
		 * - return wstring on Windows
		 * - return string on *NIX
		 * \return native string of the path
		 */
		FilePath::NFilePath GetNativePath() const;
		
		/**
		 * \brief Returns the string representation of a path
		 */
		std::string toString() const{
			return this->path;
		}
		
		/**
		 * \brief Return if this FilePath is empty
		 */
		bool empty() {
			return this->path.empty();
		}
		
		FilePath& operator= (const FilePath& rhs) {
			this->path = rhs.path;
			this->NormalizePath();
			return *this;
		}
		
		FilePath& operator= (const std::string& str) {
			this->path = str;
			this->NormalizePath();
			return *this;
		}
		/*
		FilePath& operator= (const std::wstring& wstr) {
			this->path = utf8_encode(wstr);
			this->NormalizePath();
			return *this;
		}*/
		
		FilePath& operator= (const char* str) {
			this->path = std::string(str);
			this->NormalizePath();
			return *this;
		}
		
		FilePath& operator+= (const FilePath& rhs) {
			this->path += rhs.path;
			this->NormalizePath();
			return *this;
		}
		
		friend FilePath operator+ (FilePath lhs, const FilePath& rhs) {
			return lhs += rhs;
		}
		
		friend FilePath operator+ (FilePath lhs, const std::string& str) {
			FilePath tmp(str);
			return lhs += tmp;
		}
		/*
		friend FilePath operator+ (FilePath lhs, const std::wstring& wstr) {
			FilePath tmp(wstr);
			return lhs += tmp;
		}*/
		
		friend FilePath operator+ (FilePath lhs, const char* str) {
			FilePath tmp(str);
			return lhs += tmp;
		}
		
private:
		std::string path; /// Stores path as an UTF8 string

		// Cached paths
		static std::string settings_folder;
		static std::string udata_folder;
		static std::string cache_folder;
		
		//friend std::ostream& operator<<(std::ostream& os, const FilePath& path);

	}; // End of FileSystem
	
	/**
	 * \brief Output to a stream
	 */
	template <typename charT, typename traits>
	std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,traits>& os, const FilePath& path)
	{
		
		return os << path.toString();
	}
	
	/**
	 * \brief Input from a stream
	 */
	template <typename charT, typename traits>
	std::basic_istream<charT,traits> & operator>> (std::basic_istream<charT,traits>& is, 
			   FilePath& path) {
		std::string tmp;
		is >> tmp;
		path = FilePath(tmp);
	}

}

