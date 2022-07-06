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
| glm::vec3 | **[view_pos](/engine/Classes/structtec_1_1_view/#variable-view-pos)**  |
| glm::quat | **[view_quat](/engine/Classes/structtec_1_1_view/#variable-view-quat)**  |
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

### variable view_pos

```cpp
glm::vec3 view_pos {0};
```


### variable view_quat

```cpp
glm::quat view_quat;
```


### variable active

```cpp
bool active = false;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000