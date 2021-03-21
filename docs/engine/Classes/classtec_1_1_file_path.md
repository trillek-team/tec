---
title: tec::FilePath
summary: OS File system path separator. 

---

# tec::FilePath



[OS](/engine/Classes/classtec_1_1_o_s/) File system path separator. 
`#include <filesystem.hpp>`

## Public Types

|                | Name           |
| -------------- | -------------- |
| typedef std::string | **[NFilePath](/engine/Classes/classtec_1_1_file_path/#typedef-nfilepath)** <br>[OS](/engine/Classes/classtec_1_1_o_s/) File system path separator.  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[FilePath](/engine/Classes/classtec_1_1_file_path/#function-filepath)**()<br>Builds a empty path.  |
| | **[FilePath](/engine/Classes/classtec_1_1_file_path/#function-filepath)**(const std::string & other, std::size_t pos =0, std::size_t count =std::string::npos)<br>Builds a path from a string or substring.  |
| | **[FilePath](/engine/Classes/classtec_1_1_file_path/#function-filepath)**(const std::wstring & other, std::size_t pos =0, std::size_t count =std::wstring::npos)<br>Builds a path from a wstring or substring.  |
| bool | **[DirExists](/engine/Classes/classtec_1_1_file_path/#function-direxists)**() const<br>Check if a directory exists.  |
| bool | **[FileExists](/engine/Classes/classtec_1_1_file_path/#function-fileexists)**() const<br>Check if a file exists.  |
| std::string | **[FileName](/engine/Classes/classtec_1_1_file_path/#function-filename)**() const<br>Extract a filename from a path.  |
| std::string | **[FileExtension](/engine/Classes/classtec_1_1_file_path/#function-fileextension)**() const<br>Extract a extension from a path to a file.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[BasePath](/engine/Classes/classtec_1_1_file_path/#function-basepath)**() const<br>Return base path of a full path.  |
| bool | **[isAbsolutePath](/engine/Classes/classtec_1_1_file_path/#function-isabsolutepath)**() const<br>Is an absolute or relative path ?  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[Subpath](/engine/Classes/classtec_1_1_file_path/#function-subpath)**(std::size_t begin, std::size_t end =[FilePath::npos](/engine/Classes/classtec_1_1_file_path/#variable-npos)) const<br>Return a subpath.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[SubpathFrom](/engine/Classes/classtec_1_1_file_path/#function-subpathfrom)**(const std::string & needle, bool include =false) const<br>Return a subpath.  |
| void | **[NormalizePath](/engine/Classes/classtec_1_1_file_path/#function-normalizepath)**()<br>Normalize path to the [OS](/engine/Classes/classtec_1_1_o_s/) format.  |
| bool | **[isValidPath](/engine/Classes/classtec_1_1_file_path/#function-isvalidpath)**() const<br>Check if the path is a valid absolute o relative path.  |
| [FilePath::NFilePath](/engine/Classes/classtec_1_1_file_path/#typedef-nfilepath) | **[GetNativePath](/engine/Classes/classtec_1_1_file_path/#function-getnativepath)**() const<br>Returns a path on the native [OS](/engine/Classes/classtec_1_1_o_s/) encoding.  |
| std::string | **[toString](/engine/Classes/classtec_1_1_file_path/#function-tostring)**() const<br>Returns the string representation of a path.  |
| std::string | **[toGenericString](/engine/Classes/classtec_1_1_file_path/#function-togenericstring)**() const<br>Returns a generic string representation of a path (uses \ as path separator ALWAYS)  |
| bool | **[empty](/engine/Classes/classtec_1_1_file_path/#function-empty)**() const<br>Return if this [FilePath](/engine/Classes/classtec_1_1_file_path/) is empty.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator=](/engine/Classes/classtec_1_1_file_path/#function-operator=)**(const [FilePath](/engine/Classes/classtec_1_1_file_path/) & rhs) |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator=](/engine/Classes/classtec_1_1_file_path/#function-operator=)**(const std::string & str) |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator=](/engine/Classes/classtec_1_1_file_path/#function-operator=)**(const std::wstring & wstr) |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator=](/engine/Classes/classtec_1_1_file_path/#function-operator=)**(const char * str) |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator+=](/engine/Classes/classtec_1_1_file_path/#function-operator+=)**(const [FilePath](/engine/Classes/classtec_1_1_file_path/) & rhs)<br>Concatenate a path.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator+=](/engine/Classes/classtec_1_1_file_path/#function-operator+=)**(const char * lhs)<br>Concatenate a path.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator+=](/engine/Classes/classtec_1_1_file_path/#function-operator+=)**(const std::string & lhs)<br>Concatenate a path.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator+=](/engine/Classes/classtec_1_1_file_path/#function-operator+=)**(const std::wstring & lhs)<br>Concatenate a path.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator/=](/engine/Classes/classtec_1_1_file_path/#function-operator/=)**(const [FilePath](/engine/Classes/classtec_1_1_file_path/) & rhs)<br>Append a subdirectory or file.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator/=](/engine/Classes/classtec_1_1_file_path/#function-operator/=)**(const char * rhs)<br>Append a subdirectory or file.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator/=](/engine/Classes/classtec_1_1_file_path/#function-operator/=)**(const std::string & rhs)<br>Append a subdirectory or file.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) & | **[operator/=](/engine/Classes/classtec_1_1_file_path/#function-operator/=)**(const std::wstring & rhs)<br>Append a subdirectory or file.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[GetUserSettingsPath](/engine/Classes/classtec_1_1_file_path/#function-getusersettingspath)**()<br>Returns the path to the User settings folder.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[GetUserDataPath](/engine/Classes/classtec_1_1_file_path/#function-getuserdatapath)**()<br>Returns the path to the User persistent data folder (for save files, for example)  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[GetUserCachePath](/engine/Classes/classtec_1_1_file_path/#function-getusercachepath)**()<br>Returns the path to the User cache folder.  |
| bool | **[MkDir](/engine/Classes/classtec_1_1_file_path/#function-mkdir)**(const [FilePath](/engine/Classes/classtec_1_1_file_path/) & path)<br>Try to create a directory route.  |
| bool | **[MkPath](/engine/Classes/classtec_1_1_file_path/#function-mkpath)**(const [FilePath](/engine/Classes/classtec_1_1_file_path/) & path)<br>Try to create a full path.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[GetProgramPath](/engine/Classes/classtec_1_1_file_path/#function-getprogrampath)**()<br>Try to obtain the full path to the program binary file.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[GetAssetsBasePath](/engine/Classes/classtec_1_1_file_path/#function-getassetsbasepath)**()<br>Return the base directory where search the assets.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[GetAssetPath](/engine/Classes/classtec_1_1_file_path/#function-getassetpath)**(const [FilePath](/engine/Classes/classtec_1_1_file_path/) & asset)<br>returns the full path to an asset  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[GetAssetPath](/engine/Classes/classtec_1_1_file_path/#function-getassetpath)**(const std::string & asset)<br>returns the full path to an asset  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[GetAssetPath](/engine/Classes/classtec_1_1_file_path/#function-getassetpath)**(const char * asset)<br>returns the full path to an asset  |
| void | **[SetAssetsBasePath](/engine/Classes/classtec_1_1_file_path/#function-setassetsbasepath)**([FilePath](/engine/Classes/classtec_1_1_file_path/) new_base)<br>Sets the base directory where search the assets.  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| const char | **[PATH_SEPARATOR_C](/engine/Classes/classtec_1_1_file_path/#variable-path_separator_c)**  |
| const std::size_t | **[npos](/engine/Classes/classtec_1_1_file_path/#variable-npos)** <br>Native string format for paths.  |

## Public Types Documentation

### typedef NFilePath

```cpp
typedef std::string tec::FilePath::NFilePath;
```

[OS](/engine/Classes/classtec_1_1_o_s/) File system path separator. 

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
  * **end** Last element (default [FilePath::npos](/engine/Classes/classtec_1_1_file_path/#variable-npos))




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

Normalize path to the [OS](/engine/Classes/classtec_1_1_o_s/) format. 



* Convert slashes to the correct [OS](/engine/Classes/classtec_1_1_o_s/) slash
* Remove drive unit if is a *NIX [OS](/engine/Classes/classtec_1_1_o_s/)normalized path 


### function isValidPath

```cpp
bool isValidPath() const
```

Check if the path is a valid absolute o relative path. 

### function GetNativePath

```cpp
FilePath::NFilePath GetNativePath() const
```

Returns a path on the native [OS](/engine/Classes/classtec_1_1_o_s/) encoding. 



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

Return if this [FilePath](/engine/Classes/classtec_1_1_file_path/) is empty. 

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
* EXE_PATH/../share/assets/ Were EXE_PATH is the value returned by [GetProgramPath()](/engine/Classes/classtec_1_1_file_path/#function-getprogrampath) If find a valid path, then stores it for the future 


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

Updated on 21 March 2021 at 16:53:40 UTC