---
title: tec::vertex

---

# tec::vertex



## Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[tec::vertex::AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/)**  |
| struct | **[tec::vertex::ColorVertex](/engine/Classes/structtec_1_1vertex_1_1_color_vertex/)**  |
| struct | **[tec::vertex::FormatDefine](/engine/Classes/structtec_1_1vertex_1_1_format_define/)**  |
| struct | **[tec::vertex::FormatDefinition](/engine/Classes/structtec_1_1vertex_1_1_format_definition/)** <br>Holds relevent data to process and bind vertex data via glVertexAttrib*Pointer calls The convert() function loads data from a [Mesh](/engine/Classes/structtec_1_1_mesh/) into the described format.  |
| struct | **[tec::vertex::FormatInfo](/engine/Classes/structtec_1_1vertex_1_1_format_info/)**  |
| struct | **[tec::vertex::FullVertex](/engine/Classes/structtec_1_1vertex_1_1_full_vertex/)**  |
| struct | **[tec::vertex::LiteFullVertex](/engine/Classes/structtec_1_1vertex_1_1_lite_full_vertex/)**  |
| struct | **[tec::vertex::PackWeightVertex](/engine/Classes/structtec_1_1vertex_1_1_pack_weight_vertex/)**  |
| struct | **[tec::vertex::Vertex](/engine/Classes/structtec_1_1vertex_1_1_vertex/)**  |
| struct | **[tec::vertex::WeightData](/engine/Classes/structtec_1_1vertex_1_1_weight_data/)**  |
| struct | **[tec::vertex::WeightVertex](/engine/Classes/structtec_1_1vertex_1_1_weight_vertex/)**  |

## Types

|                | Name           |
| -------------- | -------------- |
| enum| **[FormatCode](/engine/Namespaces/namespacetec_1_1vertex/#enum-formatcode)** { VF_BASE, VF_LITEFULL, VF_WEIGHT, VF_COLOR, VF_PWC, VF_FULL} |

## Attributes

|                | Name           |
| -------------- | -------------- |
| const [FormatDefinition](/engine/Classes/structtec_1_1vertex_1_1_format_definition/) *[] | **[from_vftype](/engine/Namespaces/namespacetec_1_1vertex/#variable-from-vftype)**  |

## Types Documentation

### enum FormatCode

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| VF_BASE | |   |
| VF_LITEFULL | |   |
| VF_WEIGHT | |   |
| VF_COLOR | |   |
| VF_PWC | |   |
| VF_FULL | |   |






## Attributes Documentation

### variable from_vftype

```cpp
const FormatDefinition *[] from_vftype {
		&FormatInfo<Vertex>::define_format, 
		&FormatInfo<LiteFullVertex>::define_format, 
		nullptr, 
		&FormatInfo<ColorVertex>::define_format, 
		nullptr, 
		&FormatInfo<FullVertex>::define_format, 
};
```





-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000