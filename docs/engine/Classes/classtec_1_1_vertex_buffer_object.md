---
title: tec::VertexBufferObject

---

# tec::VertexBufferObject





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[VertexBufferObject](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-vertexbufferobject)**(vertex::FormatCode _load_format) |
| | **[~VertexBufferObject](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-~vertexbufferobject)**() |
| void | **[Destroy](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-destroy)**()<br>Delete the underlying GL buffers.  |
| GLuint | **[GetVAO](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-getvao)**()<br>Get the ID of the vertex array object. note: this method is not const, since GL can modify the ID.  |
| [VertexGroup](/engine/Classes/structtec_1_1_vertex_group/) * | **[GetVertexGroup](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-getvertexgroup)**(const std::size_t vertex_group_number)<br>Gets the specified [VertexGroup]().  |
| std::size_t | **[GetVertexGroupCount](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-getvertexgroupcount)**() const<br>Gets the number of vertex groups store in the buffer.  |
| bool | **[IsDynamic](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-isdynamic)**() const |
| bool | **[Update](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-update)**() |
| bool | **[Load](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-load)**(std::shared_ptr< [MeshFile](/engine/Classes/classtec_1_1_mesh_file/) > mesh) |

## Public Functions Documentation

### function VertexBufferObject

```cpp
VertexBufferObject(
    vertex::FormatCode _load_format
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
inline GLuint GetVAO()
```

Get the ID of the vertex array object. note: this method is not const, since GL can modify the ID. 

**Return**: GLuint the GL texture ID. 

### function GetVertexGroup

```cpp
VertexGroup * GetVertexGroup(
    const std::size_t vertex_group_number
)
```

Gets the specified [VertexGroup](). 

**Parameters**: 

  * **size_t** vertex_group_number Number of the vertex group to get. 


**Return**: [VertexGroup](/engine/Classes/structtec_1_1_vertex_group/)& The specified [VertexGroup](/engine/Classes/structtec_1_1_vertex_group/). 

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
bool Update()
```


Called to update dynamic vertex buffer object. 


### function Load

```cpp
bool Load(
    std::shared_ptr< MeshFile > mesh
)
```


**Parameters**: 

  * **std::shared_ptr<MeshFile>** mesh The mesh to load into this [VertexBufferObject](/engine/Classes/classtec_1_1_vertex_buffer_object/). 


**Return**: true if the load was successful 

Loads a mesh into a vertex buffer object. note: calls Load(const std::vector<VertexData>& verts, const std::vector<GLuint>& indices). 


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000