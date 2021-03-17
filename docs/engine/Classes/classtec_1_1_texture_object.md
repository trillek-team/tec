---
title: tec::TextureObject

---

# tec::TextureObject



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[TextureObject](Classes/classtec_1_1_texture_object.md#function-textureobject)**() |
| | **[~TextureObject](Classes/classtec_1_1_texture_object.md#function-~textureobject)**() |
| | **[TextureObject](Classes/classtec_1_1_texture_object.md#function-textureobject)**(const [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) & image)<br>new texture instance from image  |
| | **[TextureObject](Classes/classtec_1_1_texture_object.md#function-textureobject)**(std::weak_ptr< [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) > pbp)<br>new texture instance from an image pointer  |
| | **[TextureObject](Classes/classtec_1_1_texture_object.md#function-textureobject)**(const [TextureObject](Classes/classtec_1_1_texture_object.md) & ) |
| [TextureObject](Classes/classtec_1_1_texture_object.md) & | **[operator=](Classes/classtec_1_1_texture_object.md#function-operator=)**(const [TextureObject](Classes/classtec_1_1_texture_object.md) & ) |
| | **[TextureObject](Classes/classtec_1_1_texture_object.md#function-textureobject)**([TextureObject](Classes/classtec_1_1_texture_object.md) && other) |
| [TextureObject](Classes/classtec_1_1_texture_object.md) & | **[operator=](Classes/classtec_1_1_texture_object.md#function-operator=)**([TextureObject](Classes/classtec_1_1_texture_object.md) && other) |
| void | **[Destroy](Classes/classtec_1_1_texture_object.md#function-destroy)**()<br>delete the underlaying GL texture  |
| GLuint | **[GetID](Classes/classtec_1_1_texture_object.md#function-getid)**()<br>get the ID of the texture note: this method is not const, since GL can modify the texture with the ID  |
| bool | **[IsDynamic](Classes/classtec_1_1_texture_object.md#function-isdynamic)**() |
| void | **[Update](Classes/classtec_1_1_texture_object.md#function-update)**() |
| void | **[SetCompare](Classes/classtec_1_1_texture_object.md#function-setcompare)**(bool c) |
| void | **[Load](Classes/classtec_1_1_texture_object.md#function-load)**(const [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) & image)<br>create a texture from an image  |
| void | **[Load](Classes/classtec_1_1_texture_object.md#function-load)**(const uint8_t * image, GLuint width, GLuint height)<br>create a texture from raw image data  |
| void | **[Generate](Classes/classtec_1_1_texture_object.md#function-generate)**(GLuint width, GLuint height, bool usealpha)<br>create a blank texture RGB or RGBA format  |
| void | **[GenerateDepth](Classes/classtec_1_1_texture_object.md#function-generatedepth)**(GLuint width, GLuint height, bool stencil)<br>create a blank depth texture with or without stencil  |
| void | **[GenerateStencil](Classes/classtec_1_1_texture_object.md#function-generatestencil)**(GLuint width, GLuint height)<br>create a blank stencil texture  |
| void | **[GenerateMultisample](Classes/classtec_1_1_texture_object.md#function-generatemultisample)**(GLuint width, GLuint height, GLuint samples)<br>create a blank multi-sample texture RGB or RGBA format  |
| void | **[GenerateMultisampleDepth](Classes/classtec_1_1_texture_object.md#function-generatemultisampledepth)**(GLuint width, GLuint height, GLuint samples, bool stencil)<br>create a blank multi-sample depth texture with or without stencil  |
| void | **[GenerateMultisampleStencil](Classes/classtec_1_1_texture_object.md#function-generatemultisamplestencil)**(GLuint width, GLuint height, GLuint samples)<br>create a blank multi-sample stencil texture  |
| bool | **[operator==](Classes/classtec_1_1_texture_object.md#function-operator==)**(const [TextureObject](Classes/classtec_1_1_texture_object.md) & other) |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| GLuint | **[texture_id](Classes/classtec_1_1_texture_object.md#variable-texture_id)**  |
| bool | **[compare](Classes/classtec_1_1_texture_object.md#variable-compare)**  |
| std::weak_ptr< [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) > | **[source_ptr](Classes/classtec_1_1_texture_object.md#variable-source_ptr)**  |

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
)
```


### function operator=

```cpp
TextureObject & operator=(
    const TextureObject & 
)
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


Called by the [RenderSystem](Classes/classtec_1_1_render_system.md) to update dynamic textures 


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

### function Load

```cpp
void Load(
    const uint8_t * image,
    GLuint width,
    GLuint height
)
```

create a texture from raw image data 

### function Generate

```cpp
void Generate(
    GLuint width,
    GLuint height,
    bool usealpha
)
```

create a blank texture RGB or RGBA format 

### function GenerateDepth

```cpp
void GenerateDepth(
    GLuint width,
    GLuint height,
    bool stencil
)
```

create a blank depth texture with or without stencil 

### function GenerateStencil

```cpp
void GenerateStencil(
    GLuint width,
    GLuint height
)
```

create a blank stencil texture 

### function GenerateMultisample

```cpp
void GenerateMultisample(
    GLuint width,
    GLuint height,
    GLuint samples
)
```

create a blank multi-sample texture RGB or RGBA format 

### function GenerateMultisampleDepth

```cpp
void GenerateMultisampleDepth(
    GLuint width,
    GLuint height,
    GLuint samples,
    bool stencil
)
```

create a blank multi-sample depth texture with or without stencil 

### function GenerateMultisampleStencil

```cpp
void GenerateMultisampleStencil(
    GLuint width,
    GLuint height,
    GLuint samples
)
```

create a blank multi-sample stencil texture 

### function operator==

```cpp
inline bool operator==(
    const TextureObject & other
)
```


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

Updated on 17 March 2021 at 23:08:11 UTC