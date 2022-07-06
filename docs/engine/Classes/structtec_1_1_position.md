---
title: tec::Position

---

# tec::Position





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Position](/engine/Classes/structtec_1_1_position/#function-position)**(glm::vec3 pos) |
| | **[Position](/engine/Classes/structtec_1_1_position/#function-position)**() =default |
| void | **[Translate](/engine/Classes/structtec_1_1_position/#function-translate)**(const glm::vec3 amount) |
| void | **[Translate](/engine/Classes/structtec_1_1_position/#function-translate)**(const glm::vec3 amount, const glm::quat orientation) |
| void | **[Out](/engine/Classes/structtec_1_1_position/#function-out)**(proto::Component * target) const |
| void | **[Out](/engine/Classes/structtec_1_1_position/#function-out)**(proto::Position * comp) const |
| void | **[In](/engine/Classes/structtec_1_1_position/#function-in)**(const proto::Component & source) |
| void | **[In](/engine/Classes/structtec_1_1_position/#function-in)**(const proto::Position & comp) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::vec3 | **[value](/engine/Classes/structtec_1_1_position/#variable-value)**  |

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
) const
```


### function Out

```cpp
void Out(
    proto::Position * comp
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
    const proto::Position & comp
)
```


## Public Attributes Documentation

### variable value

```cpp
glm::vec3 value = {0.0f, 0.0f, 0.0f};
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000