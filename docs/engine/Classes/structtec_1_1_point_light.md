---
title: tec::PointLight

---

# tec::PointLight



Inherits from [tec::BaseLight](Classes/structtec_1_1_base_light.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[PointLight](Classes/structtec_1_1_point_light.md#function-pointlight)**() =default |
| void | **[Out](Classes/structtec_1_1_point_light.md#function-out)**(proto::Component * target) |
| void | **[In](Classes/structtec_1_1_point_light.md#function-in)**(const proto::Component & source) |
| float | **[UpdateBoundingRadius](Classes/structtec_1_1_point_light.md#function-updateboundingradius)**() |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| float | **[constant](Classes/structtec_1_1_point_light.md#variable-constant)**  |
| float | **[linear](Classes/structtec_1_1_point_light.md#variable-linear)**  |
| float | **[exponential](Classes/structtec_1_1_point_light.md#variable-exponential)**  |
| struct tec::PointLight::@0 | **[Attenuation](Classes/structtec_1_1_point_light.md#variable-attenuation)**  |
| float | **[bounding_radius](Classes/structtec_1_1_point_light.md#variable-bounding_radius)**  |

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

Updated on 17 March 2021 at 23:08:11 UTC