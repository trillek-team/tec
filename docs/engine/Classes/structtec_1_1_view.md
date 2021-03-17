---
title: tec::View

---

# tec::View



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[View](/engine/Classes/structtec_1_1_view/#function-view)**(bool active =false) |
| void | **[Out](/engine/Classes/structtec_1_1_view/#function-out)**(proto::Component * target) |
| void | **[In](/engine/Classes/structtec_1_1_view/#function-in)**(const proto::Component & source) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| glm::mat4 | **[view_matrix](/engine/Classes/structtec_1_1_view/#variable-view_matrix)**  |
| bool | **[active](/engine/Classes/structtec_1_1_view/#variable-active)**  |

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

Updated on 17 March 2021 at 22:09:29 UTC