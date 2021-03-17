---
title: tec::Position

---

# tec::Position



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Position](Classes/structtec_1_1_position.md#function-position)**(glm::vec3 pos) |
| | **[Position](Classes/structtec_1_1_position.md#function-position)**() =default |
| void | **[Translate](Classes/structtec_1_1_position.md#function-translate)**(const glm::vec3 amount) |
| void | **[Translate](Classes/structtec_1_1_position.md#function-translate)**(const glm::vec3 amount, const glm::quat orientation) |
| void | **[Out](Classes/structtec_1_1_position.md#function-out)**(proto::Component * target) |
| void | **[Out](Classes/structtec_1_1_position.md#function-out)**(proto::Position * comp) |
| void | **[In](Classes/structtec_1_1_position.md#function-in)**(const proto::Component & source) |
| void | **[In](Classes/structtec_1_1_position.md#function-in)**(const proto::Position & comp) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[value](Classes/structtec_1_1_position.md#variable-value)**  |

## Public Functions Documentation

### function Position

```cpp
inline Position(
    glm::vec3 pos
)
```


### function Position

```cpp
Position() =default
```


### function Translate

```cpp
void Translate(
    const glm::vec3 amount
)
```


### function Translate

```cpp
void Translate(
    const glm::vec3 amount,
    const glm::quat orientation
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
    proto::Position * comp
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
    const proto::Position & comp
)
```


## Public Attributes Documentation

### variable value

```cpp
glm::vec3 value = {0.0f, 0.0f, 0.0f};
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC