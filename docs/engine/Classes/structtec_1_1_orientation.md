---
title: tec::Orientation

---

# tec::Orientation





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Orientation](/engine/Classes/structtec_1_1_orientation/#function-orientation)**(glm::vec3 rot) |
| | **[Orientation](/engine/Classes/structtec_1_1_orientation/#function-orientation)**(glm::quat q) |
| | **[Orientation](/engine/Classes/structtec_1_1_orientation/#function-orientation)**() =default |
| void | **[Rotate](/engine/Classes/structtec_1_1_orientation/#function-rotate)**(const glm::vec3 amount =glm::vec3(0.0f)) |
| void | **[OrientedRotate](/engine/Classes/structtec_1_1_orientation/#function-orientedrotate)**(const glm::vec3 amount) |
| void | **[Out](/engine/Classes/structtec_1_1_orientation/#function-out)**(proto::Component * target) const |
| void | **[Out](/engine/Classes/structtec_1_1_orientation/#function-out)**(proto::Quaternion * comp) const |
| void | **[In](/engine/Classes/structtec_1_1_orientation/#function-in)**(const proto::Component & source) |
| void | **[In](/engine/Classes/structtec_1_1_orientation/#function-in)**(const proto::Quaternion & comp) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::quat | **[value](/engine/Classes/structtec_1_1_orientation/#variable-value)**  |
| glm::vec3 | **[rotation](/engine/Classes/structtec_1_1_orientation/#variable-rotation)**  |

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
) const
```


### function Out

```cpp
void Out(
    proto::Quaternion * comp
) const
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
    const proto::Quaternion & comp
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

Updated on 2022-07-06 at 04:15:07 +0000