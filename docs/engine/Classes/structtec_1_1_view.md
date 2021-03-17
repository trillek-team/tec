---
title: tec::View

---

# tec::View



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[View](Classes/structtec_1_1_view.md#function-view)**(bool active =false) |
| void | **[Out](Classes/structtec_1_1_view.md#function-out)**(proto::Component * target) |
| void | **[In](Classes/structtec_1_1_view.md#function-in)**(const proto::Component & source) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::mat4 | **[view_matrix](Classes/structtec_1_1_view.md#variable-view_matrix)**  |
| bool | **[active](Classes/structtec_1_1_view.md#variable-active)**  |

## Public Functions Documentation

### function View

```cpp
inline View(
    bool active =false
)
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

### variable view_matrix

```cpp
glm::mat4 view_matrix {0};
```


### variable active

```cpp
bool active = false;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC