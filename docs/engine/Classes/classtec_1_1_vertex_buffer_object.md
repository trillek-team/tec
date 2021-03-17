---
title: tec::VertexBufferObject

---

# tec::VertexBufferObject



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[VertexBufferObject](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-vertexbufferobject)**() |
| | **[VertexBufferObject](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-vertexbufferobject)**(std::shared_ptr< [MeshFile](/engine/Classes/classtec_1_1_mesh_file/) > mesh) |
| | **[~VertexBufferObject](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-~vertexbufferobject)**() |
| void | **[Destroy](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-destroy)**()<br>Delete the underlying GL buffers.  |
| const GLuint | **[GetVAO](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-getvao)**()<br>Get the ID of the vertex array object. note: this method is not const, since GL can modify the ID.  |
| const GLuint | **[GetIBO](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-getibo)**()<br>Get the ID of the index buffer object. note: this method is not const, since GL can modify the ID.  |
| [VertexGroup](/engine/Classes/structtec_1_1_vertex_group/) * | **[GetVertexGroup](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-getvertexgroup)**(const std::size_t vertex_group_number)<br>Gets the specified [VertexGroup](/engine/Classes/structtec_1_1_vertex_group/).  |
| std::size_t | **[GetVertexGroupCount](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-getvertexgroupcount)**() const<br>Gets the number of vertex groups store in the buffer.  |
| bool | **[IsDynamic](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-isdynamic)**() const |
| void | **[Update](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-update)**() |
| void | **[Load](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-load)**(std::shared_ptr< [MeshFile](/engine/Classes/classtec_1_1_mesh_file/) > mesh) |
| void | **[Load](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-load)**(const std::vector< [VertexData](/engine/Classes/structtec_1_1_vertex_data/) > & verts, const std::vector< GLuint > & indices) |

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

Gets the specified [VertexGroup](/engine/Classes/structtec_1_1_vertex_group/). 

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

  * **std::shared_ptr<MeshFile>** mesh The mesh to load into this [VertexBufferObject](/engine/Classes/classtec_1_1_vertex_buffer_object/). 


**Return**: void 

Loads a mesh into a vertex buffer object. note: calls [Load(const std::vector<VertexData>& verts, const std::vector<GLuint>& indices)](/engine/Classes/classtec_1_1_vertex_buffer_object/#function-load). 


### function Load

```cpp
void Load(
    const std::vector< VertexData > & verts,
    const std::vector< GLuint > & indices
)
```


Loads a set of vertex and index data into a vertex buffer object. 


-------------------------------

Updated on 17 March 2021 at 23:25:12 UTC