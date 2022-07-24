---
title: tec::Queue

---

# tec::Queue



 [More...](#detailed-description)


`#include <event-queue.hpp>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Queue](/engine/Classes/structtec_1_1_queue/#function-queue)**() |
| void | **[emplace](/engine/Classes/structtec_1_1_queue/#function-emplace)**(T && e) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::queue< T > | **[queue](/engine/Classes/structtec_1_1_queue/#variable-queue)**  |
| std::mutex | **[queue_write](/engine/Classes/structtec_1_1_queue/#variable-queue-write)**  |

## Detailed Description

```cpp
template <class T >
struct tec::Queue;
```

## Public Functions Documentation

### function Queue

```cpp
inline Queue()
```


### function emplace

```cpp
inline void emplace(
    T && e
)
```


## Public Attributes Documentation

### variable queue

```cpp
std::queue< T > queue;
```


### variable queue_write

```cpp
std::mutex queue_write;
```


-------------------------------

Updated on 2022-07-24 at 19:27:35 +0000