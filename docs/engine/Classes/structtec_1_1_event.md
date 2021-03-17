---
title: tec::Event

---

# tec::Event



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Event](Classes/structtec_1_1_event.md#function-event)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id, std::shared_ptr< T > data) |
| | **[Event](Classes/structtec_1_1_event.md#function-event)**([Event](Classes/structtec_1_1_event.md) && other) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| [eid](Namespaces/namespacetec.md#typedef-eid) | **[entity_id](Classes/structtec_1_1_event.md#variable-entity_id)**  |
| std::shared_ptr< T > | **[data](Classes/structtec_1_1_event.md#variable-data)**  |

## Detailed Description

```cpp
template <class T >
struct tec::Event;
```

## Public Functions Documentation

### function Event

```cpp
inline Event(
    eid entity_id,
    std::shared_ptr< T > data
)
```


### function Event

```cpp
inline Event(
    Event && other
)
```


## Public Attributes Documentation

### variable entity_id

```cpp
eid entity_id;
```


### variable data

```cpp
std::shared_ptr< T > data;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC