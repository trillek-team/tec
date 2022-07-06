---
title: tec::OBJ

---

# tec::OBJ





Inherits from [tec::MeshFile](/engine/Classes/classtec_1_1_mesh_file/)

## Public Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[Face](/engine/Classes/structtec_1_1_o_b_j_1_1_face/)**  |
| struct | **[MTL](/engine/Classes/structtec_1_1_o_b_j_1_1_m_t_l/)**  |
| struct | **[OBJGroup](/engine/Classes/structtec_1_1_o_b_j_1_1_o_b_j_group/)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [OBJ](/engine/Classes/classtec_1_1_o_b_j/) > | **[Create](/engine/Classes/classtec_1_1_o_b_j/#function-create)**(const [Path](/engine/Classes/classtec_1_1_path/) & fname)<br>Returns a resource with the specified name.  |
| bool | **[Parse](/engine/Classes/classtec_1_1_o_b_j/#function-parse)**()<br>Loads the [OBJ](/engine/Classes/classtec_1_1_o_b_j/) file from disk and parses it.  |
| bool | **[ParseMTL](/engine/Classes/classtec_1_1_o_b_j/#function-parsemtl)**(const [Path](/engine/Classes/classtec_1_1_path/) & fname)<br>Loads the [MTL]() file from disk and parses it.  |
| void | **[PopulateMeshGroups](/engine/Classes/classtec_1_1_o_b_j/#function-populatemeshgroups)**()<br>Calculates the final vertex positions based on the bind-pose skeleton.  |
| void | **[SetFileName](/engine/Classes/classtec_1_1_o_b_j/#function-setfilename)**(const [Path](/engine/Classes/classtec_1_1_path/) & fname)<br>Sets the mesh filename.  |

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
static std::shared_ptr< OBJ > Create(
    const Path & fname
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** [Path](/engine/Classes/classtec_1_1_path/)& fname The filename of the [OBJ](/engine/Classes/classtec_1_1_o_b_j/) resource 


**Return**: std::shared_ptr<OBJ> The created [OBJ](/engine/Classes/classtec_1_1_o_b_j/) resource. 

The only used initialization property is "filename". 


### function Parse

```cpp
bool Parse()
```

Loads the [OBJ](/engine/Classes/classtec_1_1_o_b_j/) file from disk and parses it. 

**Return**: bool If the mesh was valid and loaded correctly. 

### function ParseMTL

```cpp
bool ParseMTL(
    const Path & fname
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
    const Path & fname
)
```

Sets the mesh filename. 

**Parameters**: 

  * **const** std::string& fname The mesh filename. 


**Return**: bool True if initialization finished with no errors. 

This is just a shorthand function that can be called directly via script API. 


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000