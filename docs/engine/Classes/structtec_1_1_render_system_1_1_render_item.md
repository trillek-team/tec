---
title: tec::RenderSystem::RenderItem

---

# tec::RenderSystem::RenderItem



## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::mat4 * | **[model_matrix](Classes/structtec_1_1_render_system_1_1_render_item.md#variable-model_matrix)**  |
| std::set< [VertexGroup](Classes/structtec_1_1_vertex_group.md) * > * | **[vertex_groups](Classes/structtec_1_1_render_system_1_1_render_item.md#variable-vertex_groups)**  |
| GLuint | **[vao](Classes/structtec_1_1_render_system_1_1_render_item.md#variable-vao)**  |
| GLuint | **[ibo](Classes/structtec_1_1_render_system_1_1_render_item.md#variable-ibo)**  |
| bool | **[animated](Classes/structtec_1_1_render_system_1_1_render_item.md#variable-animated)**  |
| [Animation](Classes/classtec_1_1_animation.md) * | **[animation](Classes/structtec_1_1_render_system_1_1_render_item.md#variable-animation)**  |

## Friends

|                | Name           |
| -------------- | -------------- |
| bool | **[operator<](Classes/structtec_1_1_render_system_1_1_render_item.md#friend-operator<)**(const RenderItem & a, const RenderItem & b)  |

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

Updated on 17 March 2021 at 23:08:11 UTC