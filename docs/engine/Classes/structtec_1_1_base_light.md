---
title: tec::BaseLight

---

# tec::BaseLight



Inherited by [tec::DirectionalLight](Classes/structtec_1_1_directional_light.md), [tec::PointLight](Classes/structtec_1_1_point_light.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[BaseLight](Classes/structtec_1_1_base_light.md#function-baselight)**() =default |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[color](Classes/structtec_1_1_base_light.md#variable-color)**  |
| float | **[ambient_intensity](Classes/structtec_1_1_base_light.md#variable-ambient_intensity)**  |
| float | **[diffuse_intensity](Classes/structtec_1_1_base_light.md#variable-diffuse_intensity)**  |

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

Updated on 17 March 2021 at 23:08:11 UTC