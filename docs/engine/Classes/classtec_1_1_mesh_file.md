---
title: tec::MeshFile

---

# tec::MeshFile



Inherited by [tec::MD5Mesh](Classes/classtec_1_1_m_d5_mesh.md), [tec::OBJ](Classes/classtec_1_1_o_b_j.md)

## Public Functions

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

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::vector< [Mesh](Classes/structtec_1_1_mesh.md) * > | **[meshes](Classes/classtec_1_1_mesh_file.md#variable-meshes)**  |
| std::string | **[name](Classes/classtec_1_1_mesh_file.md#variable-name)**  |
| bool | **[dirty](Classes/classtec_1_1_mesh_file.md#variable-dirty)**  |

## Public Functions Documentation

### function ~MeshFile

```cpp
inline virtual ~MeshFile()
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

Updated on 17 March 2021 at 23:08:11 UTC