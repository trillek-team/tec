---
title: tec::OBJ

---

# tec::OBJ



Inherits from [tec::MeshFile](Classes/classtec_1_1_mesh_file.md)

## Public Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[Face](Classes/structtec_1_1_o_b_j_1_1_face.md)**  |
| struct | **[MTL](Classes/structtec_1_1_o_b_j_1_1_m_t_l.md)**  |
| struct | **[OBJGroup](Classes/structtec_1_1_o_b_j_1_1_o_b_j_group.md)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [OBJ](Classes/classtec_1_1_o_b_j.md) > | **[Create](Classes/classtec_1_1_o_b_j.md#function-create)**(const [FilePath](Classes/classtec_1_1_file_path.md) & fname)<br>Returns a resource with the specified name.  |
| bool | **[Parse](Classes/classtec_1_1_o_b_j.md#function-parse)**()<br>Loads the [OBJ](Classes/classtec_1_1_o_b_j.md) file from disk and parses it.  |
| bool | **[ParseMTL](Classes/classtec_1_1_o_b_j.md#function-parsemtl)**(const [FilePath](Classes/classtec_1_1_file_path.md) & fname)<br>Loads the [MTL]() file from disk and parses it.  |
| void | **[PopulateMeshGroups](Classes/classtec_1_1_o_b_j.md#function-populatemeshgroups)**()<br>Calculates the final vertex positions based on the bind-pose skeleton.  |
| void | **[SetFileName](Classes/classtec_1_1_o_b_j.md#function-setfilename)**(const [FilePath](Classes/classtec_1_1_file_path.md) & fname)<br>Sets the mesh filename.  |

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
static std::shared_ptr< OBJ > Create(
    const FilePath & fname
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** [FilePath](Classes/classtec_1_1_file_path.md)& fname The filename of the [OBJ](Classes/classtec_1_1_o_b_j.md) resource 


**Return**: std::shared_ptr<OBJ> The created [OBJ](Classes/classtec_1_1_o_b_j.md) resource. 

The only used initialization property is "filename". 


### function Parse

```cpp
bool Parse()
```

Loads the [OBJ](Classes/classtec_1_1_o_b_j.md) file from disk and parses it. 

**Return**: bool If the mesh was valid and loaded correctly. 

### function ParseMTL

```cpp
bool ParseMTL(
    const FilePath & fname
)
```

Loads the [MTL]() file from disk and parses it. 

**Return**: bool If the material was valid and loaded correctly. 

### function PopulateMeshGroups

```cpp
void PopulateMeshGroups()
```

Calculates the final vertex positions based on the bind-pose skeleton. 

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


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC