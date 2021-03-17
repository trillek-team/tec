---
title: tec::MD5Mesh

---

# tec::MD5Mesh



Inherits from [tec::MeshFile](Classes/classtec_1_1_mesh_file.md)

## Public Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[InternalMesh](Classes/structtec_1_1_m_d5_mesh_1_1_internal_mesh.md)**  |
| struct | **[Joint](Classes/structtec_1_1_m_d5_mesh_1_1_joint.md)**  |
| struct | **[Triangle](Classes/structtec_1_1_m_d5_mesh_1_1_triangle.md)**  |
| struct | **[Vertex](Classes/structtec_1_1_m_d5_mesh_1_1_vertex.md)**  |
| struct | **[Weight](Classes/structtec_1_1_m_d5_mesh_1_1_weight.md)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [MD5Mesh](Classes/classtec_1_1_m_d5_mesh.md) > | **[Create](Classes/classtec_1_1_m_d5_mesh.md#function-create)**(const [FilePath](Classes/classtec_1_1_file_path.md) & fname)<br>Returns a resource with the specified name.  |
| bool | **[Parse](Classes/classtec_1_1_m_d5_mesh.md#function-parse)**()<br>Loads the [MD5Mesh](Classes/classtec_1_1_m_d5_mesh.md) file from disk and parses it.  |
| void | **[CalculateVertexPositions](Classes/classtec_1_1_m_d5_mesh.md#function-calculatevertexpositions)**()<br>Calculates the final vertex positions based on the bind-pose skeleton.  |
| void | **[CalculateVertexNormals](Classes/classtec_1_1_m_d5_mesh.md#function-calculatevertexnormals)**()<br>Calculates the vertex normals based on the bind-pose skeleton and mesh tris.  |
| void | **[UpdateIndexList](Classes/classtec_1_1_m_d5_mesh.md#function-updateindexlist)**()<br>Updates the meshgroups index list based from the loaded mesh groups.  |
| void | **[SetFileName](Classes/classtec_1_1_m_d5_mesh.md#function-setfilename)**(const [FilePath](Classes/classtec_1_1_file_path.md) & fname)<br>Sets the mesh filename.  |

## Friends

|                | Name           |
| -------------- | -------------- |
| class | **[MD5Anim](Classes/classtec_1_1_m_d5_mesh.md#friend-md5anim)**  |

## Additional inherited members

**Public Functions inherited from [tec::MeshFile](Classes/classtec_1_1_mesh_file.md)**

|                | Name           |
| -------------- | -------------- |
| virtual | **[~MeshFile](Classes/classtec_1_1_mesh_file.md#function-~meshfile)**() |
| [Mesh](Classes/structtec_1_1_mesh.md) * | **[CreateMesh](Classes/classtec_1_1_mesh_file.md#function-createmesh)**()<br>Creates a new mesh and adds it to this file.  |
| void | **[AddMesh](Classes/classtec_1_1_mesh_file.md#function-addmesh)**([Mesh](Classes/structtec_1_1_mesh.md) * mesh)<br>Adds a mesh to this file. Ownership is transferred.  |
| [Mesh](Classes/structtec_1_1_mesh.md) * | **[GetMesh](Classes/classtec_1_1_mesh_file.md#function-getmesh)**(const std::size_t index)<br>Returns a specific mesh.  |
| std::size_t | **[GetMeshCount](Classes/classtec_1_1_mesh_file.md#function-getmeshcount)**() const<br>Returns the number of meshes in this file.  |
| const std::string | **[GetName](Classes/classtec_1_1_mesh_file.md#function-getname)**() const |
| void | **[SetName](Classes/classtec_1_1_mesh_file.md#function-setname)**(const std::string & _name) |
| bool | **[IsDirty](Classes/classtec_1_1_mesh_file.md#function-isdirty)**() const |
| void | **[Invalidate](Classes/classtec_1_1_mesh_file.md#function-invalidate)**()<br>Mark dirty.  |
| void | **[Validate](Classes/classtec_1_1_mesh_file.md#function-validate)**()<br>Mark not dirty.  |

**Protected Attributes inherited from [tec::MeshFile](Classes/classtec_1_1_mesh_file.md)**

|                | Name           |
| -------------- | -------------- |
| std::vector< [Mesh](Classes/structtec_1_1_mesh.md) * > | **[meshes](Classes/classtec_1_1_mesh_file.md#variable-meshes)**  |
| std::string | **[name](Classes/classtec_1_1_mesh_file.md#variable-name)**  |
| bool | **[dirty](Classes/classtec_1_1_mesh_file.md#variable-dirty)**  |


## Public Functions Documentation

### function Create

```cpp
static std::shared_ptr< MD5Mesh > Create(
    const FilePath & fname
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** [FilePath](Classes/classtec_1_1_file_path.md)& fname The filename of the [MD5Mesh](Classes/classtec_1_1_m_d5_mesh.md) resource 


**Return**: std::shared_ptr<MD5Mesh> The created [MD5Mesh](Classes/classtec_1_1_m_d5_mesh.md) resource. 

The only used initialization property is "filename". 


### function Parse

```cpp
bool Parse()
```

Loads the [MD5Mesh](Classes/classtec_1_1_m_d5_mesh.md) file from disk and parses it. 

**Return**: bool If the mesh was valid and loaded correctly. 

### function CalculateVertexPositions

```cpp
void CalculateVertexPositions()
```

Calculates the final vertex positions based on the bind-pose skeleton. 

**Return**: void 

There isn't a return as the processing will just do nothing if the parse data was default objects. 


### function CalculateVertexNormals

```cpp
void CalculateVertexNormals()
```

Calculates the vertex normals based on the bind-pose skeleton and mesh tris. 

**Return**: void 

There isn't a return as the processing will just do nothing if the parse data was default objects. 


### function UpdateIndexList

```cpp
void UpdateIndexList()
```

Updates the meshgroups index list based from the loaded mesh groups. 

**Return**: void 

There isn't a return as the processing will just do nothing if the parse data was default objects. 


### function SetFileName

```cpp
inline void SetFileName(
    const FilePath & fname
)
```

Sets the mesh filename. 

**Parameters**: 

  * **const** std::string& fname The mesh filename. 


**Return**: bool True if initialization finished with no errors. 

This is just a shorthand function that can be called directly via script API. 


## Friends

### friend MD5Anim

```cpp
friend class MD5Anim(
    MD5Anim 
);
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC