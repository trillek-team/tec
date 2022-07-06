---
title: tec::TextureObject

---

# tec::TextureObject





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[TextureObject](/engine/Classes/classtec_1_1_texture_object/#function-textureobject)**() |
| | **[~TextureObject](/engine/Classes/classtec_1_1_texture_object/#function-~textureobject)**() |
| | **[TextureObject](/engine/Classes/classtec_1_1_texture_object/#function-textureobject)**(const [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) & image)<br>new texture instance from image  |
| | **[TextureObject](/engine/Classes/classtec_1_1_texture_object/#function-textureobject)**(std::weak_ptr< [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) > pbp)<br>new texture instance from an image pointer  |
| | **[TextureObject](/engine/Classes/classtec_1_1_texture_object/#function-textureobject)**(const [TextureObject](/engine/Classes/classtec_1_1_texture_object/) & ) =delete |
| [TextureObject](/engine/Classes/classtec_1_1_texture_object/) & | **[operator=](/engine/Classes/classtec_1_1_texture_object/#function-operator=)**(const [TextureObject](/engine/Classes/classtec_1_1_texture_object/) & ) =delete |
| | **[TextureObject](/engine/Classes/classtec_1_1_texture_object/#function-textureobject)**([TextureObject](/engine/Classes/classtec_1_1_texture_object/) && other) |
| [TextureObject](/engine/Classes/classtec_1_1_texture_object/) & | **[operator=](/engine/Classes/classtec_1_1_texture_object/#function-operator=)**([TextureObject](/engine/Classes/classtec_1_1_texture_object/) && other) |
| void | **[Destroy](/engine/Classes/classtec_1_1_texture_object/#function-destroy)**()<br>delete the underlaying GL texture  |
| GLuint | **[GetID](/engine/Classes/classtec_1_1_texture_object/#function-getid)**()<br>get the ID of the texture note: this method is not const, since GL can modify the texture with the ID  |
| bool | **[IsDynamic](/engine/Classes/classtec_1_1_texture_object/#function-isdynamic)**() |
| void | **[Update](/engine/Classes/classtec_1_1_texture_object/#function-update)**() |
| void | **[SetCompare](/engine/Classes/classtec_1_1_texture_object/#function-setcompare)**(bool c) |
| void | **[Load](/engine/Classes/classtec_1_1_texture_object/#function-load)**(const [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) & image)<br>create a texture from an image  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| GLuint | **[texture_id](/engine/Classes/classtec_1_1_texture_object/#variable-texture-id)**  |
| bool | **[compare](/engine/Classes/classtec_1_1_texture_object/#variable-compare)**  |
| std::weak_ptr< [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) > | **[source_ptr](/engine/Classes/classtec_1_1_texture_object/#variable-source-ptr)**  |

## Public Functions Documentation

### function TextureObject

```cpp
inline TextureObject()
```


### function ~TextureObject

```cpp
~TextureObject()
```


### function TextureObject

```cpp
TextureObject(
    const PixelBuffer & image
)
```

new texture instance from image 

### function TextureObject

```cpp
TextureObject(
    std::weak_ptr< PixelBuffer > pbp
)
```

new texture instance from an image pointer 

### function TextureObject

```cpp
TextureObject(
    const TextureObject & 
) =delete
```


### function operator=

```cpp
TextureObject & operator=(
    const TextureObject & 
) =delete
```


### function TextureObject

```cpp
TextureObject(
    TextureObject && other
)
```


### function operator=

```cpp
TextureObject & operator=(
    TextureObject && other
)
```


### function Destroy

```cpp
void Destroy()
```

delete the underlaying GL texture 

### function GetID

```cpp
inline GLuint GetID()
```

get the ID of the texture note: this method is not const, since GL can modify the texture with the ID 

**Return**: GLuint the GL texture ID 

### function IsDynamic

```cpp
inline bool IsDynamic()
```


**Return**: true if the texture was created dynamic 

### function Update

```cpp
void Update()
```


Called by the [RenderSystem](/engine/Classes/classtec_1_1_render_system/) to update dynamic textures 


### function SetCompare

```cpp
inline void SetCompare(
    bool c
)
```


### function Load

```cpp
void Load(
    const PixelBuffer & image
)
```

create a texture from an image 

## Protected Attributes Documentation

### variable texture_id

```cpp
GLuint texture_id {0};
```


### variable compare

```cpp
bool compare {0};
```


### variable source_ptr

```cpp
std::weak_ptr< PixelBuffer > source_ptr;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000