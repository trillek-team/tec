---
title: tec::SpotLight

---

# tec::SpotLight





Inherits from [tec::DirectionalLight](/engine/Classes/structtec_1_1_directional_light/), [tec::BaseLight](/engine/Classes/structtec_1_1_base_light/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[SpotLight](/engine/Classes/structtec_1_1_spot_light/#function-spotlight)**() =default |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| float | **[cutoff](/engine/Classes/structtec_1_1_spot_light/#variable-cutoff)**  |

## Additional inherited members

**Public Functions inherited from [tec::DirectionalLight](/engine/Classes/structtec_1_1_directional_light/)**

|                | Name           |
| -------------- | -------------- |
| | **[DirectionalLight](/engine/Classes/structtec_1_1_directional_light/#function-directionallight)**() =default |
| void | **[Out](/engine/Classes/structtec_1_1_directional_light/#function-out)**(proto::Component * target) |
| void | **[In](/engine/Classes/structtec_1_1_directional_light/#function-in)**(const proto::Component & source) |

**Public Attributes inherited from [tec::DirectionalLight](/engine/Classes/structtec_1_1_directional_light/)**

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[direction](/engine/Classes/structtec_1_1_directional_light/#variable-direction)**  |

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

Updated on 2022-07-06 at 04:15:07 +0000