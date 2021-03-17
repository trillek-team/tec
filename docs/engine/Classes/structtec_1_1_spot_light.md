---
title: tec::SpotLight

---

# tec::SpotLight



Inherits from [tec::DirectionalLight](Classes/structtec_1_1_directional_light.md), [tec::BaseLight](Classes/structtec_1_1_base_light.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[SpotLight](Classes/structtec_1_1_spot_light.md#function-spotlight)**() =default |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| float | **[cutoff](Classes/structtec_1_1_spot_light.md#variable-cutoff)**  |

## Additional inherited members

**Public Functions inherited from [tec::DirectionalLight](Classes/structtec_1_1_directional_light.md)**

|                | Name           |
| -------------- | -------------- |
| | **[DirectionalLight](Classes/structtec_1_1_directional_light.md#function-directionallight)**() =default |
| void | **[Out](Classes/structtec_1_1_directional_light.md#function-out)**(proto::Component * target) |
| void | **[In](Classes/structtec_1_1_directional_light.md#function-in)**(const proto::Component & source) |

**Public Attributes inherited from [tec::DirectionalLight](Classes/structtec_1_1_directional_light.md)**

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[direction](Classes/structtec_1_1_directional_light.md#variable-direction)**  |

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

### function SpotLight

```cpp
SpotLight() =default
```


## Public Attributes Documentation

### variable cutoff

```cpp
float cutoff {0.f};
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC