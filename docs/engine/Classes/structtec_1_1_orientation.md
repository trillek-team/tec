---
title: tec::Orientation

---

# tec::Orientation



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Orientation](Classes/structtec_1_1_orientation.md#function-orientation)**(glm::vec3 rot) |
| | **[Orientation](Classes/structtec_1_1_orientation.md#function-orientation)**(glm::quat q) |
| | **[Orientation](Classes/structtec_1_1_orientation.md#function-orientation)**() =default |
| void | **[Rotate](Classes/structtec_1_1_orientation.md#function-rotate)**(const glm::vec3 amount =glm::vec3(0.0f)) |
| void | **[OrientedRotate](Classes/structtec_1_1_orientation.md#function-orientedrotate)**(const glm::vec3 amount) |
| void | **[Out](Classes/structtec_1_1_orientation.md#function-out)**(proto::Component * target) |
| void | **[Out](Classes/structtec_1_1_orientation.md#function-out)**(proto::Orientation * comp) |
| void | **[In](Classes/structtec_1_1_orientation.md#function-in)**(const proto::Component & source) |
| void | **[In](Classes/structtec_1_1_orientation.md#function-in)**(const proto::Orientation & comp) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::quat | **[value](Classes/structtec_1_1_orientation.md#variable-value)**  |
| glm::vec3 | **[rotation](Classes/structtec_1_1_orientation.md#variable-rotation)**  |

## Public Functions Documentation

### function Orientation

```cpp
inline Orientation(
    glm::vec3 rot
)
```


### function Orientation

```cpp
inline Orientation(
    glm::quat q
)
```


### function Orientation

```cpp
Orientation() =default
```


### function Rotate

```cpp
void Rotate(
    const glm::vec3 amount =glm::vec3(0.0f)
)
```


### function OrientedRotate

```cpp
void OrientedRotate(
    const glm::vec3 amount
)
```


### function Out

```cpp
void Out(
    proto::Component * target
)
```


### function Out

```cpp
void Out(
    proto::Orientation * comp
)
```


### function In

```cpp
void In(
    const proto::Component & source
)
```


### function In

```cpp
void In(
    const proto::Orientation & comp
)
```


## Public Attributes Documentation

### variable value

```cpp
glm::quat value {1.f, 0.f, 0.f, 0.f};
```


### variable rotation

```cpp
glm::vec3 rotation {0.f, 0.f, 0.f};
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC