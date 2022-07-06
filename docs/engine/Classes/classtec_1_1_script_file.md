---
title: tec::ScriptFile

---

# tec::ScriptFile





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ScriptFile](/engine/Classes/classtec_1_1_script_file/#function-scriptfile)**() =default |
| bool | **[Load](/engine/Classes/classtec_1_1_script_file/#function-load)**(const [Path](/engine/Classes/classtec_1_1_path/) & _filename)<br>Returns a resource with the specified name.  |
| | **[ScriptFile](/engine/Classes/classtec_1_1_script_file/#function-scriptfile)**(const [ScriptFile](/engine/Classes/classtec_1_1_script_file/) & ) =delete |
| [ScriptFile](/engine/Classes/classtec_1_1_script_file/) & | **[operator=](/engine/Classes/classtec_1_1_script_file/#function-operator=)**(const [ScriptFile](/engine/Classes/classtec_1_1_script_file/) & ) =delete |
| | **[ScriptFile](/engine/Classes/classtec_1_1_script_file/#function-scriptfile)**([ScriptFile](/engine/Classes/classtec_1_1_script_file/) && other) |
| [ScriptFile](/engine/Classes/classtec_1_1_script_file/) & | **[operator=](/engine/Classes/classtec_1_1_script_file/#function-operator=)**([ScriptFile](/engine/Classes/classtec_1_1_script_file/) && other) |
| const std::string & | **[GetScript](/engine/Classes/classtec_1_1_script_file/#function-getscript)**() const<br>Returns a reference to the script text for reading.  |
| std::string & | **[GetScript](/engine/Classes/classtec_1_1_script_file/#function-getscript)**()<br>Returns a reference to the script text.  |
| const std::string | **[GetName](/engine/Classes/classtec_1_1_script_file/#function-getname)**() const |
| void | **[SetName](/engine/Classes/classtec_1_1_script_file/#function-setname)**(const std::string & _name) |
| bool | **[IsDirty](/engine/Classes/classtec_1_1_script_file/#function-isdirty)**() const |
| void | **[Invalidate](/engine/Classes/classtec_1_1_script_file/#function-invalidate)**()<br>Mark dirty.  |
| void | **[Validate](/engine/Classes/classtec_1_1_script_file/#function-validate)**()<br>Mark not dirty.  |
| std::shared_ptr< [ScriptFile](/engine/Classes/classtec_1_1_script_file/) > | **[Create](/engine/Classes/classtec_1_1_script_file/#function-create)**(const [Path](/engine/Classes/classtec_1_1_path/) & fname)<br>Returns a resource with the specified name.  |
| std::shared_ptr< [ScriptFile](/engine/Classes/classtec_1_1_script_file/) > | **[Create](/engine/Classes/classtec_1_1_script_file/#function-create)**(const std::string name, const [Path](/engine/Classes/classtec_1_1_path/) & filename =[Path](/engine/Classes/classtec_1_1_path/)())<br>Factory method that creates a Script and stores it in the ScriptMap under name. It will optionally load a script file with the given filename.  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::string | **[name](/engine/Classes/classtec_1_1_script_file/#variable-name)**  |
| std::string | **[script](/engine/Classes/classtec_1_1_script_file/#variable-script)**  |
| [Path](/engine/Classes/classtec_1_1_path/) | **[filename](/engine/Classes/classtec_1_1_script_file/#variable-filename)**  |
| bool | **[dirty](/engine/Classes/classtec_1_1_script_file/#variable-dirty)**  |

## Public Functions Documentation

### function ScriptFile

```cpp
ScriptFile() =default
```


### function Load

```cpp
bool Load(
    const Path & _filename
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** [Path](/engine/Classes/classtec_1_1_path/) filename The filename of the image file to load. 


**Return**: bool True if initialization finished with no errors. 

### function ScriptFile

```cpp
ScriptFile(
    const ScriptFile & 
) =delete
```


### function operator=

```cpp
ScriptFile & operator=(
    const ScriptFile & 
) =delete
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
static std::shared_ptr< ScriptFile > Create(
    const Path & fname
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** std::vector<Property>& properties The creation properties for the resource. 


**Return**: std::shared_ptr<ScriptFile> The created [ScriptFile](/engine/Classes/classtec_1_1_script_file/) resource. 

The only used initialization property is "filename". 


### function Create

```cpp
static inline std::shared_ptr< ScriptFile > Create(
    const std::string name,
    const Path & filename =Path()
)
```

Factory method that creates a Script and stores it in the ScriptMap under name. It will optionally load a script file with the given filename. 

**Parameters**: 

  * **const** std::string name The name to store the [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) under. 
  * **const** [Path](/engine/Classes/classtec_1_1_path/) filename The optional filename of the script to load. 


**Return**: std::shared_ptr<ScriptFile> The created [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/). 

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
Path filename;
```


### variable dirty

```cpp
bool dirty {false};
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000