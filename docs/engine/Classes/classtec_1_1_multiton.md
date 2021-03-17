---
title: tec::Multiton

---

# tec::Multiton



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| std::map< ID_T, T >::iterator | **[Begin](Classes/classtec_1_1_multiton.md#function-begin)**() |
| std::map< ID_T, T >::iterator | **[End](Classes/classtec_1_1_multiton.md#function-end)**() |
| std::size_t | **[Size](Classes/classtec_1_1_multiton.md#function-size)**() |
| std::vector< ID_T > & | **[Keys](Classes/classtec_1_1_multiton.md#function-keys)**() |
| T | **[Get](Classes/classtec_1_1_multiton.md#function-get)**(const ID_T id)<br>Get the instance for the given ID.  |
| bool | **[Has](Classes/classtec_1_1_multiton.md#function-has)**(const ID_T id) |
| void | **[Set](Classes/classtec_1_1_multiton.md#function-set)**(const ID_T id, T instance)<br>Set (or add/create) an instance for the given ID.  |
| void | **[Remove](Classes/classtec_1_1_multiton.md#function-remove)**(const ID_T id)<br>Remove the instance for the given ID.  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| T | **[default_value](Classes/classtec_1_1_multiton.md#variable-default_value)**  |
| std::map< ID_T, T > | **[instances](Classes/classtec_1_1_multiton.md#variable-instances)**  |

## Detailed Description

```cpp
template <typename ID_T ,
typename T >
class tec::Multiton;
```

## Public Functions Documentation

### function Begin

```cpp
static inline std::map< ID_T, T >::iterator Begin()
```


### function End

```cpp
static inline std::map< ID_T, T >::iterator End()
```


### function Size

```cpp
static inline std::size_t Size()
```


### function Keys

```cpp
static inline std::vector< ID_T > & Keys()
```


### function Get

```cpp
static inline T Get(
    const ID_T id
)
```

Get the instance for the given ID. 

**Parameters**: 

  * **const** ID The ID of the instance to get. 


**Return**: T The ID's instance or the default one. 

This doesn't create an instance if the ID doesn't exist. Instead it just returns the default. 


### function Has

```cpp
static inline bool Has(
    const ID_T id
)
```


### function Set

```cpp
static inline void Set(
    const ID_T id,
    T instance
)
```

Set (or add/create) an instance for the given ID. 

**Parameters**: 

  * **const** ID id The ID of the instance to set. 
  * **T** instance The ID's instance. 


**Return**: void 

### function Remove

```cpp
static inline void Remove(
    const ID_T id
)
```

Remove the instance for the given ID. 

**Parameters**: 

  * **const** ID id The ID of the instance to remove. 


**Return**: void 

## Protected Attributes Documentation

### variable default_value

```cpp
static T default_value;
```


### variable instances

```cpp
static std::map< ID_T, T > instances;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC