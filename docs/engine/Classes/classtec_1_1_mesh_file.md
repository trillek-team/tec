---
title: tec::MeshFile

---

# tec::MeshFile





Inherited by [tec::MD5Mesh](/engine/Classes/classtec_1_1_m_d5_mesh/), [tec::OBJ](/engine/Classes/classtec_1_1_o_b_j/)

## Public Functions

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

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::vector< [Mesh](/engine/Classes/structtec_1_1_mesh/) * > | **[meshes](/engine/Classes/classtec_1_1_mesh_file/#variable-meshes)**  |
| std::string | **[name](/engine/Classes/classtec_1_1_mesh_file/#variable-name)**  |
| bool | **[dirty](/engine/Classes/classtec_1_1_mesh_file/#variable-dirty)**  |

## Public Functions Documentation

### function ~MeshFile

```cpp
virtual ~MeshFile()
```


### function CreateMesh

```cpp
inline Mesh * CreateMesh()
```

Creates a new mesh and adds it to this file. 

**Return**: Mesh* The mesh that was added to this file. 

### function AddMesh

```cpp
inline void AddMesh(
    Mesh * mesh
)
```

Adds a mesh to this file. Ownership is transferred. 

**Parameters**: 

  * **Mesh*** mesh The mesh to add to this file. 


**Return**: void 

### function GetMesh

```cpp
inline Mesh * GetMesh(
    const std::size_t index
)
```

Returns a specific mesh. 

**Parameters**: 

  * **const** unsigned size_t index The index of the mesh to retrieve. 


**Return**: std::weak_ptr<MeshGroup> The requested mesh or null if the index is invalid. 

### function GetMeshCount

```cpp
inline std::size_t GetMeshCount() const
```

Returns the number of meshes in this file. 

**Return**: size_t The number of meshes in this file. 

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

## Protected Attributes Documentation

### variable meshes

```cpp
std::vector< Mesh * > meshes;
```


### variable name

```cpp
std::string name {"test"};
```


### variable dirty

```cpp
bool dirty {false};
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000