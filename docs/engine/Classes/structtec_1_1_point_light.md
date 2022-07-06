---
title: tec::PointLight

---

# tec::PointLight





Inherits from [tec::BaseLight](/engine/Classes/structtec_1_1_base_light/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[PointLight](/engine/Classes/structtec_1_1_point_light/#function-pointlight)**() =default |
| void | **[Out](/engine/Classes/structtec_1_1_point_light/#function-out)**(proto::Component * target) |
| void | **[In](/engine/Classes/structtec_1_1_point_light/#function-in)**(const proto::Component & source) |
| float | **[UpdateBoundingRadius](/engine/Classes/structtec_1_1_point_light/#function-updateboundingradius)**() |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| float | **[constant](/engine/Classes/structtec_1_1_point_light/#variable-constant)**  |
| float | **[linear](/engine/Classes/structtec_1_1_point_light/#variable-linear)**  |
| float | **[exponential](/engine/Classes/structtec_1_1_point_light/#variable-exponential)**  |
| struct tec::PointLight::@0 | **[Attenuation](/engine/Classes/structtec_1_1_point_light/#variable-attenuation)**  |
| float | **[bounding_radius](/engine/Classes/structtec_1_1_point_light/#variable-bounding-radius)**  |

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

### function PointLight

```cpp
PointLight() =default
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


### function UpdateBoundingRadius

```cpp
inline float UpdateBoundingRadius()
```


## Public Attributes Documentation

### variable constant

```cpp
float constant {0.f};
```


### variable linear

```cpp
float linear {0.f};
```


### variable exponential

```cpp
float exponential {0.3f};
```


### variable Attenuation

```cpp
struct tec::PointLight::@0 Attenuation;
```


### variable bounding_radius

```cpp
float bounding_radius {0.f};
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000