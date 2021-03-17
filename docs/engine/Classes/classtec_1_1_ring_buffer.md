---
title: tec::RingBuffer

---

# tec::RingBuffer



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| const T & | **[operator[]](Classes/classtec_1_1_ring_buffer.md#function-operator[])**(std::size_t pos) const |
| T & | **[back](Classes/classtec_1_1_ring_buffer.md#function-back)**() |
| const T & | **[back](Classes/classtec_1_1_ring_buffer.md#function-back)**() const |
| void | **[pop_back](Classes/classtec_1_1_ring_buffer.md#function-pop_back)**() |
| void | **[push_front](Classes/classtec_1_1_ring_buffer.md#function-push_front)**(const T & value) |
| bool | **[empty](Classes/classtec_1_1_ring_buffer.md#function-empty)**() const |
| bool | **[full](Classes/classtec_1_1_ring_buffer.md#function-full)**() const |
| std::size_t | **[size](Classes/classtec_1_1_ring_buffer.md#function-size)**() const |
| std::size_t | **[max_size](Classes/classtec_1_1_ring_buffer.md#function-max_size)**() const |
| void | **[clear](Classes/classtec_1_1_ring_buffer.md#function-clear)**() |

## Detailed Description

```cpp
template <class T ,
std::size_t N>
class tec::RingBuffer;
```

## Public Functions Documentation

### function operator[]

```cpp
inline const T & operator[](
    std::size_t pos
) const
```


Return element pos of the buffer. Not bounds checks 


### function back

```cpp
inline T & back()
```


Returns the last element on the container (ie, the most older) 


### function back

```cpp
inline const T & back() const
```


Returns the last element on the container (ie, the most older) 


### function pop_back

```cpp
inline void pop_back()
```


Remove the last element on the ring buffer 


### function push_front

```cpp
inline void push_front(
    const T & value
)
```


Prepends the given element value to the beginning of the buffer 


### function empty

```cpp
inline bool empty() const
```


Returns true if there is not any element stored 


### function full

```cpp
inline bool full() const
```


Returns true if there is not any element stored 


### function size

```cpp
inline std::size_t size() const
```


Returns the number of items stored 


### function max_size

```cpp
inline std::size_t max_size() const
```


Returns the max number of elements that can be stored (ie N) 


### function clear

```cpp
inline void clear()
```


Removes all elements of the buffer 


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC