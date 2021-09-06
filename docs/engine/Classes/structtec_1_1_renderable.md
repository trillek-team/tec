---
title: tec::Renderable

---

# tec::Renderable





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Renderable](/engine/Classes/structtec_1_1_renderable/#function-renderable)**() |
| void | **[Out](/engine/Classes/structtec_1_1_renderable/#function-out)**(proto::Component * target) |
| void | **[In](/engine/Classes/structtec_1_1_renderable/#function-in)**(const proto::Component & source) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [RenderItem](/engine/Classes/structtec_1_1_render_item/) > | **[render_item](/engine/Classes/structtec_1_1_renderable/#variable-render_item)**  |
| std::string | **[mesh_name](/engine/Classes/structtec_1_1_renderable/#variable-mesh_name)**  |
| std::shared_ptr< [MeshFile](/engine/Classes/classtec_1_1_mesh_file/) > | **[mesh](/engine/Classes/structtec_1_1_renderable/#variable-mesh)**  |
| std::string | **[shader_name](/engine/Classes/structtec_1_1_renderable/#variable-shader_name)**  |
| std::shared_ptr< [Shader](/engine/Classes/classtec_1_1_shader/) > | **[shader](/engine/Classes/structtec_1_1_renderable/#variable-shader)**  |
| glm::vec3 | **[local_translation](/engine/Classes/structtec_1_1_renderable/#variable-local_translation)**  |
| [Orientation](/engine/Classes/structtec_1_1_orientation/) | **[local_orientation](/engine/Classes/structtec_1_1_renderable/#variable-local_orientation)**  |
| bool | **[hidden](/engine/Classes/structtec_1_1_renderable/#variable-hidden)**  |

## Public Functions Documentation

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

### variable render_item

```cpp
std::shared_ptr< RenderItem > render_item;
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
glm::vec3 local_translation;
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

Updated on  6 September 2021 at 18:30:10 UTC