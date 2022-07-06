---
title: tec::Velocity

---

# tec::Velocity





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Velocity](/engine/Classes/structtec_1_1_velocity/#function-velocity)**() |
| | **[Velocity](/engine/Classes/structtec_1_1_velocity/#function-velocity)**(glm::vec3 linear, glm::vec3 angular) |
| btVector3 | **[GetLinear](/engine/Classes/structtec_1_1_velocity/#function-getlinear)**() const |
| btVector3 | **[GetAngular](/engine/Classes/structtec_1_1_velocity/#function-getangular)**() const |
| void | **[Out](/engine/Classes/structtec_1_1_velocity/#function-out)**(proto::Component * target) |
| void | **[In](/engine/Classes/structtec_1_1_velocity/#function-in)**(const proto::Component & source) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[linear](/engine/Classes/structtec_1_1_velocity/#variable-linear)**  |
| glm::vec3 | **[angular](/engine/Classes/structtec_1_1_velocity/#variable-angular)**  |

## Public Functions Documentation

### function Velocity

```cpp
inline Velocity()
```


### function Velocity

```cpp
inline Velocity(
    glm::vec3 linear,
    glm::vec3 angular
)
```


### function GetLinear

```cpp
inline btVector3 GetLinear() const
```


### function GetAngular

```cpp
inline btVector3 GetAngular() const
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

### variable linear

```cpp
glm::vec3 linear;
```


### variable angular

```cpp
glm::vec3 angular;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000