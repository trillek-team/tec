---
title: tec::Mesh

---

# tec::Mesh





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[~Mesh](/engine/Classes/structtec_1_1_mesh/#function-~mesh)**() |
| void | **[resize](/engine/Classes/structtec_1_1_mesh/#function-resize)**(size_t n) |
| size_t | **[vert_count](/engine/Classes/structtec_1_1_mesh/#function-vert-count)**() const |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::vector< [vertex::Vertex](/engine/Classes/structtec_1_1vertex_1_1_vertex/) > | **[base_verts](/engine/Classes/structtec_1_1_mesh/#variable-base-verts)**  |
| std::vector< glm::vec4 > | **[vert_color](/engine/Classes/structtec_1_1_mesh/#variable-vert-color)**  |
| std::vector< [vertex::WeightData](/engine/Classes/structtec_1_1vertex_1_1_weight_data/) > | **[vert_weight](/engine/Classes/structtec_1_1_mesh/#variable-vert-weight)**  |
| bool | **[has_color](/engine/Classes/structtec_1_1_mesh/#variable-has-color)**  |
| bool | **[has_weight](/engine/Classes/structtec_1_1_mesh/#variable-has-weight)**  |
| std::vector< [ObjectGroup](/engine/Classes/structtec_1_1_object_group/) * > | **[object_groups](/engine/Classes/structtec_1_1_mesh/#variable-object-groups)**  |

## Public Functions Documentation

### function ~Mesh

```cpp
inline ~Mesh()
```


### function resize

```cpp
inline void resize(
    size_t n
)
```


### function vert_count

```cpp
inline size_t vert_count() const
```


## Public Attributes Documentation

### variable base_verts

```cpp
std::vector< vertex::Vertex > base_verts;
```


### variable vert_color

```cpp
std::vector< glm::vec4 > vert_color;
```


### variable vert_weight

```cpp
std::vector< vertex::WeightData > vert_weight;
```


### variable has_color

```cpp
bool has_color {false};
```


### variable has_weight

```cpp
bool has_weight {false};
```


### variable object_groups

```cpp
std::vector< ObjectGroup * > object_groups;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000