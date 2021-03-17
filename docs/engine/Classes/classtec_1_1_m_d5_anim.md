---
title: tec::MD5Anim

---

# tec::MD5Anim



## Public Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[BoundingBox](Classes/structtec_1_1_m_d5_anim_1_1_bounding_box.md)**  |
| struct | **[Frame](Classes/structtec_1_1_m_d5_anim_1_1_frame.md)**  |
| struct | **[FrameSkeleton](Classes/structtec_1_1_m_d5_anim_1_1_frame_skeleton.md)**  |
| struct | **[Joint](Classes/structtec_1_1_m_d5_anim_1_1_joint.md)**  |
| struct | **[SkeletonJoint](Classes/structtec_1_1_m_d5_anim_1_1_skeleton_joint.md)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[MD5Anim](Classes/classtec_1_1_m_d5_anim.md#function-md5anim)**() =default |
| | **[~MD5Anim](Classes/classtec_1_1_m_d5_anim.md#function-~md5anim)**() =default |
| bool | **[Parse](Classes/classtec_1_1_m_d5_anim.md#function-parse)**()<br>Loads the [MD5Anim](Classes/classtec_1_1_m_d5_anim.md) file from disk and parses it.  |
| void | **[SetFileName](Classes/classtec_1_1_m_d5_anim.md#function-setfilename)**(const [FilePath](Classes/classtec_1_1_file_path.md) & fname)<br>Sets the animation filename.  |
| [FilePath](Classes/classtec_1_1_file_path.md) | **[GetFileName](Classes/classtec_1_1_m_d5_anim.md#function-getfilename)**() |
| const std::string | **[GetName](Classes/classtec_1_1_m_d5_anim.md#function-getname)**() const |
| void | **[SetName](Classes/classtec_1_1_m_d5_anim.md#function-setname)**(const std::string & _name) |
| std::size_t | **[GetFrameCount](Classes/classtec_1_1_m_d5_anim.md#function-getframecount)**() const<br>Returns the number of animation frames.  |
| int | **[GetFrameRate](Classes/classtec_1_1_m_d5_anim.md#function-getframerate)**() const<br>Returns the number of frame rate per second.  |
| void | **[BuildFrameSkeleton](Classes/classtec_1_1_m_d5_anim.md#function-buildframeskeleton)**(std::size_t frame_index)<br>Builds or rebuilds the [FrameSkeleton]() for a given frame index.  |
| bool | **[CheckMesh](Classes/classtec_1_1_m_d5_anim.md#function-checkmesh)**(std::shared_ptr< [MD5Mesh](Classes/classtec_1_1_m_d5_mesh.md) > mesh_file)<br>Checks of a given mesh is valid for this animation.  |
| [FrameSkeleton](Classes/structtec_1_1_m_d5_anim_1_1_frame_skeleton.md) | **[InterpolateSkeletons](Classes/classtec_1_1_m_d5_anim.md#function-interpolateskeletons)**(std::size_t frame_index_start, std::size_t frame_index_end, float delta)<br>Gets the interpolated [FrameSkeleton]() between 2 frames at a given delta.  |
| std::shared_ptr< [MD5Anim](Classes/classtec_1_1_m_d5_anim.md) > | **[Create](Classes/classtec_1_1_m_d5_anim.md#function-create)**(const [FilePath](Classes/classtec_1_1_file_path.md) & fname)<br>Returns a resource with the specified name.  |

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

Loads the [MD5Anim](Classes/classtec_1_1_m_d5_anim.md) file from disk and parses it. 

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


**Return**: [FrameSkeleton](Classes/structtec_1_1_m_d5_anim_1_1_frame_skeleton.md) The current [FrameSkeleton](Classes/structtec_1_1_m_d5_anim_1_1_frame_skeleton.md) for the given delta. 

### function Create

```cpp
static std::shared_ptr< MD5Anim > Create(
    const FilePath & fname
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** [FilePath](Classes/classtec_1_1_file_path.md)& fname The filename of the [MD5Anim](Classes/classtec_1_1_m_d5_anim.md) resource 


**Return**: std::shared_ptr<MD5Anim> The create [MD5Anim](Classes/classtec_1_1_m_d5_anim.md) resource. 

The only used initialization property is "filename". 


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC