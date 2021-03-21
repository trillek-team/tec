---
title: tec::MouseMoveEvent

---

# tec::MouseMoveEvent



 [More...](#detailed-description)


`#include <events.hpp>`

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| double | **[norm_x](/engine/Classes/structtec_1_1_mouse_move_event/#variable-norm_x)**  |
| double | **[norm_y](/engine/Classes/structtec_1_1_mouse_move_event/#variable-norm_y)**  |
| int | **[old_x](/engine/Classes/structtec_1_1_mouse_move_event/#variable-old_x)** <br>Resolution independent new x, y (0-1) from upper-left to lower-right.  |
| int | **[old_y](/engine/Classes/structtec_1_1_mouse_move_event/#variable-old_y)**  |
| int | **[new_x](/engine/Classes/structtec_1_1_mouse_move_event/#variable-new_x)** <br>Client space old x, y.  |
| int | **[new_y](/engine/Classes/structtec_1_1_mouse_move_event/#variable-new_y)**  |

## Detailed Description

```cpp
struct tec::MouseMoveEvent;
```


Mouse change of position event 

## Public Attributes Documentation

### variable norm_x

```cpp
double norm_x {0.0};
```


### variable norm_y

```cpp
double norm_y {0.0};
```


### variable old_x

```cpp
int old_x {0};
```

Resolution independent new x, y (0-1) from upper-left to lower-right. 

### variable old_y

```cpp
int old_y {0};
```


### variable new_x

```cpp
int new_x {0};
```

Client space old x, y. 

### variable new_y

```cpp
int new_y {0};
```


-------------------------------

Updated on 21 March 2021 at 16:41:46 UTC