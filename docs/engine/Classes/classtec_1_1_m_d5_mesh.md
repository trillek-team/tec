---
title: tec::MD5Mesh

---

# tec::MD5Mesh





Inherits from [tec::MeshFile](/engine/Classes/classtec_1_1_mesh_file/)

## Public Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[InternalMesh](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_internal_mesh/)**  |
| struct | **[Joint](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_joint/)**  |
| struct | **[Triangle](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_triangle/)**  |
| struct | **[Vertex](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_vertex/)**  |
| struct | **[Weight](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_weight/)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [MD5Mesh](/engine/Classes/classtec_1_1_m_d5_mesh/) > | **[Create](/engine/Classes/classtec_1_1_m_d5_mesh/#function-create)**(const [Path](/engine/Classes/classtec_1_1_path/) & fname)<br>Returns a resource with the specified name.  |
| bool | **[Parse](/engine/Classes/classtec_1_1_m_d5_mesh/#function-parse)**()<br>Loads the [MD5Mesh](/engine/Classes/classtec_1_1_m_d5_mesh/) file from disk and parses it.  |
| void | **[CalculateVertexPositions](/engine/Classes/classtec_1_1_m_d5_mesh/#function-calculatevertexpositions)**()<br>Calculates the final vertex positions based on the bind-pose skeleton.  |
| void | **[CalculateVertexNormals](/engine/Classes/classtec_1_1_m_d5_mesh/#function-calculatevertexnormals)**()<br>Calculates the vertex normals based on the bind-pose skeleton and mesh tris.  |
| void | **[UpdateIndexList](/engine/Classes/classtec_1_1_m_d5_mesh/#function-updateindexlist)**()<br>Updates the meshgroups index list based from the loaded mesh groups.  |
| void | **[SetFileName](/engine/Classes/classtec_1_1_m_d5_mesh/#function-setfilename)**(const [Path](/engine/Classes/classtec_1_1_path/) & fname)<br>Sets the mesh filename.  |

## Friends

|                | Name           |
| -------------- | -------------- |
| class | **[MD5Anim](/engine/Classes/classtec_1_1_m_d5_mesh/#friend-md5anim)**  |

## Additional inherited members

**Public Functions inherited from [tec::MeshFile](/engine/Classes/classtec_1_1_mesh_file/)**

|                | Name           |
| -------------- | -------------- |
| virtual | **[~MeshFile](/engine/Classes/classtec_1_1_mesh_file/#function-~meshfile)**() |
| [Mesh](/engine/Classes/structtec_1_1_mesh/) * | **[CreateMesh](/engine/Classes/classtec_1_1_mesh_file/#function-createmesh)**()<br>Creates a new mesh and adds it to this file.  |
| void | **[AddMesh](/engine/Classes/classtec_1_1_mesh_file/#function-addmesh)**([Mesh](/engine/Classes/structtec_1_1_mesh/) * mesh)<br>Adds a mesh to this file. Ownership is transferred.  |
| [Mesh](/engine/Classes/structtec_1_1_mesh/) * | **[GetMesh](/engine/Classes/classtec_1_1_mesh_file/#function-getmesh)**(const std::size_t index)<br>Returns a specific mesh.  |
| std::size_t | **[GetMeshCount](/engine/Classes/classtec_1_1_mesh_file/#function-getmeshcount)**() const<br>Returns the number of meshes in this file.  |
| const std::string | **[GetName](/engine/Classes/classtec_1_1_mesh_file/#function-getname)**() const |
| void | **[SetName](/engine/Classes/classtec_1_1_mesh_file/#function-setname)**(const std::string & _name) |
| bool | **[IsDirty](/engine/Classes/classtec_1_1_mesh_file/#function-isdirty)**() const |
| void | **[Invalidate](/engine/Classes/classtec_1_1_mesh_file/#function-invalidate)**()<br>Mark dirty.  |
| void | **[Validate](/engine/Classes/classtec_1_1_mesh_file/#function-validate)**()<br>Mark not dirty.  |

**Protected Attributes inherited from [tec::MeshFile](/engine/Classes/classtec_1_1_mesh_file/)**

|                | Name           |
| -------------- | -------------- |
| std::vector< [Mesh](/engine/Classes/structtec_1_1_mesh/) * > | **[meshes](/engine/Classes/classtec_1_1_mesh_file/#variable-meshes)**  |
| std::string | **[name](/engine/Classes/classtec_1_1_mesh_file/#variable-name)**  |
| bool | **[dirty](/engine/Classes/classtec_1_1_mesh_file/#variable-dirty)**  |


## Public Functions Documentation

### function Create

```cpp
static std::shared_ptr< MD5Mesh > Create(
    const Path & fname
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** [Path](/engine/Classes/classtec_1_1_path/)& fname The filename of the [MD5Mesh](/engine/Classes/classtec_1_1_m_d5_mesh/) resource 


**Return**: std::shared_ptr<MD5Mesh> The created [MD5Mesh](/engine/Classes/classtec_1_1_m_d5_mesh/) resource. 

The only used initialization property is "filename". 


### function Parse

```cpp
bool Parse()
```

Loads the [MD5Mesh](/engine/Classes/classtec_1_1_m_d5_mesh/) file from disk and parses it. 

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
    const Path & fname
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

Updated on 2022-07-06 at 04:15:07 +0000