---
title: tec::vertex::FormatInfo

---

# tec::vertex::FormatInfo



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| [FormatDefine](/engine/Classes/structtec_1_1vertex_1_1_format_define/)< [Vertex](/engine/Classes/structtec_1_1vertex_1_1_vertex/) > | **[define_format](/engine/Classes/structtec_1_1vertex_1_1_format_info/#function-define-format)**((void *dest, const [Mesh](/engine/Classes/structtec_1_1_mesh/) *source) { auto destination=([vertex::Vertex](/engine/Classes/structtec_1_1vertex_1_1_vertex/) *) destfor(size_t i=0i< source->vert_count() i++) { destination[i]=source->base_verts[i]} } , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){0, 3, GL_FLOAT, GL_FALSE, offsetof([Vertex](/engine/Classes/structtec_1_1vertex_1_1_vertex/), position)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){1, 3, GL_FLOAT, GL_FALSE, offsetof([Vertex](/engine/Classes/structtec_1_1vertex_1_1_vertex/), normal)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){2, 2, GL_FLOAT, GL_FALSE, offsetof([Vertex](/engine/Classes/structtec_1_1vertex_1_1_vertex/), uv)} ) |
| [FormatDefine](/engine/Classes/structtec_1_1vertex_1_1_format_define/)< [ColorVertex](/engine/Classes/structtec_1_1vertex_1_1_color_vertex/) > | **[define_format](/engine/Classes/structtec_1_1vertex_1_1_format_info/#function-define-format)**((void *dest, const [Mesh](/engine/Classes/structtec_1_1_mesh/) *source) { auto destination=([vertex::ColorVertex](/engine/Classes/structtec_1_1vertex_1_1_color_vertex/) *) destfor(size_t i=0i< source->vert_count() i++) { destination[i].vert=source->base_verts[i]destination[i].color=source->vert_color[i]} } , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){0, 3, GL_FLOAT, GL_FALSE, offsetof([ColorVertex](/engine/Classes/structtec_1_1vertex_1_1_color_vertex/), vert.position)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){1, 3, GL_FLOAT, GL_FALSE, offsetof([ColorVertex](/engine/Classes/structtec_1_1vertex_1_1_color_vertex/), vert.normal)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){2, 2, GL_FLOAT, GL_FALSE, offsetof([ColorVertex](/engine/Classes/structtec_1_1vertex_1_1_color_vertex/), vert.uv)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){3, 4, GL_FLOAT, GL_FALSE, offsetof([ColorVertex](/engine/Classes/structtec_1_1vertex_1_1_color_vertex/), color)} ) |
| [FormatDefine](/engine/Classes/structtec_1_1vertex_1_1_format_define/)< [LiteFullVertex](/engine/Classes/structtec_1_1vertex_1_1_lite_full_vertex/) > | **[define_format](/engine/Classes/structtec_1_1vertex_1_1_format_info/#function-define-format)**((void *dest, const [Mesh](/engine/Classes/structtec_1_1_mesh/) *source) { auto destination=([vertex::LiteFullVertex](/engine/Classes/structtec_1_1vertex_1_1_lite_full_vertex/) *) destfor(size_t i=0i< source->vert_count() i++) { destination[i].position=source->base_verts[i].positiondestination[i].pack_normal=glm::packSnorm3x10_1x2(glm::vec4(source->base_verts[i].normal, 0.f)) *reinterpret_cast< uint32_t * >(&destination[i].uv)=glm::packHalf2x16(source->base_verts[i].uv) if(source->has_color) *reinterpret_cast< uint32_t * >(&destination[i].color)=glm::packUnorm4x8(source->vert_color[i]) if(source->has_weight) { *reinterpret_cast< uint32_t * >(&destination[i].bone_weights)=glm::packUnorm4x8(source->vert_weight[i].bone_weights) destination[i].bone_indices=source->vert_weight[i].bone_indices} } } , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){0, 3, GL_FLOAT, GL_FALSE, offsetof([LiteFullVertex](/engine/Classes/structtec_1_1vertex_1_1_lite_full_vertex/), position)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){1, 4, GL_INT_2_10_10_10_REV, GL_TRUE, offsetof([LiteFullVertex](/engine/Classes/structtec_1_1vertex_1_1_lite_full_vertex/), pack_normal)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){2, 2, GL_HALF_FLOAT, GL_FALSE, offsetof([LiteFullVertex](/engine/Classes/structtec_1_1vertex_1_1_lite_full_vertex/), uv)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){3, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof([LiteFullVertex](/engine/Classes/structtec_1_1vertex_1_1_lite_full_vertex/), color)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){4, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof([LiteFullVertex](/engine/Classes/structtec_1_1vertex_1_1_lite_full_vertex/), bone_weights)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){5, 4, GL_UNSIGNED_BYTE, GL_FALSE, offsetof([LiteFullVertex](/engine/Classes/structtec_1_1vertex_1_1_lite_full_vertex/), bone_indices)} ) |
| [FormatDefine](/engine/Classes/structtec_1_1vertex_1_1_format_define/)< [FullVertex](/engine/Classes/structtec_1_1vertex_1_1_full_vertex/) > | **[define_format](/engine/Classes/structtec_1_1vertex_1_1_format_info/#function-define-format)**((void *dest, const [Mesh](/engine/Classes/structtec_1_1_mesh/) *source) { auto destination=([vertex::FullVertex](/engine/Classes/structtec_1_1vertex_1_1_full_vertex/) *) destfor(size_t i=0i< source->vert_count() i++) { destination[i].vert=source->base_verts[i]if(source->has_color) destination[i].color=source->vert_color[i]if(source->has_weight) destination[i].weight=source->vert_weight[i]} } , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){0, 3, GL_FLOAT, GL_FALSE, offsetof([FullVertex](/engine/Classes/structtec_1_1vertex_1_1_full_vertex/), vert.position)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){1, 3, GL_FLOAT, GL_FALSE, offsetof([FullVertex](/engine/Classes/structtec_1_1vertex_1_1_full_vertex/), vert.normal)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){2, 2, GL_FLOAT, GL_FALSE, offsetof([FullVertex](/engine/Classes/structtec_1_1vertex_1_1_full_vertex/), vert.uv)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){3, 4, GL_FLOAT, GL_FALSE, offsetof([FullVertex](/engine/Classes/structtec_1_1vertex_1_1_full_vertex/), color)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){4, 4, GL_FLOAT, GL_FALSE, offsetof([FullVertex](/engine/Classes/structtec_1_1vertex_1_1_full_vertex/), weight.bone_weights)} , [AttribData](/engine/Classes/structtec_1_1vertex_1_1_attrib_data/){5, 4, GL_UNSIGNED_INT, GL_FALSE, offsetof([FullVertex](/engine/Classes/structtec_1_1vertex_1_1_full_vertex/), weight.bone_indices)} ) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| [FormatDefine](/engine/Classes/structtec_1_1vertex_1_1_format_define/)< Q > | **[define_format](/engine/Classes/structtec_1_1vertex_1_1_format_info/#variable-define-format)**  |

## Detailed Description

```cpp
template <typename Q >
struct tec::vertex::FormatInfo;
```

## Public Functions Documentation

### function define_format

```cpp
FormatDefine< Vertex > define_format(
    (void *dest, const Mesh *source) { auto destination=(vertex::Vertex *) destfor(size_t i=0i< source->vert_count() i++) { destination[i]=source->base_verts[i]} } ,
    AttribData{0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position)} ,
    AttribData{1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal)} ,
    AttribData{2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv)} 
)
```


### function define_format

```cpp
FormatDefine< ColorVertex > define_format(
    (void *dest, const Mesh *source) { auto destination=(vertex::ColorVertex *) destfor(size_t i=0i< source->vert_count() i++) { destination[i].vert=source->base_verts[i]destination[i].color=source->vert_color[i]} } ,
    AttribData{0, 3, GL_FLOAT, GL_FALSE, offsetof(ColorVertex, vert.position)} ,
    AttribData{1, 3, GL_FLOAT, GL_FALSE, offsetof(ColorVertex, vert.normal)} ,
    AttribData{2, 2, GL_FLOAT, GL_FALSE, offsetof(ColorVertex, vert.uv)} ,
    AttribData{3, 4, GL_FLOAT, GL_FALSE, offsetof(ColorVertex, color)} 
)
```


### function define_format

```cpp
FormatDefine< LiteFullVertex > define_format(
    (void *dest, const Mesh *source) { auto destination=(vertex::LiteFullVertex *) destfor(size_t i=0i< source->vert_count() i++) { destination[i].position=source->base_verts[i].positiondestination[i].pack_normal=glm::packSnorm3x10_1x2(glm::vec4(source->base_verts[i].normal, 0.f)) *reinterpret_cast< uint32_t * >(&destination[i].uv)=glm::packHalf2x16(source->base_verts[i].uv) if(source->has_color) *reinterpret_cast< uint32_t * >(&destination[i].color)=glm::packUnorm4x8(source->vert_color[i]) if(source->has_weight) { *reinterpret_cast< uint32_t * >(&destination[i].bone_weights)=glm::packUnorm4x8(source->vert_weight[i].bone_weights) destination[i].bone_indices=source->vert_weight[i].bone_indices} } } ,
    AttribData{0, 3, GL_FLOAT, GL_FALSE, offsetof(LiteFullVertex, position)} ,
    AttribData{1, 4, GL_INT_2_10_10_10_REV, GL_TRUE, offsetof(LiteFullVertex, pack_normal)} ,
    AttribData{2, 2, GL_HALF_FLOAT, GL_FALSE, offsetof(LiteFullVertex, uv)} ,
    AttribData{3, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(LiteFullVertex, color)} ,
    AttribData{4, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(LiteFullVertex, bone_weights)} ,
    AttribData{5, 4, GL_UNSIGNED_BYTE, GL_FALSE, offsetof(LiteFullVertex, bone_indices)} 
)
```


### function define_format

```cpp
FormatDefine< FullVertex > define_format(
    (void *dest, const Mesh *source) { auto destination=(vertex::FullVertex *) destfor(size_t i=0i< source->vert_count() i++) { destination[i].vert=source->base_verts[i]if(source->has_color) destination[i].color=source->vert_color[i]if(source->has_weight) destination[i].weight=source->vert_weight[i]} } ,
    AttribData{0, 3, GL_FLOAT, GL_FALSE, offsetof(FullVertex, vert.position)} ,
    AttribData{1, 3, GL_FLOAT, GL_FALSE, offsetof(FullVertex, vert.normal)} ,
    AttribData{2, 2, GL_FLOAT, GL_FALSE, offsetof(FullVertex, vert.uv)} ,
    AttribData{3, 4, GL_FLOAT, GL_FALSE, offsetof(FullVertex, color)} ,
    AttribData{4, 4, GL_FLOAT, GL_FALSE, offsetof(FullVertex, weight.bone_weights)} ,
    AttribData{5, 4, GL_UNSIGNED_INT, GL_FALSE, offsetof(FullVertex, weight.bone_indices)} 
)
```


## Public Attributes Documentation

### variable define_format

```cpp
static FormatDefine< Q > define_format;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000