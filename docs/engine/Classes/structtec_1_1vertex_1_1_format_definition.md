---
title: tec::vertex::FormatDefinition
summary: Holds relevent data to process and bind vertex data via glVertexAttrib*Pointer calls The convert() function loads data from a Mesh into the described format. 

---

# tec::vertex::FormatDefinition



Holds relevent data to process and bind vertex data via glVertexAttrib*Pointer calls The convert() function loads data from a [Mesh](/engine/Classes/structtec_1_1_mesh/) into the described format. 

Inherited by [tec::vertex::FormatDefine< Q >](/engine/Classes/structtec_1_1vertex_1_1_format_define/), [tec::vertex::FormatDefine< T >](/engine/Classes/structtec_1_1vertex_1_1_format_define/)

## Public Types

|                | Name           |
| -------------- | -------------- |
| typedef std::function< void(void *dest, const [Mesh](/engine/Classes/structtec_1_1_mesh/) *source)> | **[func_t](/engine/Classes/structtec_1_1vertex_1_1_format_definition/#typedef-func-t)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| template <typename C ,typename... A\> <br>| **[FormatDefinition](/engine/Classes/structtec_1_1vertex_1_1_format_definition/#function-formatdefinition)**(const C * , func_t f, A &&... a) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| const size_t | **[format_stride](/engine/Classes/structtec_1_1vertex_1_1_format_definition/#variable-format-stride)**  |
| const std::vector< [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/) > | **[info](/engine/Classes/structtec_1_1vertex_1_1_format_definition/#variable-info)**  |
| const func_t | **[convert](/engine/Classes/structtec_1_1vertex_1_1_format_definition/#variable-convert)**  |

## Public Types Documentation

### typedef func_t

```cpp
typedef std::function<void(void* dest, const Mesh* source)> tec::vertex::FormatDefinition::func_t;
```


## Public Functions Documentation

### function FormatDefinition

```cpp
template <typename C ,
typename... A>
inline FormatDefinition(
    const C * ,
    func_t f,
    A &&... a
)
```


## Public Attributes Documentation

### variable format_stride

```cpp
const size_t format_stride;
```


### variable info

```cpp
const std::vector< AttribData > info;
```


### variable convert

```cpp
const func_t convert;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000