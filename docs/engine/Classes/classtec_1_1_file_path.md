---
title: tec::FilePath
summary: OS File system path separator. 

---

# tec::FilePath



[OS](Classes/classtec_1_1_o_s.md) File system path separator. 
`#include <filesystem.hpp>`

## Public Types

|                | Name           |
| -------------- | -------------- |
| typedef std::string | **[NFilePath](Classes/classtec_1_1_file_path.md#typedef-nfilepath)** <br>[OS](Classes/classtec_1_1_o_s.md) File system path separator.  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[FilePath](Classes/classtec_1_1_file_path.md#function-filepath)**()<br>Builds a empty path.  |
| | **[FilePath](Classes/classtec_1_1_file_path.md#function-filepath)**(const std::string & other, std::size_t pos =0, std::size_t count =std::string::npos)<br>Builds a path from a string or substring.  |
| | **[FilePath](Classes/classtec_1_1_file_path.md#function-filepath)**(const std::wstring & other, std::size_t pos =0, std::size_t count =std::wstring::npos)<br>Builds a path from a wstring or substring.  |
| bool | **[DirExists](Classes/classtec_1_1_file_path.md#function-direxists)**() const<br>Check if a directory exists.  |
| bool | **[FileExists](Classes/classtec_1_1_file_path.md#function-fileexists)**() const<br>Check if a file exists.  |
| std::string | **[FileName](Classes/classtec_1_1_file_path.md#function-filename)**() const<br>Extract a filename from a path.  |
| std::string | **[FileExtension](Classes/classtec_1_1_file_path.md#function-fileextension)**() const<br>Extract a extension from a path to a file.  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[BasePath](Classes/classtec_1_1_file_path.md#function-basepath)**() const<br>Return base path of a full path.  |
| bool | **[isAbsolutePath](Classes/classtec_1_1_file_path.md#function-isabsolutepath)**() const<br>Is an absolute or relative path ?  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[Subpath](Classes/classtec_1_1_file_path.md#function-subpath)**(std::size_t begin, std::size_t end =[FilePath::npos](Classes/classtec_1_1_file_path.md#variable-npos)) const<br>Return a subpath.  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[SubpathFrom](Classes/classtec_1_1_file_path.md#function-subpathfrom)**(const std::string & needle, bool include =false) const<br>Return a subpath.  |
| void | **[NormalizePath](Classes/classtec_1_1_file_path.md#function-normalizepath)**()<br>Normalize path to the [OS](Classes/classtec_1_1_o_s.md) format.  |
| bool | **[isValidPath](Classes/classtec_1_1_file_path.md#function-isvalidpath)**() const<br>Check if the path is a valid absolute o relative path.  |
| [FilePath::NFilePath](Classes/classtec_1_1_file_path.md#typedef-nfilepath) | **[GetNativePath](Classes/classtec_1_1_file_path.md#function-getnativepath)**() const<br>Returns a path on the native [OS](Classes/classtec_1_1_o_s.md) encoding.  |
| std::string | **[toString](Classes/classtec_1_1_file_path.md#function-tostring)**() const<br>Returns the string representation of a path.  |
| std::string | **[toGenericString](Classes/classtec_1_1_file_path.md#function-togenericstring)**() const<br>Returns a generic string representation of a path (uses \ as path separator ALWAYS)  |
| bool | **[empty](Classes/classtec_1_1_file_path.md#function-empty)**() const<br>Return if this [FilePath](Classes/classtec_1_1_file_path.md) is empty.  |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator=](Classes/classtec_1_1_file_path.md#function-operator=)**(const [FilePath](Classes/classtec_1_1_file_path.md) & rhs) |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator=](Classes/classtec_1_1_file_path.md#function-operator=)**(const std::string & str) |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator=](Classes/classtec_1_1_file_path.md#function-operator=)**(const std::wstring & wstr) |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator=](Classes/classtec_1_1_file_path.md#function-operator=)**(const char * str) |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator+=](Classes/classtec_1_1_file_path.md#function-operator+=)**(const [FilePath](Classes/classtec_1_1_file_path.md) & rhs)<br>Concatenate a path.  |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator+=](Classes/classtec_1_1_file_path.md#function-operator+=)**(const char * lhs)<br>Concatenate a path.  |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator+=](Classes/classtec_1_1_file_path.md#function-operator+=)**(const std::string & lhs)<br>Concatenate a path.  |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator+=](Classes/classtec_1_1_file_path.md#function-operator+=)**(const std::wstring & lhs)<br>Concatenate a path.  |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator/=](Classes/classtec_1_1_file_path.md#function-operator/=)**(const [FilePath](Classes/classtec_1_1_file_path.md) & rhs)<br>Append a subdirectory or file.  |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator/=](Classes/classtec_1_1_file_path.md#function-operator/=)**(const char * rhs)<br>Append a subdirectory or file.  |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator/=](Classes/classtec_1_1_file_path.md#function-operator/=)**(const std::string & rhs)<br>Append a subdirectory or file.  |
| [FilePath](Classes/classtec_1_1_file_path.md) & | **[operator/=](Classes/classtec_1_1_file_path.md#function-operator/=)**(const std::wstring & rhs)<br>Append a subdirectory or file.  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[GetUserSettingsPath](Classes/classtec_1_1_file_path.md#function-getusersettingspath)**()<br>Returns the path to the User settings folder.  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[GetUserDataPath](Classes/classtec_1_1_file_path.md#function-getuserdatapath)**()<br>Returns the path to the User persistent data folder (for save files, for example)  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[GetUserCachePath](Classes/classtec_1_1_file_path.md#function-getusercachepath)**()<br>Returns the path to the User cache folder.  |
| bool | **[MkDir](Classes/classtec_1_1_file_path.md#function-mkdir)**(const [FilePath](Classes/classtec_1_1_file_path.md) & path)<br>Try to create a directory route.  |
| bool | **[MkPath](Classes/classtec_1_1_file_path.md#function-mkpath)**(const [FilePath](Classes/classtec_1_1_file_path.md) & path)<br>Try to create a full path.  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[GetProgramPath](Classes/classtec_1_1_file_path.md#function-getprogrampath)**()<br>Try to obtain the full path to the program binary file.  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[GetAssetsBasePath](Classes/classtec_1_1_file_path.md#function-getassetsbasepath)**()<br>Return the base directory where search the assets.  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[GetAssetPath](Classes/classtec_1_1_file_path.md#function-getassetpath)**(const [FilePath](Classes/classtec_1_1_file_path.md) & asset)<br>returns the full path to an asset  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[GetAssetPath](Classes/classtec_1_1_file_path.md#function-getassetpath)**(const std::string & asset)<br>returns the full path to an asset  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[GetAssetPath](Classes/classtec_1_1_file_path.md#function-getassetpath)**(const char * asset)<br>returns the full path to an asset  |
| void | **[SetAssetsBasePath](Classes/classtec_1_1_file_path.md#function-setassetsbasepath)**([FilePath](Classes/classtec_1_1_file_path.md) new_base)<br>Sets the base directory where search the assets.  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| const char | **[PATH_SEPARATOR_C](Classes/classtec_1_1_file_path.md#variable-path_separator_c)**  |
| const std::size_t | **[npos](Classes/classtec_1_1_file_path.md#variable-npos)** <br>Native string format for paths.  |

## Public Types Documentation

### typedef NFilePath

```cpp
typedef std::string tec::FilePath::NFilePath;
```

[OS](Classes/classtec_1_1_o_s.md) File system path separator. 

## Public Functions Documentation

### function FilePath

```cpp
explicit FilePath()
```

Builds a empty path. 

### function FilePath

```cpp
FilePath(
    const std::string & other,
    std::size_t pos =0,
    std::size_t count =std::string::npos
)
```

Builds a path from a string or substring. 

**Parameters**: 

  * **other** A string with a path 
  * **pos** Begin of the range to get a slice (default = 0) 
  * **count** How many bytes to grab from other (default = size of other) 


### function FilePath

```cpp
FilePath(
    const std::wstring & other,
    std::size_t pos =0,
    std::size_t count =std::wstring::npos
)
```

Builds a path from a wstring or substring. 

**Parameters**: 

  * **other** A wstring with a path 
  * **pos** Begin of the range to get a slice (default = 0) 
  * **count** How many bytes to grab from other (default = size of other) 


### function DirExists

```cpp
bool DirExists() const
```

Check if a directory exists. 

**Return**: True if the directory exists 

### function FileExists

```cpp
bool FileExists() const
```

Check if a file exists. 

**Return**: True if the directory exists 

### function FileName

```cpp
std::string FileName() const
```

Extract a filename from a path. 

**Return**: Empty string if is an invalid path for a file 

### function FileExtension

```cpp
std::string FileExtension() const
```

Extract a extension from a path to a file. 

**Return**: Empty string if isn't a file or not have extension 

### function BasePath

```cpp
FilePath BasePath() const
```

Return base path of a full path. 

**Return**: Empty string if is an invalid path. Returned path would have the final slash 

If is a path of a file, returns the path to the dir that contains the file If is a path of a directory, returns the path to the dir that contains the directory (like cd .. && pwd) 


### function isAbsolutePath

```cpp
bool isAbsolutePath() const
```

Is an absolute or relative path ? 

### function Subpath

```cpp
FilePath Subpath(
    std::size_t begin,
    std::size_t end =FilePath::npos
) const
```

Return a subpath. 

**Parameters**: 

  * **begin** First element (each element is separated by a path separator) 
  * **end** Last element (default [FilePath::npos](Classes/classtec_1_1_file_path.md#variable-npos))




```cpp
FilePath f("./assets/foo/bar/mesh.obj");
f.Subpath(2, FilePath::npos); // ==> "/foo/bar/mesh.obj"
f.Subpath(0, 2); // ==> "./assets/"
```


### function SubpathFrom

```cpp
FilePath SubpathFrom(
    const std::string & needle,
    bool include =false
) const
```

Return a subpath. 

**Parameters**: 

  * **needle** path element to search 
  * **include** Includes the needle element on the output ?




```cpp
FilePath f("./assets/foo/bar/mesh.obj");
f.SubpathFrom(assets);       // ==> "/foo/bar/mesh.obj"
f.SubpathFrom(assets, true); // ==> "/assets/foo/bar/mesh.obj"
```


### function NormalizePath

```cpp
void NormalizePath()
```

Normalize path to the [OS](Classes/classtec_1_1_o_s.md) format. 



* Convert slashes to the correct [OS](Classes/classtec_1_1_o_s.md) slash
* Remove drive unit if is a *NIX [OS](Classes/classtec_1_1_o_s.md)normalized path 


### function isValidPath

```cpp
bool isValidPath() const
```

Check if the path is a valid absolute o relative path. 

### function GetNativePath

```cpp
FilePath::NFilePath GetNativePath() const
```

Returns a path on the native [OS](Classes/classtec_1_1_o_s.md) encoding. 



* Normalize path
* return wstring on Windows
* return string on *NIX native string of the path 


### function toString

```cpp
inline std::string toString() const
```

Returns the string representation of a path. 

### function toGenericString

```cpp
std::string toGenericString() const
```

Returns a generic string representation of a path (uses \ as path separator ALWAYS) 

### function empty

```cpp
inline bool empty() const
```

Return if this [FilePath](Classes/classtec_1_1_file_path.md) is empty. 

### function operator=

```cpp
inline FilePath & operator=(
    const FilePath & rhs
)
```


### function operator=

```cpp
inline FilePath & operator=(
    const std::string & str
)
```


### function operator=

```cpp
inline FilePath & operator=(
    const std::wstring & wstr
)
```


### function operator=

```cpp
inline FilePath & operator=(
    const char * str
)
```


### function operator+=

```cpp
inline FilePath & operator+=(
    const FilePath & rhs
)
```

Concatenate a path. 

### function operator+=

```cpp
inline FilePath & operator+=(
    const char * lhs
)
```

Concatenate a path. 

### function operator+=

```cpp
inline FilePath & operator+=(
    const std::string & lhs
)
```

Concatenate a path. 

### function operator+=

```cpp
inline FilePath & operator+=(
    const std::wstring & lhs
)
```

Concatenate a path. 

### function operator/=

```cpp
inline FilePath & operator/=(
    const FilePath & rhs
)
```

Append a subdirectory or file. 

### function operator/=

```cpp
inline FilePath & operator/=(
    const char * rhs
)
```

Append a subdirectory or file. 

### function operator/=

```cpp
inline FilePath & operator/=(
    const std::string & rhs
)
```

Append a subdirectory or file. 

### function operator/=

```cpp
inline FilePath & operator/=(
    const std::wstring & rhs
)
```

Append a subdirectory or file. 

### function GetUserSettingsPath

```cpp
static FilePath GetUserSettingsPath()
```

Returns the path to the User settings folder. 

**Return**: string with the full path. Empty string if fails 

Usually this paths are : *nix : /home/USER/.config/APPNAME/ OSX : /Users/USER/Library/Application Support/APPNAME/ WIN : C:\Users\USER\AppData\Roaming\APPNAME\


### function GetUserDataPath

```cpp
static FilePath GetUserDataPath()
```

Returns the path to the User persistent data folder (for save files, for example) 

**Return**: string with the full path. Empty string if fails 

Usually this paths are : *nix : /home/USER/.local/share/APPNAME/ OSX : /Users/USER/Library/Application Support/APPNAME/data/ WIN : C:\Users\USER\AppData\Roaming\APPNAME\data\


### function GetUserCachePath

```cpp
static FilePath GetUserCachePath()
```

Returns the path to the User cache folder. 

**Return**: string with the full path. Empty string if fails 

Usually this paths are : *nix : /home/USER/.cache/APPNAME/ OSX : /Users/USER/Library/Application Support/APPNAME/cache/ WIN : C:\Users\USER\AppData\Local\APPNAME\


### function MkDir

```cpp
static bool MkDir(
    const FilePath & path
)
```

Try to create a directory route. 

**Return**: True if success or the dir exists 

### function MkPath

```cpp
static bool MkPath(
    const FilePath & path
)
```

Try to create a full path. 

**Parameters**: 

  * **path** Absolute path 


**Return**: True if success. 

### function GetProgramPath

```cpp
static FilePath GetProgramPath()
```

Try to obtain the full path to the program binary file. 

**Return**: string with the full path. Empty string if fails 

### function GetAssetsBasePath

```cpp
static FilePath GetAssetsBasePath()
```

Return the base directory where search the assets. 

If isn't set, then would try to search a valid directory path, probing with this paths:

* ./assets/
* EXE_PATH/assets/
* EXE_PATH/../assets/
* EXE_PATH/../share/assets/ Were EXE_PATH is the value returned by [GetProgramPath()](Classes/classtec_1_1_file_path.md#function-getprogrampath) If find a valid path, then stores it for the future 


### function GetAssetPath

```cpp
static FilePath GetAssetPath(
    const FilePath & asset
)
```

returns the full path to an asset 

**Parameters**: 

  * **asset** Relative path to asset base folder that identify a asset file (for example "shaders/foo.vert") 


### function GetAssetPath

```cpp
static FilePath GetAssetPath(
    const std::string & asset
)
```

returns the full path to an asset 

**Parameters**: 

  * **asset** String path-like that identify a asset file (for example "shaders/foo.vert") 


### function GetAssetPath

```cpp
static FilePath GetAssetPath(
    const char * asset
)
```

returns the full path to an asset 

**Parameters**: 

  * **asset** String path-like that identify a asset file (for example "shaders/foo.vert") 


### function SetAssetsBasePath

```cpp
static void SetAssetsBasePath(
    FilePath new_base
)
```

Sets the base directory where search the assets. 

## Public Attributes Documentation

### variable PATH_SEPARATOR_C

```cpp
static const char PATH_SEPARATOR_C = '/';
```


### variable npos

```cpp
static const std::size_t npos = std::string::npos;
```

Native string format for paths. 

-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC