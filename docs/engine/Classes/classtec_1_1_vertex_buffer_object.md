---
title: tec::VertexBufferObject

---

# tec::VertexBufferObject



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[VertexBufferObject](Classes/classtec_1_1_vertex_buffer_object.md#function-vertexbufferobject)**() |
| | **[VertexBufferObject](Classes/classtec_1_1_vertex_buffer_object.md#function-vertexbufferobject)**(std::shared_ptr< [MeshFile](Classes/classtec_1_1_mesh_file.md) > mesh) |
| | **[~VertexBufferObject](Classes/classtec_1_1_vertex_buffer_object.md#function-~vertexbufferobject)**() |
| void | **[Destroy](Classes/classtec_1_1_vertex_buffer_object.md#function-destroy)**()<br>Delete the underlying GL buffers.  |
| const GLuint | **[GetVAO](Classes/classtec_1_1_vertex_buffer_object.md#function-getvao)**()<br>Get the ID of the vertex array object. note: this method is not const, since GL can modify the ID.  |
| const GLuint | **[GetIBO](Classes/classtec_1_1_vertex_buffer_object.md#function-getibo)**()<br>Get the ID of the index buffer object. note: this method is not const, since GL can modify the ID.  |
| [VertexGroup](Classes/structtec_1_1_vertex_group.md) * | **[GetVertexGroup](Classes/classtec_1_1_vertex_buffer_object.md#function-getvertexgroup)**(const std::size_t vertex_group_number)<br>Gets the specified [VertexGroup](Classes/structtec_1_1_vertex_group.md).  |
| std::size_t | **[GetVertexGroupCount](Classes/classtec_1_1_vertex_buffer_object.md#function-getvertexgroupcount)**() const<br>Gets the number of vertex groups store in the buffer.  |
| bool | **[IsDynamic](Classes/classtec_1_1_vertex_buffer_object.md#function-isdynamic)**() const |
| void | **[Update](Classes/classtec_1_1_vertex_buffer_object.md#function-update)**() |
| void | **[Load](Classes/classtec_1_1_vertex_buffer_object.md#function-load)**(std::shared_ptr< [MeshFile](Classes/classtec_1_1_mesh_file.md) > mesh) |
| void | **[Load](Classes/classtec_1_1_vertex_buffer_object.md#function-load)**(const std::vector< [VertexData](Classes/structtec_1_1_vertex_data.md) > & verts, const std::vector< GLuint > & indices) |

## Public Functions Documentation

### function VertexBufferObject

```cpp
VertexBufferObject()
```


### function VertexBufferObject

```cpp
VertexBufferObject(
    std::shared_ptr< MeshFile > mesh
)
```


### function ~VertexBufferObject

```cpp
~VertexBufferObject()
```


### function Destroy

```cpp
void Destroy()
```

Delete the underlying GL buffers. 

### function GetVAO

```cpp
const GLuint GetVAO()
```

Get the ID of the vertex array object. note: this method is not const, since GL can modify the ID. 

**Return**: GLuint the GL texture ID. 

### function GetIBO

```cpp
const GLuint GetIBO()
```

Get the ID of the index buffer object. note: this method is not const, since GL can modify the ID. 

**Return**: GLuint the GL texture ID. 

### function GetVertexGroup

```cpp
VertexGroup * GetVertexGroup(
    const std::size_t vertex_group_number
)
```

Gets the specified [VertexGroup](Classes/structtec_1_1_vertex_group.md). 

**Parameters**: 

  * **size_t** vertex_group_number Number of the vertex group to get. 


**Return**: [VertexGroup](Classes/structtec_1_1_vertex_group.md)& The specified [VertexGroup](Classes/structtec_1_1_vertex_group.md). 

### function GetVertexGroupCount

```cpp
std::size_t GetVertexGroupCount() const
```

Gets the number of vertex groups store in the buffer. 

**Return**: size_t The number of vertex groups. 

### function IsDynamic

```cpp
bool IsDynamic() const
```


**Return**: true If the vertex buffer object was created dynamic 

### function Update

```cpp
void Update()
```


Called to update dynamic vertex buffer object. 


### function Load

```cpp
void Load(
    std::shared_ptr< MeshFile > mesh
)
```


**Parameters**: 

  * **std::shared_ptr<MeshFile>** mesh The mesh to load into this [VertexBufferObject](Classes/classtec_1_1_vertex_buffer_object.md). 


**Return**: void 

Loads a mesh into a vertex buffer object. note: calls [Load(const std::vector<VertexData>& verts, const std::vector<GLuint>& indices)](Classes/classtec_1_1_vertex_buffer_object.md#function-load). 


### function Load

```cpp
void Load(
    const std::vector< VertexData > & verts,
    const std::vector< GLuint > & indices
)
```


Loads a set of vertex and index data into a vertex buffer object. 


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC