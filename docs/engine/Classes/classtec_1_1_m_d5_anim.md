---
title: tec::MD5Anim

---

# tec::MD5Anim



## Public Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[BoundingBox](/engine/Classes/structtec_1_1_m_d5_anim_1_1_bounding_box/)**  |
| struct | **[Frame](/engine/Classes/structtec_1_1_m_d5_anim_1_1_frame/)**  |
| struct | **[FrameSkeleton](/engine/Classes/structtec_1_1_m_d5_anim_1_1_frame_skeleton/)**  |
| struct | **[Joint](/engine/Classes/structtec_1_1_m_d5_anim_1_1_joint/)**  |
| struct | **[SkeletonJoint](/engine/Classes/structtec_1_1_m_d5_anim_1_1_skeleton_joint/)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[MD5Anim](/engine/Classes/classtec_1_1_m_d5_anim/#function-md5anim)**() =default |
| | **[~MD5Anim](/engine/Classes/classtec_1_1_m_d5_anim/#function-~md5anim)**() =default |
| bool | **[Parse](/engine/Classes/classtec_1_1_m_d5_anim/#function-parse)**()<br>Loads the [MD5Anim](/engine/Classes/classtec_1_1_m_d5_anim/) file from disk and parses it.  |
| void | **[SetFileName](/engine/Classes/classtec_1_1_m_d5_anim/#function-setfilename)**(const [FilePath](/engine/Classes/classtec_1_1_file_path/) & fname)<br>Sets the animation filename.  |
| [FilePath](/engine/Classes/classtec_1_1_file_path/) | **[GetFileName](/engine/Classes/classtec_1_1_m_d5_anim/#function-getfilename)**() |
| const std::string | **[GetName](/engine/Classes/classtec_1_1_m_d5_anim/#function-getname)**() const |
| void | **[SetName](/engine/Classes/classtec_1_1_m_d5_anim/#function-setname)**(const std::string & _name) |
| std::size_t | **[GetFrameCount](/engine/Classes/classtec_1_1_m_d5_anim/#function-getframecount)**() const<br>Returns the number of animation frames.  |
| int | **[GetFrameRate](/engine/Classes/classtec_1_1_m_d5_anim/#function-getframerate)**() const<br>Returns the number of frame rate per second.  |
| void | **[BuildFrameSkeleton](/engine/Classes/classtec_1_1_m_d5_anim/#function-buildframeskeleton)**(std::size_t frame_index)<br>Builds or rebuilds the [FrameSkeleton]() for a given frame index.  |
| bool | **[CheckMesh](/engine/Classes/classtec_1_1_m_d5_anim/#function-checkmesh)**(std::shared_ptr< [MD5Mesh](/engine/Classes/classtec_1_1_m_d5_mesh/) > mesh_file)<br>Checks of a given mesh is valid for this animation.  |
| [FrameSkeleton](/engine/Classes/structtec_1_1_m_d5_anim_1_1_frame_skeleton/) | **[InterpolateSkeletons](/engine/Classes/classtec_1_1_m_d5_anim/#function-interpolateskeletons)**(std::size_t frame_index_start, std::size_t frame_index_end, float delta)<br>Gets the interpolated [FrameSkeleton]() between 2 frames at a given delta.  |
| std::shared_ptr< [MD5Anim](/engine/Classes/classtec_1_1_m_d5_anim/) > | **[Create](/engine/Classes/classtec_1_1_m_d5_anim/#function-create)**(const [FilePath](/engine/Classes/classtec_1_1_file_path/) & fname)<br>Returns a resource with the specified name.  |

## Public Functions Documentation

### function MD5Anim

```cpp
MD5Anim() =default
```


### function ~MD5Anim

```cpp
~MD5Anim() =default
```


### function Parse

```cpp
bool Parse()
```

Loads the [MD5Anim](/engine/Classes/classtec_1_1_m_d5_anim/) file from disk and parses it. 

**Return**: bool If the animation was valid and loaded correctly. 

### function SetFileName

```cpp
inline void SetFileName(
    const FilePath & fname
)
```

Sets the animation filename. 

**Parameters**: 

  * **const** std::string& fname The mesh filename. 


**Return**: bool True if initialization finished with no errors. 

This is just a shorthand function that can be called directly via script API. 


### function GetFileName

```cpp
inline FilePath GetFileName()
```


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


### function GetFrameCount

```cpp
inline std::size_t GetFrameCount() const
```

Returns the number of animation frames. 

**Return**: bool size_t The number of frames. 

### function GetFrameRate

```cpp
inline int GetFrameRate() const
```

Returns the number of frame rate per second. 

**Return**: int The frame rate per second. 

### function BuildFrameSkeleton

```cpp
void BuildFrameSkeleton(
    std::size_t frame_index
)
```

Builds or rebuilds the [FrameSkeleton]() for a given frame index. 

**Parameters**: 

  * **size_t** frame_index Index of the frame to build/rebuild. 


**Return**: void 

### function CheckMesh

```cpp
bool CheckMesh(
    std::shared_ptr< MD5Mesh > mesh_file
)
```

Checks of a given mesh is valid for this animation. 

**Parameters**: 

  * **std::shared_ptr<Mesh>** mesh_file The mesh to check against. 


**Return**: bool True if the mesh is compatible. 

### function InterpolateSkeletons

```cpp
FrameSkeleton InterpolateSkeletons(
    std::size_t frame_index_start,
    std::size_t frame_index_end,
    float delta
)
```

Gets the interpolated [FrameSkeleton]() between 2 frames at a given delta. 

**Parameters**: 

  * **size_t** frame_index_start The starting frame index. 
  * **size_t** frame_index_start The ending frame index. 
  * **float** delta The change in time since the last call. 


**Return**: [FrameSkeleton](/engine/Classes/structtec_1_1_m_d5_anim_1_1_frame_skeleton/) The current [FrameSkeleton](/engine/Classes/structtec_1_1_m_d5_anim_1_1_frame_skeleton/) for the given delta. 

### function Create

```cpp
static std::shared_ptr< MD5Anim > Create(
    const FilePath & fname
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** [FilePath](/engine/Classes/classtec_1_1_file_path/)& fname The filename of the [MD5Anim](/engine/Classes/classtec_1_1_m_d5_anim/) resource 


**Return**: std::shared_ptr<MD5Anim> The create [MD5Anim](/engine/Classes/classtec_1_1_m_d5_anim/) resource. 

The only used initialization property is "filename". 


-------------------------------

Updated on 21 March 2021 at 16:43:56 UTC