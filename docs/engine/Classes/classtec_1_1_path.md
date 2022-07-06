---
title: tec::Path

---

# tec::Path





## Public Types

|                | Name           |
| -------------- | -------------- |
| typedef std::string | **[NativePath](/engine/Classes/classtec_1_1_path/#typedef-nativepath)** <br>[OS](/engine/Classes/classtec_1_1_o_s/) File system path separator.  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Path](/engine/Classes/classtec_1_1_path/#function-path)**()<br>`assets:/shaders/` where to find shaders  |
| | **[Path](/engine/Classes/classtec_1_1_path/#function-path)**(const std::string_view & other)<br>Builds a [Path](/engine/Classes/classtec_1_1_path/) from a string_view or substring there of.  |
| | **[Path](/engine/Classes/classtec_1_1_path/#function-path)**(const std::string & other, std::size_t pos =0, std::size_t count =std::string::npos)<br>Builds a [Path](/engine/Classes/classtec_1_1_path/) from a string or substring.  |
| | **[Path](/engine/Classes/classtec_1_1_path/#function-path)**(std::string && other) |
| template <size_t N\> <br>| **[Path](/engine/Classes/classtec_1_1_path/#function-path)**(const char(&) other[N])<br>This template builds a [Path](/engine/Classes/classtec_1_1_path/) from a string literal.  |
| | **[Path](/engine/Classes/classtec_1_1_path/#function-path)**(const std::wstring & other)<br>Builds a path from a wstring.  |
| bool | **[DirExists](/engine/Classes/classtec_1_1_path/#function-direxists)**() const<br>Check if a directory exists.  |
| bool | **[FileExists](/engine/Classes/classtec_1_1_path/#function-fileexists)**() const<br>Check if a file exists.  |
| std::string | **[FileName](/engine/Classes/classtec_1_1_path/#function-filename)**() const<br>Extract the filename from a [Path](/engine/Classes/classtec_1_1_path/).  |
| std::string | **[FileStem](/engine/Classes/classtec_1_1_path/#function-filestem)**() const<br>Extract a filename without the extension from a [Path](/engine/Classes/classtec_1_1_path/) to a file.  |
| std::string | **[FileExtension](/engine/Classes/classtec_1_1_path/#function-fileextension)**() const<br>Extract an extension from a [Path](/engine/Classes/classtec_1_1_path/) to a file.  |
| [Path](/engine/Classes/classtec_1_1_path/) | **[BasePath](/engine/Classes/classtec_1_1_path/#function-basepath)**() const<br>Return base path of a full path.  |
| bool | **[isAbsolutePath](/engine/Classes/classtec_1_1_path/#function-isabsolutepath)**() const<br>Is this an absolute or relative [Path](/engine/Classes/classtec_1_1_path/)?  |
| [Path](/engine/Classes/classtec_1_1_path/) | **[Relative](/engine/Classes/classtec_1_1_path/#function-relative)**() const<br>Get the relative part of an absolute path.  |
| [Path](/engine/Classes/classtec_1_1_path/) | **[Subpath](/engine/Classes/classtec_1_1_path/#function-subpath)**(std::size_t begin, std::size_t end =[Path::npos](/engine/Classes/classtec_1_1_path/#variable-npos)) const<br>Return a subpath.  |
| [Path](/engine/Classes/classtec_1_1_path/) | **[SubpathFrom](/engine/Classes/classtec_1_1_path/#function-subpathfrom)**(const std::string & needle, bool include =false) const<br>Return a subpath.  |
| std::unique_ptr< FILE > | **[OpenFile](/engine/Classes/classtec_1_1_path/#function-openfile)**(PATH_OPEN_FLAGS open_mode =FS_DEFAULT) const<br>Open the file at this path as a C-style stream for C functions.  |
| std::unique_ptr< std::fstream > | **[OpenStream](/engine/Classes/classtec_1_1_path/#function-openstream)**(PATH_OPEN_FLAGS open_mode =FS_DEFAULT) const<br>Open the file at this [Path](/engine/Classes/classtec_1_1_path/) as a std::fstream.  |
| std::string | **[toString](/engine/Classes/classtec_1_1_path/#function-tostring)**() const<br>Returns the string representation of this [Path](/engine/Classes/classtec_1_1_path/).  |
| bool | **[empty](/engine/Classes/classtec_1_1_path/#function-empty)**() const |
| | **[operator bool](/engine/Classes/classtec_1_1_path/#function-operator-bool)**() const |
| [Path](/engine/Classes/classtec_1_1_path/) & | **[operator=](/engine/Classes/classtec_1_1_path/#function-operator=)**(const [Path](/engine/Classes/classtec_1_1_path/) & rhs) |
| [Path](/engine/Classes/classtec_1_1_path/) & | **[operator=](/engine/Classes/classtec_1_1_path/#function-operator=)**(const std::string & str) |
| [Path](/engine/Classes/classtec_1_1_path/) & | **[operator=](/engine/Classes/classtec_1_1_path/#function-operator=)**(std::string && str) |
| [Path](/engine/Classes/classtec_1_1_path/) & | **[operator=](/engine/Classes/classtec_1_1_path/#function-operator=)**(const std::wstring & wstr) |
| [Path](/engine/Classes/classtec_1_1_path/) & | **[operator=](/engine/Classes/classtec_1_1_path/#function-operator=)**(const std::string_view & strv) |
| template <size_t N\> <br>[Path](/engine/Classes/classtec_1_1_path/) & | **[operator=](/engine/Classes/classtec_1_1_path/#function-operator=)**(const char(&) stra[N]) |
| [Path](/engine/Classes/classtec_1_1_path/) & | **[operator/=](/engine/Classes/classtec_1_1_path/#function-operator/=)**(const [Path](/engine/Classes/classtec_1_1_path/) & rhs)<br>Append a subdirectory or file.  |
| [Path](/engine/Classes/classtec_1_1_path/) & | **[operator/=](/engine/Classes/classtec_1_1_path/#function-operator/=)**(const std::string_view rhs)<br>Append a subdirectory or file.  |
| [Path](/engine/Classes/classtec_1_1_path/) & | **[operator/=](/engine/Classes/classtec_1_1_path/#function-operator/=)**(const char * rhs)<br>Append a subdirectory or file.  |
| [Path](/engine/Classes/classtec_1_1_path/) & | **[operator/=](/engine/Classes/classtec_1_1_path/#function-operator/=)**(const std::string & rhs)<br>Append a subdirectory or file.  |
| [Path](/engine/Classes/classtec_1_1_path/) & | **[operator/=](/engine/Classes/classtec_1_1_path/#function-operator/=)**(const std::wstring & rhs)<br>Append a subdirectory or file.  |
| [Path](/engine/Classes/classtec_1_1_path/) | **[GetUserSettingsPath](/engine/Classes/classtec_1_1_path/#function-getusersettingspath)**()<br>Returns the path to the User settings folder.  |
| [Path](/engine/Classes/classtec_1_1_path/) | **[GetUserDataPath](/engine/Classes/classtec_1_1_path/#function-getuserdatapath)**()<br>Returns the path to the User persistent data folder (for save files, for example)  |
| [Path](/engine/Classes/classtec_1_1_path/) | **[GetUserCachePath](/engine/Classes/classtec_1_1_path/#function-getusercachepath)**()<br>Returns the path to the User cache folder.  |
| bool | **[MkDir](/engine/Classes/classtec_1_1_path/#function-mkdir)**(const [Path](/engine/Classes/classtec_1_1_path/) & path)<br>Try to create a directory.  |
| bool | **[MkPath](/engine/Classes/classtec_1_1_path/#function-mkpath)**(const [Path](/engine/Classes/classtec_1_1_path/) & path)<br>Try to create the directory chain of a full path.  |
| void | **[Listing](/engine/Classes/classtec_1_1_path/#function-listing)**(const [Path](/engine/Classes/classtec_1_1_path/) & path) |
| void | **[Remove](/engine/Classes/classtec_1_1_path/#function-remove)**(const [Path](/engine/Classes/classtec_1_1_path/) & path)<br>Remove the file or directory at the given [Path](/engine/Classes/classtec_1_1_path/).  |
| size_t | **[RemoveAll](/engine/Classes/classtec_1_1_path/#function-removeall)**(const [Path](/engine/Classes/classtec_1_1_path/) & path)<br>Remove everything at the given [Path](/engine/Classes/classtec_1_1_path/).  |
| [Path](/engine/Classes/classtec_1_1_path/) | **[GetProgramPath](/engine/Classes/classtec_1_1_path/#function-getprogrampath)**()<br>Try to obtain the full path to the program binary file.  |
| void | **[LocateAssets](/engine/Classes/classtec_1_1_path/#function-locateassets)**()<br>Attempts locate the assets directory This function will try to search for the first valid directory, probing in this order:  |
| [Path](/engine/Classes/classtec_1_1_path/) | **[GetAssetsBasePath](/engine/Classes/classtec_1_1_path/#function-getassetsbasepath)**()<br>Return the absolute [Path](/engine/Classes/classtec_1_1_path/) to the assets directory.  |
| void | **[SetAssetsBasePath](/engine/Classes/classtec_1_1_path/#function-setassetsbasepath)**([Path](/engine/Classes/classtec_1_1_path/) new_base)<br>Sets the directory root in which to search for assets Calling this function while (or after) any assets are loading may have undesired results.  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr char | **[PATH_NATIVE_CHAR](/engine/Classes/classtec_1_1_path/#variable-path-native-char)**  |
| constexpr char | **[PATH_CHAR](/engine/Classes/classtec_1_1_path/#variable-path-char)** <br>Native string format for paths.  |
| const std::size_t | **[npos](/engine/Classes/classtec_1_1_path/#variable-npos)** <br>The tec [Path](/engine/Classes/classtec_1_1_path/) separator.  |
| const [Path](/engine/Classes/classtec_1_1_path/) | **[assets](/engine/Classes/classtec_1_1_path/#variable-assets)**  |
| const [Path](/engine/Classes/classtec_1_1_path/) | **[scripts](/engine/Classes/classtec_1_1_path/#variable-scripts)** <br>The `assets:/` pseudo root.  |
| const [Path](/engine/Classes/classtec_1_1_path/) | **[shaders](/engine/Classes/classtec_1_1_path/#variable-shaders)** <br>`assets:/scripts/` where to find scripts  |

## Public Types Documentation

### typedef NativePath

```cpp
typedef std::string tec::Path::NativePath;
```

[OS](/engine/Classes/classtec_1_1_o_s/) File system path separator. 

## Public Functions Documentation

### function Path

```cpp
explicit Path()
```

`assets:/shaders/` where to find shaders 

Builds an empty [Path](/engine/Classes/classtec_1_1_path/)


### function Path

```cpp
explicit Path(
    const std::string_view & other
)
```

Builds a [Path](/engine/Classes/classtec_1_1_path/) from a string_view or substring there of. 

**Parameters**: 

  * **other** A string_view with a path 


### function Path

```cpp
explicit Path(
    const std::string & other,
    std::size_t pos =0,
    std::size_t count =std::string::npos
)
```

Builds a [Path](/engine/Classes/classtec_1_1_path/) from a string or substring. 

**Parameters**: 

  * **other** A string with a path 
  * **pos** Begin of the range to get a slice (default = 0) 
  * **count** How many bytes to grab from other (default = size of other) 


### function Path

```cpp
explicit Path(
    std::string && other
)
```


### function Path

```cpp
template <size_t N>
inline Path(
    const char(&) other[N]
)
```

This template builds a [Path](/engine/Classes/classtec_1_1_path/) from a string literal. 

### function Path

```cpp
explicit Path(
    const std::wstring & other
)
```

Builds a path from a wstring. 

**Parameters**: 

  * **other** A wstring containing a path 


### function DirExists

```cpp
bool DirExists() const
```

Check if a directory exists. 

**Return**: true if the directory exists 

### function FileExists

```cpp
bool FileExists() const
```

Check if a file exists. 

**Return**: true if the file exists and is accessible 

### function FileName

```cpp
std::string FileName() const
```

Extract the filename from a [Path](/engine/Classes/classtec_1_1_path/). 

**Return**: Empty string if is an invalid path for a file 

### function FileStem

```cpp
std::string FileStem() const
```

Extract a filename without the extension from a [Path](/engine/Classes/classtec_1_1_path/) to a file. 

**Return**: Empty string if is an invalid path for a file 

### function FileExtension

```cpp
std::string FileExtension() const
```

Extract an extension from a [Path](/engine/Classes/classtec_1_1_path/) to a file. 

**Return**: Empty string if isn't a file or does not have an extension 

### function BasePath

```cpp
Path BasePath() const
```

Return base path of a full path. 



* If this is a path to a file, returns a [Path](/engine/Classes/classtec_1_1_path/) to the directory that contains the file.
* If this is a path to a directory, returns a [Path](/engine/Classes/classtec_1_1_path/) to the containing directory (similar to `cd .. && pwd`) Containing [Path](/engine/Classes/classtec_1_1_path/) or Empty if it was an invalid [Path](/engine/Classes/classtec_1_1_path/). Returned path will have a final slash 


### function isAbsolutePath

```cpp
bool isAbsolutePath() const
```

Is this an absolute or relative [Path](/engine/Classes/classtec_1_1_path/)? 

### function Relative

```cpp
Path Relative() const
```

Get the relative part of an absolute path. 

**Return**: a relative path

* If this path is already relative, this has no effect. 

### function Subpath

```cpp
Path Subpath(
    std::size_t begin,
    std::size_t end =Path::npos
) const
```

Return a subpath. 

**Parameters**: 

  * **begin** First element (each element is separated by a path separator) 
  * **end** Last element (default [Path::npos](/engine/Classes/classtec_1_1_path/#variable-npos))




```cpp
Path f("assets/foo/bar/mesh.obj");
f.Subpath(2, Path::npos); // ==> "bar/mesh.obj"
f.Subpath(0, 2); // ==> "assets/foo"
```


### function SubpathFrom

```cpp
Path SubpathFrom(
    const std::string & needle,
    bool include =false
) const
```

Return a subpath. 

**Parameters**: 

  * **needle** path element to search 
  * **include** true to include the needle element in the result




```cpp
Path f("./assets/foo/bar/mesh.obj");
f.SubpathFrom("assets");       // ==> "foo/bar/mesh.obj"
f.SubpathFrom("assets", true); // ==> "assets/foo/bar/mesh.obj"
```


### function OpenFile

```cpp
std::unique_ptr< FILE > OpenFile(
    PATH_OPEN_FLAGS open_mode =FS_DEFAULT
) const
```

Open the file at this path as a C-style stream for C functions. 

**Parameters**: 

  * **open_mode** what access is required, default is readonly access to existing files. Will throw a [PathException](/engine/Classes/classtec_1_1_path_exception/) if the file does not exist, or if the open_mode is invalid. 


**Return**: a std::unique_ptr wrapped FILE pointer with a deleter for RAII. 

### function OpenStream

```cpp
std::unique_ptr< std::fstream > OpenStream(
    PATH_OPEN_FLAGS open_mode =FS_DEFAULT
) const
```

Open the file at this [Path](/engine/Classes/classtec_1_1_path/) as a std::fstream. 

**Parameters**: 

  * **open_mode** what access is required, default is readonly access to existing files. Will throw a [PathException](/engine/Classes/classtec_1_1_path_exception/) if the file does not exist, or if the open_mode is invalid. 


**Return**: a pointer to a std::fstream. 

### function toString

```cpp
inline std::string toString() const
```

Returns the string representation of this [Path](/engine/Classes/classtec_1_1_path/). 

### function empty

```cpp
inline bool empty() const
```


**Return**: true if this [Path](/engine/Classes/classtec_1_1_path/) is empty 

### function operator bool

```cpp
inline operator bool() const
```


**Return**: true if this [Path](/engine/Classes/classtec_1_1_path/) is valid. 

### function operator=

```cpp
inline Path & operator=(
    const Path & rhs
)
```


### function operator=

```cpp
inline Path & operator=(
    const std::string & str
)
```


### function operator=

```cpp
inline Path & operator=(
    std::string && str
)
```


### function operator=

```cpp
inline Path & operator=(
    const std::wstring & wstr
)
```


### function operator=

```cpp
inline Path & operator=(
    const std::string_view & strv
)
```


### function operator=

```cpp
template <size_t N>
inline Path & operator=(
    const char(&) stra[N]
)
```


### function operator/=

```cpp
inline Path & operator/=(
    const Path & rhs
)
```

Append a subdirectory or file. 

### function operator/=

```cpp
inline Path & operator/=(
    const std::string_view rhs
)
```

Append a subdirectory or file. 

### function operator/=

```cpp
inline Path & operator/=(
    const char * rhs
)
```

Append a subdirectory or file. 

### function operator/=

```cpp
inline Path & operator/=(
    const std::string & rhs
)
```

Append a subdirectory or file. 

### function operator/=

```cpp
inline Path & operator/=(
    const std::wstring & rhs
)
```

Append a subdirectory or file. 

### function GetUserSettingsPath

```cpp
static Path GetUserSettingsPath()
```

Returns the path to the User settings folder. 

**Return**: The full [Path](/engine/Classes/classtec_1_1_path/). Empty/invalid if it fails 

Usually these paths are:

* *nix : `/home/$USER/.config/APPNAME/`
* OSX : `/Users/$USER/Library/Application Support/APPNAME/`
* WIN : `APPDATA%\APPNAME\` i.e. `C:\Users\USERNAME%\AppData\Roaming\trillek\`
on *nix platforms, XDG_CONFIG_HOME will be respected if set. 


### function GetUserDataPath

```cpp
static Path GetUserDataPath()
```

Returns the path to the User persistent data folder (for save files, for example) 

**Return**: The full [Path](/engine/Classes/classtec_1_1_path/). Empty/invalid if it fails 

Usually these paths are:

* *nix: `/home/$USER/.local/share/APPNAME/`
* [OS](/engine/Classes/classtec_1_1_o_s/) X: `/Users/$USER/Library/Application Support/APPNAME/data/`
* WIN : `APPDATA%\APPNAME\` i.e. `C:\Users\USERNAME%\AppData\Roaming\trillek\data\`
on *nix platforms, XDG_DATA_HOME will be respected if set. 


### function GetUserCachePath

```cpp
static Path GetUserCachePath()
```

Returns the path to the User cache folder. 

**Return**: The full [Path](/engine/Classes/classtec_1_1_path/). Empty/invalid if it fails 

Usually these paths are:

* *nix: `/home/$USER/.cache/APPNAME/`
* [OS](/engine/Classes/classtec_1_1_o_s/) X: `/Users/$USER/Library/Application Support/APPNAME/cache/`
* WIN : `LOCALAPPDATA%\APPNAME\` i.e. `C:\Users\USERNAME%\AppData\Local\trillek\`
on *nix platforms, XDG_CACHE_HOME will be respected if set. 


### function MkDir

```cpp
static bool MkDir(
    const Path & path
)
```

Try to create a directory. 

**Return**: true if success or the dir exists 

### function MkPath

```cpp
static bool MkPath(
    const Path & path
)
```

Try to create the directory chain of a full path. 

**Parameters**: 

  * **path** Absolute [Path](/engine/Classes/classtec_1_1_path/)


**Return**: True if success. 

### function Listing

```cpp
static void Listing(
    const Path & path
)
```


### function Remove

```cpp
static void Remove(
    const Path & path
)
```

Remove the file or directory at the given [Path](/engine/Classes/classtec_1_1_path/). 

**Parameters**: 

  * **path** The [Path](/engine/Classes/classtec_1_1_path/) to delete, if it's a directory, it must be empty. This method will throw a [tec::PathException](/engine/Classes/classtec_1_1_path_exception/) if the file does not exist, if the path is a non-empty directory, or if access otherwise prevents removing the file 


### function RemoveAll

```cpp
static size_t RemoveAll(
    const Path & path
)
```

Remove everything at the given [Path](/engine/Classes/classtec_1_1_path/). 

**Parameters**: 

  * **path** The [Path](/engine/Classes/classtec_1_1_path/) to delete, this may be a file or directory. 


**Return**: the number of items deleted This method will throw a [tec::PathException](/engine/Classes/classtec_1_1_path_exception/) if the path does not exist, or if permissions otherwise prevent removing any file.


This function attempts to verify that it can delete all files before actually doing so, If any file fails the initial access checks, then no files or directories will be removed. This method will also not permit deleting certain kinds of special files.

Note: do not depend on the access check. A set of files can pass the initial access check, but still fail to delete, in which case RemoveAll will throw an exception and an unspecified number of files could be removed. 


### function GetProgramPath

```cpp
static Path GetProgramPath()
```

Try to obtain the full path to the program binary file. 

**Return**: Absolute [Path](/engine/Classes/classtec_1_1_path/) to the current executable. Empty if this fails. 

### function LocateAssets

```cpp
static void LocateAssets()
```

Attempts locate the assets directory This function will try to search for the first valid directory, probing in this order: 



* ./assets/
* EXE_PATH/assets/
* EXE_PATH/../assets/
* EXE_PATH/../share/assets/
Were EXE_PATH is the value as returned by [GetProgramPath()](/engine/Classes/classtec_1_1_path/#function-getprogrampath). The first valid path is cached, and can then be later read with [GetAssetsBasePath()](/engine/Classes/classtec_1_1_path/#function-getassetsbasepath). 


### function GetAssetsBasePath

```cpp
static Path GetAssetsBasePath()
```

Return the absolute [Path](/engine/Classes/classtec_1_1_path/) to the assets directory. 



* If this wasn't set, then it will first internally call [LocateAssets()](/engine/Classes/classtec_1_1_path/#function-locateassets)
* If you intend to load an asset, use `Path::assets / foo` or `[Path](/engine/Classes/classtec_1_1_path/)("assets:/foo.bar")`


### function SetAssetsBasePath

```cpp
static void SetAssetsBasePath(
    Path new_base
)
```

Sets the directory root in which to search for assets Calling this function while (or after) any assets are loading may have undesired results. 

## Public Attributes Documentation

### variable PATH_NATIVE_CHAR

```cpp
static constexpr char PATH_NATIVE_CHAR = '/';
```


### variable PATH_CHAR

```cpp
static constexpr char PATH_CHAR = '/';
```

Native string format for paths. 

### variable npos

```cpp
static const std::size_t npos = std::string::npos;
```

The tec [Path](/engine/Classes/classtec_1_1_path/) separator. 

a value indicating "far as possible" 


### variable assets

```cpp
static const Path assets {"assets:", "/"};
```


### variable scripts

```cpp
static const Path scripts {"assets:", "/scripts/"};
```

The `assets:/` pseudo root. 

### variable shaders

```cpp
static const Path shaders {"assets:", "/shaders/"};
```

`assets:/scripts/` where to find scripts 

-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000