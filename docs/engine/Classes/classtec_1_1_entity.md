---
title: tec::Entity

---

# tec::Entity



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Entity](Classes/classtec_1_1_entity.md#function-entity)**([eid](Namespaces/namespacetec.md#typedef-eid) id) |
| template <typename T ,typename... U\> <br>T * | **[Add](Classes/classtec_1_1_entity.md#function-add)**(U &&... args) |
| template <typename... T\> <br>std::tuple< T *... > | **[Add](Classes/classtec_1_1_entity.md#function-add)**() |
| template <typename... T\> <br>std::tuple< T *... > | **[Add](Classes/classtec_1_1_entity.md#function-add)**(T... args) |
| template <typename... T\> <br>void | **[Out](Classes/classtec_1_1_entity.md#function-out)**(proto::Entity & entity) |
| template <typename T \> <br>void | **[Add](Classes/classtec_1_1_entity.md#function-add)**(T * comp) |
| template <typename T \> <br>void | **[Remove](Classes/classtec_1_1_entity.md#function-remove)**() |
| template <typename T \> <br>bool | **[Has](Classes/classtec_1_1_entity.md#function-has)**() |
| template <typename T \> <br>const T * | **[Get](Classes/classtec_1_1_entity.md#function-get)**() |
| template <typename... T\> <br>std::tuple< const T *... > | **[GetList](Classes/classtec_1_1_entity.md#function-getlist)**() |
| template <typename T \> <br>void | **[Update](Classes/classtec_1_1_entity.md#function-update)**(T * val) |
| [eid](Namespaces/namespacetec.md#typedef-eid) | **[GetID](Classes/classtec_1_1_entity.md#function-getid)**() |

## Public Functions Documentation

### function Entity

```cpp
inline Entity(
    eid id
)
```


### function Add

```cpp
template <typename T ,
typename... U>
inline T * Add(
    U &&... args
)
```


### function Add

```cpp
template <typename... T>
inline std::tuple< T *... > Add()
```


### function Add

```cpp
template <typename... T>
inline std::tuple< T *... > Add(
    T... args
)
```


### function Out

```cpp
template <typename... T>
inline void Out(
    proto::Entity & entity
)
```


### function Add

```cpp
template <typename T >
inline void Add(
    T * comp
)
```


### function Remove

```cpp
template <typename T >
inline void Remove()
```


### function Has

```cpp
template <typename T >
inline bool Has()
```


### function Get

```cpp
template <typename T >
inline const T * Get()
```


### function GetList

```cpp
template <typename... T>
inline std::tuple< const T *... > GetList()
```


### function Update

```cpp
template <typename T >
inline void Update(
    T * val
)
```


### function GetID

```cpp
inline eid GetID()
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC