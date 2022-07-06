---
title: tec::RingBuffer

---

# tec::RingBuffer



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| const T & | **[operator[]](/engine/Classes/classtec_1_1_ring_buffer/#function-operator[])**(std::size_t pos) const |
| T & | **[back](/engine/Classes/classtec_1_1_ring_buffer/#function-back)**() |
| const T & | **[back](/engine/Classes/classtec_1_1_ring_buffer/#function-back)**() const |
| void | **[pop_back](/engine/Classes/classtec_1_1_ring_buffer/#function-pop-back)**() |
| void | **[push_front](/engine/Classes/classtec_1_1_ring_buffer/#function-push-front)**(const T & value) |
| bool | **[empty](/engine/Classes/classtec_1_1_ring_buffer/#function-empty)**() const |
| bool | **[full](/engine/Classes/classtec_1_1_ring_buffer/#function-full)**() const |
| std::size_t | **[size](/engine/Classes/classtec_1_1_ring_buffer/#function-size)**() const |
| std::size_t | **[max_size](/engine/Classes/classtec_1_1_ring_buffer/#function-max-size)**() const |
| void | **[clear](/engine/Classes/classtec_1_1_ring_buffer/#function-clear)**() |

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

Updated on 2022-07-06 at 04:15:07 +0000