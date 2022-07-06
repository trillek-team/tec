---
title: tec::PathException

---

# tec::PathException





Inherits from std::exception

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[PathException](/engine/Classes/classtec_1_1_path_exception/#function-pathexception)**() |
| | **[PathException](/engine/Classes/classtec_1_1_path_exception/#function-pathexception)**(const char * msg) |
| | **[PathException](/engine/Classes/classtec_1_1_path_exception/#function-pathexception)**(const std::string & msg) |
| virtual const char * | **[what](/engine/Classes/classtec_1_1_path_exception/#function-what)**() const override |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::string | **[message](/engine/Classes/classtec_1_1_path_exception/#variable-message)**  |

## Public Functions Documentation

### function PathException

```cpp
inline PathException()
```


### function PathException

```cpp
inline PathException(
    const char * msg
)
```


### function PathException

```cpp
inline PathException(
    const std::string & msg
)
```


### function what

```cpp
inline virtual const char * what() const override
```


## Protected Attributes Documentation

### variable message

```cpp
std::string message;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000