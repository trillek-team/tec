---
title: tec::RenderItem

---

# tec::RenderItem





## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[model_position](/engine/Classes/structtec_1_1_render_item/#variable-model_position)**  |
| glm::vec3 | **[model_scale](/engine/Classes/structtec_1_1_render_item/#variable-model_scale)**  |
| glm::quat | **[model_quat](/engine/Classes/structtec_1_1_render_item/#variable-model_quat)**  |
| std::vector< [VertexGroup](/engine/Classes/structtec_1_1_vertex_group/) > | **[vertex_groups](/engine/Classes/structtec_1_1_render_item/#variable-vertex_groups)**  |
| std::shared_ptr< [VertexBufferObject](/engine/Classes/classtec_1_1_vertex_buffer_object/) > | **[vbo](/engine/Classes/structtec_1_1_render_item/#variable-vbo)**  |
| [MeshFile](/engine/Classes/classtec_1_1_mesh_file/) * | **[mesh_at_set](/engine/Classes/structtec_1_1_render_item/#variable-mesh_at_set)**  |
| bool | **[animated](/engine/Classes/structtec_1_1_render_item/#variable-animated)**  |
| [Animation](/engine/Classes/classtec_1_1_animation/) * | **[animation](/engine/Classes/structtec_1_1_render_item/#variable-animation)**  |

## Public Attributes Documentation

### variable model_position

```cpp
glm::vec3 model_position;
```


### variable model_scale

```cpp
glm::vec3 model_scale;
```


### variable model_quat

```cpp
glm::quat model_quat;
```


### variable vertex_groups

```cpp
std::vector< VertexGroup > vertex_groups;
```


### variable vbo

```cpp
std::shared_ptr< VertexBufferObject > vbo;
```


### variable mesh_at_set

```cpp
MeshFile * mesh_at_set {nullptr};
```


### variable animated

```cpp
bool animated {false};
```


### variable animation

```cpp
Animation * animation {nullptr};
```


-------------------------------

Updated on  6 September 2021 at 18:30:10 UTC