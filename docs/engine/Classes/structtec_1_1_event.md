---
title: tec::Event

---

# tec::Event



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Event](/engine/Classes/structtec_1_1_event/#function-event)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::shared_ptr< T > data) |
| | **[Event](/engine/Classes/structtec_1_1_event/#function-event)**([Event](/engine/Classes/structtec_1_1_event/) && other) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| [eid](/engine/Namespaces/namespacetec/#typedef-eid) | **[entity_id](/engine/Classes/structtec_1_1_event/#variable-entity_id)**  |
| std::shared_ptr< T > | **[data](/engine/Classes/structtec_1_1_event/#variable-data)**  |

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

Updated on 21 March 2021 at 16:58:09 UTC