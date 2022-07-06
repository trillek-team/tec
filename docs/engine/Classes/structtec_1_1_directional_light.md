---
title: tec::DirectionalLight

---

# tec::DirectionalLight





Inherits from [tec::BaseLight](/engine/Classes/structtec_1_1_base_light/)

Inherited by [tec::SpotLight](/engine/Classes/structtec_1_1_spot_light/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[DirectionalLight](/engine/Classes/structtec_1_1_directional_light/#function-directionallight)**() =default |
| void | **[Out](/engine/Classes/structtec_1_1_directional_light/#function-out)**(proto::Component * target) |
| void | **[In](/engine/Classes/structtec_1_1_directional_light/#function-in)**(const proto::Component & source) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[direction](/engine/Classes/structtec_1_1_directional_light/#variable-direction)**  |

## Additional inherited members

**Public Functions inherited from [tec::BaseLight](/engine/Classes/structtec_1_1_base_light/)**

|                | Name           |
| -------------- | -------------- |
| | **[BaseLight](/engine/Classes/structtec_1_1_base_light/#function-baselight)**() =default |

**Public Attributes inherited from [tec::BaseLight](/engine/Classes/structtec_1_1_base_light/)**

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[color](/engine/Classes/structtec_1_1_base_light/#variable-color)**  |
| float | **[ambient_intensity](/engine/Classes/structtec_1_1_base_light/#variable-ambient-intensity)**  |
| float | **[diffuse_intensity](/engine/Classes/structtec_1_1_base_light/#variable-diffuse-intensity)**  |


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

Updated on 2022-07-06 at 04:15:07 +0000