---
title: tec::Renderable

---

# tec::Renderable



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Renderable](Classes/structtec_1_1_renderable.md#function-renderable)**(std::shared_ptr< [VertexBufferObject](Classes/classtec_1_1_vertex_buffer_object.md) > buf, std::shared_ptr< [Shader](Classes/classtec_1_1_shader.md) > shader =nullptr) |
| | **[Renderable](Classes/structtec_1_1_renderable.md#function-renderable)**() |
| void | **[Out](Classes/structtec_1_1_renderable.md#function-out)**(proto::Component * target) |
| void | **[In](Classes/structtec_1_1_renderable.md#function-in)**(const proto::Component & source) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::set< [VertexGroup](Classes/structtec_1_1_vertex_group.md) * > | **[vertex_groups](Classes/structtec_1_1_renderable.md#variable-vertex_groups)**  |
| std::shared_ptr< [VertexBufferObject](Classes/classtec_1_1_vertex_buffer_object.md) > | **[buffer](Classes/structtec_1_1_renderable.md#variable-buffer)**  |
| std::string | **[mesh_name](Classes/structtec_1_1_renderable.md#variable-mesh_name)**  |
| std::shared_ptr< [MeshFile](Classes/classtec_1_1_mesh_file.md) > | **[mesh](Classes/structtec_1_1_renderable.md#variable-mesh)**  |
| std::string | **[shader_name](Classes/structtec_1_1_renderable.md#variable-shader_name)**  |
| std::shared_ptr< [Shader](Classes/classtec_1_1_shader.md) > | **[shader](Classes/structtec_1_1_renderable.md#variable-shader)**  |
| [Position](Classes/structtec_1_1_position.md) | **[local_translation](Classes/structtec_1_1_renderable.md#variable-local_translation)**  |
| [Orientation](Classes/structtec_1_1_orientation.md) | **[local_orientation](Classes/structtec_1_1_renderable.md#variable-local_orientation)**  |
| bool | **[hidden](Classes/structtec_1_1_renderable.md#variable-hidden)**  |

## Public Functions Documentation

### function Renderable

```cpp
Renderable(
    std::shared_ptr< VertexBufferObject > buf,
    std::shared_ptr< Shader > shader =nullptr
)
```


### function Renderable

```cpp
inline Renderable()
```


### function Out

```cpp
void Out(
    proto::Component * target
)
```


### function In

```cpp
void In(
    const proto::Component & source
)
```


## Public Attributes Documentation

### variable vertex_groups

```cpp
std::set< VertexGroup * > vertex_groups;
```


### variable buffer

```cpp
std::shared_ptr< VertexBufferObject > buffer;
```


### variable mesh_name

```cpp
std::string mesh_name;
```


### variable mesh

```cpp
std::shared_ptr< MeshFile > mesh;
```


### variable shader_name

```cpp
std::string shader_name;
```


### variable shader

```cpp
std::shared_ptr< Shader > shader;
```


### variable local_translation

```cpp
Position local_translation;
```


### variable local_orientation

```cpp
Orientation local_orientation;
```


### variable hidden

```cpp
bool hidden {false};
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC