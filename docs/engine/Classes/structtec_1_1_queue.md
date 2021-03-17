---
title: tec::Queue

---

# tec::Queue



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Queue](Classes/structtec_1_1_queue.md#function-queue)**() |
| void | **[emplace](Classes/structtec_1_1_queue.md#function-emplace)**(T && e) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::queue< T > | **[queue](Classes/structtec_1_1_queue.md#variable-queue)**  |
| std::mutex | **[queue_write](Classes/structtec_1_1_queue.md#variable-queue_write)**  |

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

Updated on 17 March 2021 at 23:08:11 UTC