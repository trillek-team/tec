---
title: tec::RenderSystem::RenderItem

---

# tec::RenderSystem::RenderItem



## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::mat4 * | **[model_matrix](/engine/Classes/structtec_1_1_render_system_1_1_render_item/#variable-model_matrix)**  |
| std::set< [VertexGroup](/engine/Classes/structtec_1_1_vertex_group/) * > * | **[vertex_groups](/engine/Classes/structtec_1_1_render_system_1_1_render_item/#variable-vertex_groups)**  |
| GLuint | **[vao](/engine/Classes/structtec_1_1_render_system_1_1_render_item/#variable-vao)**  |
| GLuint | **[ibo](/engine/Classes/structtec_1_1_render_system_1_1_render_item/#variable-ibo)**  |
| bool | **[animated](/engine/Classes/structtec_1_1_render_system_1_1_render_item/#variable-animated)**  |
| [Animation](/engine/Classes/classtec_1_1_animation/) * | **[animation](/engine/Classes/structtec_1_1_render_system_1_1_render_item/#variable-animation)**  |

## Friends

|                | Name           |
| -------------- | -------------- |
| bool | **[operator<](/engine/Classes/structtec_1_1_render_system_1_1_render_item/#friend-operator<)**(const RenderItem & a, const RenderItem & b)  |

## Public Attributes Documentation

### variable model_matrix

```cpp
glm::mat4 * model_matrix {nullptr};
```


### variable vertex_groups

```cpp
std::set< VertexGroup * > * vertex_groups {nullptr};
```


### variable vao

```cpp
GLuint vao {0};
```


### variable ibo

```cpp
GLuint ibo {0};
```


### variable animated

```cpp
bool animated {false};
```


### variable animation

```cpp
Animation * animation {nullptr};
```


## Friends

### friend operator<

```cpp
friend bool operator<(
    const RenderItem & a,

    const RenderItem & b
);
```


-------------------------------

Updated on 21 March 2021 at 16:29:13 UTC