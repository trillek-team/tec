---
title: tec::Velocity

---

# tec::Velocity



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Velocity](Classes/structtec_1_1_velocity.md#function-velocity)**() |
| | **[Velocity](Classes/structtec_1_1_velocity.md#function-velocity)**(glm::vec3 linear, glm::vec3 angular) |
| btVector3 | **[GetLinear](Classes/structtec_1_1_velocity.md#function-getlinear)**() const |
| btVector3 | **[GetAngular](Classes/structtec_1_1_velocity.md#function-getangular)**() const |
| void | **[Out](Classes/structtec_1_1_velocity.md#function-out)**(proto::Component * target) |
| void | **[In](Classes/structtec_1_1_velocity.md#function-in)**(const proto::Component & source) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[linear](Classes/structtec_1_1_velocity.md#variable-linear)**  |
| glm::vec3 | **[angular](Classes/structtec_1_1_velocity.md#variable-angular)**  |

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

Updated on 17 March 2021 at 23:08:11 UTC