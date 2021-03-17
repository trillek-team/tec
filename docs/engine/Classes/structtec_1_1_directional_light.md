---
title: tec::DirectionalLight

---

# tec::DirectionalLight



Inherits from [tec::BaseLight](Classes/structtec_1_1_base_light.md)

Inherited by [tec::SpotLight](Classes/structtec_1_1_spot_light.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[DirectionalLight](Classes/structtec_1_1_directional_light.md#function-directionallight)**() =default |
| void | **[Out](Classes/structtec_1_1_directional_light.md#function-out)**(proto::Component * target) |
| void | **[In](Classes/structtec_1_1_directional_light.md#function-in)**(const proto::Component & source) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[direction](Classes/structtec_1_1_directional_light.md#variable-direction)**  |

## Additional inherited members

**Public Functions inherited from [tec::BaseLight](Classes/structtec_1_1_base_light.md)**

|                | Name           |
| -------------- | -------------- |
| | **[BaseLight](Classes/structtec_1_1_base_light.md#function-baselight)**() =default |

**Public Attributes inherited from [tec::BaseLight](Classes/structtec_1_1_base_light.md)**

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[color](Classes/structtec_1_1_base_light.md#variable-color)**  |
| float | **[ambient_intensity](Classes/structtec_1_1_base_light.md#variable-ambient_intensity)**  |
| float | **[diffuse_intensity](Classes/structtec_1_1_base_light.md#variable-diffuse_intensity)**  |


## Public Functions Documentation

### function DirectionalLight

```cpp
DirectionalLight() =default
```


### function Out

```cpp
inline void Out(
    proto::Component * target
)
```


### function In

```cpp
inline void In(
    const proto::Component & source
)
```


## Public Attributes Documentation

### variable direction

```cpp
glm::vec3 direction {0.f, 0.f, -1.f};
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC