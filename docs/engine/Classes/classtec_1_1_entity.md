---
title: tec::Entity

---

# tec::Entity





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Entity](/engine/Classes/classtec_1_1_entity/#function-entity)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) id) |
| template <typename T ,typename... U\> <br>T * | **[Add](/engine/Classes/classtec_1_1_entity/#function-add)**(U &&... args) |
| template <typename... T\> <br>std::tuple< T *... > | **[Add](/engine/Classes/classtec_1_1_entity/#function-add)**() |
| template <typename... T\> <br>std::tuple< T *... > | **[Add](/engine/Classes/classtec_1_1_entity/#function-add)**(T... args) |
| template <typename... T\> <br>void | **[Out](/engine/Classes/classtec_1_1_entity/#function-out)**(proto::Entity & entity) |
| template <typename T \> <br>void | **[Add](/engine/Classes/classtec_1_1_entity/#function-add)**(T * comp) |
| template <typename T \> <br>void | **[Remove](/engine/Classes/classtec_1_1_entity/#function-remove)**() |
| template <typename T \> <br>bool | **[Has](/engine/Classes/classtec_1_1_entity/#function-has)**() const |
| template <typename T \> <br>const T * | **[Get](/engine/Classes/classtec_1_1_entity/#function-get)**() const |
| template <typename... T\> <br>std::tuple< const T *... > | **[GetList](/engine/Classes/classtec_1_1_entity/#function-getlist)**() |
| template <typename T \> <br>void | **[Update](/engine/Classes/classtec_1_1_entity/#function-update)**(T * val) |
| [eid](/engine/Namespaces/namespacetec/#typedef-eid) | **[GetID](/engine/Classes/classtec_1_1_entity/#function-getid)**() const |

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
inline bool Has() const
```


### function Get

```cpp
template <typename T >
inline const T * Get() const
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
inline eid GetID() const
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000