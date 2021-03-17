---
title: tec::VertexData

---

# tec::VertexData



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[VertexData](Classes/structtec_1_1_vertex_data.md#function-vertexdata)**() =default |
| | **[VertexData](Classes/structtec_1_1_vertex_data.md#function-vertexdata)**(double x, double y, double z, double r, double g, double b, double u =0.0, double v =0.0) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[position](Classes/structtec_1_1_vertex_data.md#variable-position)**  |
| glm::vec4 | **[color](Classes/structtec_1_1_vertex_data.md#variable-color)**  |
| glm::vec3 | **[normal](Classes/structtec_1_1_vertex_data.md#variable-normal)**  |
| glm::vec2 | **[uv](Classes/structtec_1_1_vertex_data.md#variable-uv)**  |
| glm::vec4 | **[bone_weights](Classes/structtec_1_1_vertex_data.md#variable-bone_weights)**  |
| glm::uvec4 | **[bone_indices](Classes/structtec_1_1_vertex_data.md#variable-bone_indices)**  |

## Public Functions Documentation

### function VertexData

```cpp
VertexData() =default
```


### function VertexData

```cpp
inline VertexData(
    double x,
    double y,
    double z,
    double r,
    double g,
    double b,
    double u =0.0,
    double v =0.0
)
```


## Public Attributes Documentation

### variable position

```cpp
glm::vec3 position {0.f, 0.f, 0.f};
```


### variable color

```cpp
glm::vec4 color {1.f, 1.f, 1.f, 1.f};
```


### variable normal

```cpp
glm::vec3 normal {0.f, 0.f, 0.f};
```


### variable uv

```cpp
glm::vec2 uv {0.f, 0.f};
```


### variable bone_weights

```cpp
glm::vec4 bone_weights {0.f, 0.f, 0.f, 0.f};
```


### variable bone_indices

```cpp
glm::uvec4 bone_indices {0, 0, 0, 0};
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC