---
title: tec::Command

---

# tec::Command



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Command](Classes/structtec_1_1_command.md#function-command)**(std::function< void(T *)> && command) |
| | **[Command](Classes/structtec_1_1_command.md#function-command)**([Command](Classes/structtec_1_1_command.md) && c) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::function< void(T *)> | **[command](Classes/structtec_1_1_command.md#variable-command)**  |

## Detailed Description

```cpp
template <class T >
struct tec::Command;
```

## Public Functions Documentation

### function Command

```cpp
inline Command(
    std::function< void(T *)> && command
)
```


### function Command

```cpp
inline Command(
    Command && c
)
```


## Public Attributes Documentation

### variable command

```cpp
std::function< void(T *)> command;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC