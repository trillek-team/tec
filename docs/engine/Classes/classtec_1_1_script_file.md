---
title: tec::ScriptFile

---

# tec::ScriptFile



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ScriptFile](Classes/classtec_1_1_script_file.md#function-scriptfile)**() =default |
| bool | **[Load](Classes/classtec_1_1_script_file.md#function-load)**(const [FilePath](Classes/classtec_1_1_file_path.md) & _filename)<br>Returns a resource with the specified name.  |
| | **[ScriptFile](Classes/classtec_1_1_script_file.md#function-scriptfile)**(const [ScriptFile](Classes/classtec_1_1_script_file.md) & ) |
| [ScriptFile](Classes/classtec_1_1_script_file.md) & | **[operator=](Classes/classtec_1_1_script_file.md#function-operator=)**(const [ScriptFile](Classes/classtec_1_1_script_file.md) & ) |
| | **[ScriptFile](Classes/classtec_1_1_script_file.md#function-scriptfile)**([ScriptFile](Classes/classtec_1_1_script_file.md) && other) |
| [ScriptFile](Classes/classtec_1_1_script_file.md) & | **[operator=](Classes/classtec_1_1_script_file.md#function-operator=)**([ScriptFile](Classes/classtec_1_1_script_file.md) && other) |
| const std::string & | **[GetScript](Classes/classtec_1_1_script_file.md#function-getscript)**() const<br>Returns a reference to the script text for reading.  |
| std::string & | **[GetScript](Classes/classtec_1_1_script_file.md#function-getscript)**()<br>Returns a reference to the script text.  |
| const std::string | **[GetName](Classes/classtec_1_1_script_file.md#function-getname)**() const |
| void | **[SetName](Classes/classtec_1_1_script_file.md#function-setname)**(const std::string & _name) |
| bool | **[IsDirty](Classes/classtec_1_1_script_file.md#function-isdirty)**() const |
| void | **[Invalidate](Classes/classtec_1_1_script_file.md#function-invalidate)**()<br>Mark dirty.  |
| void | **[Validate](Classes/classtec_1_1_script_file.md#function-validate)**()<br>Mark not dirty.  |
| std::shared_ptr< [ScriptFile](Classes/classtec_1_1_script_file.md) > | **[Create](Classes/classtec_1_1_script_file.md#function-create)**(const [FilePath](Classes/classtec_1_1_file_path.md) & fname)<br>Returns a resource with the specified name.  |
| std::shared_ptr< [ScriptFile](Classes/classtec_1_1_script_file.md) > | **[Create](Classes/classtec_1_1_script_file.md#function-create)**(const std::string name, const [FilePath](Classes/classtec_1_1_file_path.md) & filename =[FilePath](Classes/classtec_1_1_file_path.md)())<br>Factory method that creates a Script and stores it in the ScriptMap under name. It will optionally load a script file with the given filename.  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::string | **[name](Classes/classtec_1_1_script_file.md#variable-name)**  |
| std::string | **[script](Classes/classtec_1_1_script_file.md#variable-script)**  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[filename](Classes/classtec_1_1_script_file.md#variable-filename)**  |
| bool | **[dirty](Classes/classtec_1_1_script_file.md#variable-dirty)**  |

## Public Functions Documentation

### function ScriptFile

```cpp
ScriptFile() =default
```


### function Load

```cpp
inline bool Load(
    const FilePath & _filename
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** [FilePath](Classes/classtec_1_1_file_path.md) filename The filename of the image file to load. 


**Return**: bool True if initialization finished with no errors. 

### function ScriptFile

```cpp
ScriptFile(
    const ScriptFile & 
)
```


### function operator=

```cpp
ScriptFile & operator=(
    const ScriptFile & 
)
```


### function ScriptFile

```cpp
inline ScriptFile(
    ScriptFile && other
)
```


### function operator=

```cpp
inline ScriptFile & operator=(
    ScriptFile && other
)
```


### function GetScript

```cpp
inline const std::string & GetScript() const
```

Returns a reference to the script text for reading. 

### function GetScript

```cpp
inline std::string & GetScript()
```

Returns a reference to the script text. 

### function GetName

```cpp
inline const std::string GetName() const
```


### function SetName

```cpp
inline void SetName(
    const std::string & _name
)
```


### function IsDirty

```cpp
inline bool IsDirty() const
```


### function Invalidate

```cpp
inline void Invalidate()
```

Mark dirty. 

### function Validate

```cpp
inline void Validate()
```

Mark not dirty. 

### function Create

```cpp
static inline std::shared_ptr< ScriptFile > Create(
    const FilePath & fname
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** std::vector<Property>& properties The creation properties for the resource. 


**Return**: std::shared_ptr<ScriptFile> The created [ScriptFile](Classes/classtec_1_1_script_file.md) resource. 

The only used initialization property is "filename". 


### function Create

```cpp
static inline std::shared_ptr< ScriptFile > Create(
    const std::string name,
    const FilePath & filename =FilePath()
)
```

Factory method that creates a Script and stores it in the ScriptMap under name. It will optionally load a script file with the given filename. 

**Parameters**: 

  * **const** std::string name The name to store the [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) under. 
  * **const** [FilePath](Classes/classtec_1_1_file_path.md) filename The optional filename of the script to load. 


**Return**: std::shared_ptr<ScriptFile> The created [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md). 

## Protected Attributes Documentation

### variable name

```cpp
std::string name;
```


### variable script

```cpp
std::string script;
```


### variable filename

```cpp
FilePath filename;
```


### variable dirty

```cpp
bool dirty {false};
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC