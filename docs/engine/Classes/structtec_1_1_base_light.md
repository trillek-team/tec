---
title: tec::BaseLight

---

# tec::BaseLight



Inherited by [tec::DirectionalLight](/engine/Classes/structtec_1_1_directional_light/), [tec::PointLight](/engine/Classes/structtec_1_1_point_light/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[BaseLight](/engine/Classes/structtec_1_1_base_light/#function-baselight)**() =default |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[color](/engine/Classes/structtec_1_1_base_light/#variable-color)**  |
| float | **[ambient_intensity](/engine/Classes/structtec_1_1_base_light/#variable-ambient_intensity)**  |
| float | **[diffuse_intensity](/engine/Classes/structtec_1_1_base_light/#variable-diffuse_intensity)**  |

## Public Functions Documentation

### function BaseLight

```cpp
BaseLight() =default
```


## Public Attributes Documentation

### variable color

```cpp
glm::vec3 color {1.f, 1.f, 1.f};
```


### variable ambient_intensity

```cpp
float ambient_intensity {0.5f};
```


### variable diffuse_intensity

```cpp
float diffuse_intensity {0.2f};
```


-------------------------------

Updated on 17 March 2021 at 23:25:12 UTC