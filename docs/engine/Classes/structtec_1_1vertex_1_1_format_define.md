---
title: tec::vertex::FormatDefine

---

# tec::vertex::FormatDefine



 [More...](#detailed-description)

Inherits from [tec::vertex::FormatDefinition](/engine/Classes/structtec_1_1vertex_1_1_format_definition/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| template <typename... A\> <br>| **[FormatDefine](/engine/Classes/structtec_1_1vertex_1_1_format_define/#function-formatdefine)**(func_t f, A &&... a) |

## Additional inherited members

**Public Types inherited from [tec::vertex::FormatDefinition](/engine/Classes/structtec_1_1vertex_1_1_format_definition/)**

|                | Name           |
| -------------- | -------------- |
| typedef std::function< void(void *dest, const [Mesh](/engine/Classes/structtec_1_1_mesh/) *source)> | **[func_t](/engine/Classes/structtec_1_1vertex_1_1_format_definition/#typedef-func-t)**  |

**Public Functions inherited from [tec::vertex::FormatDefinition](/engine/Classes/structtec_1_1vertex_1_1_format_definition/)**

|                | Name           |
| -------------- | -------------- |
| template <typename C ,typename... A\> <br>| **[FormatDefinition](/engine/Classes/structtec_1_1vertex_1_1_format_definition/#function-formatdefinition)**(const C * , func_t f, A &&... a) |

**Public Attributes inherited from [tec::vertex::FormatDefinition](/engine/Classes/structtec_1_1vertex_1_1_format_definition/)**

|                | Name           |
| -------------- | -------------- |
| const size_t | **[format_stride](/engine/Classes/structtec_1_1vertex_1_1_format_definition/#variable-format-stride)**  |
| const std::vector< [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/) > | **[info](/engine/Classes/structtec_1_1vertex_1_1_format_definition/#variable-info)**  |
| const func_t | **[convert](/engine/Classes/structtec_1_1vertex_1_1_format_definition/#variable-convert)**  |


## Detailed Description

```cpp
template <typename T >
struct tec::vertex::FormatDefine;
```

## Public Functions Documentation

### function FormatDefine

```cpp
template <typename... A>
inline FormatDefine(
    func_t f,
    A &&... a
)
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000