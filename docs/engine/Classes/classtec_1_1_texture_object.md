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
| | **[TextureObject](/engine/Classes/classtec_1_1_texture_object/#function-textureobject)**(const [TextureObject](/engine/Classes/classtec_1_1_texture_object/) & ) |
| [TextureObject](/engine/Classes/classtec_1_1_texture_object/) & | **[operator=](/engine/Classes/classtec_1_1_texture_object/#function-operator=)**(const [TextureObject](/engine/Classes/classtec_1_1_texture_object/) & ) |
| | **[TextureObject](/engine/Classes/classtec_1_1_texture_object/#function-textureobject)**([TextureObject](/engine/Classes/classtec_1_1_texture_object/) && other) |
| [TextureObject](/engine/Classes/classtec_1_1_texture_object/) & | **[operator=](/engine/Classes/classtec_1_1_texture_object/#function-operator=)**([TextureObject](/engine/Classes/classtec_1_1_texture_object/) && other) |
| void | **[Destroy](/engine/Classes/classtec_1_1_texture_object/#function-destroy)**()<br>delete the underlaying GL texture  |
| GLuint | **[GetID](/engine/Classes/classtec_1_1_texture_object/#function-getid)**()<br>get the ID of the texture note: this method is not const, since GL can modify the texture with the ID  |
| bool | **[IsDynamic](/engine/Classes/classtec_1_1_texture_object/#function-isdynamic)**() |
| void | **[Update](/engine/Classes/classtec_1_1_texture_object/#function-update)**() |
| void | **[SetCompare](/engine/Classes/classtec_1_1_texture_object/#function-setcompare)**(bool c) |
| void | **[Load](/engine/Classes/classtec_1_1_texture_object/#function-load)**(const [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) & image)<br>create a texture from an image  |
| void | **[Load](/engine/Classes/classtec_1_1_texture_object/#function-load)**(const uint8_t * image, GLuint width, GLuint height)<br>create a texture from raw image data  |
| void | **[Generate](/engine/Classes/classtec_1_1_texture_object/#function-generate)**(GLuint width, GLuint height, bool usealpha)<br>create a blank texture RGB or RGBA format  |
| void | **[GenerateDepth](/engine/Classes/classtec_1_1_texture_object/#function-generatedepth)**(GLuint width, GLuint height, bool stencil)<br>create a blank depth texture with or without stencil  |
| void | **[GenerateStencil](/engine/Classes/classtec_1_1_texture_object/#function-generatestencil)**(GLuint width, GLuint height)<br>create a blank stencil texture  |
| void | **[GenerateMultisample](/engine/Classes/classtec_1_1_texture_object/#function-generatemultisample)**(GLuint width, GLuint height, GLuint samples)<br>create a blank multi-sample texture RGB or RGBA format  |
| void | **[GenerateMultisampleDepth](/engine/Classes/classtec_1_1_texture_object/#function-generatemultisampledepth)**(GLuint width, GLuint height, GLuint samples, bool stencil)<br>create a blank multi-sample depth texture with or without stencil  |
| void | **[GenerateMultisampleStencil](/engine/Classes/classtec_1_1_texture_object/#function-generatemultisamplestencil)**(GLuint width, GLuint height, GLuint samples)<br>create a blank multi-sample stencil texture  |
| bool | **[operator==](/engine/Classes/classtec_1_1_texture_object/#function-operator==)**(const [TextureObject](/engine/Classes/classtec_1_1_texture_object/) & other) |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| GLuint | **[texture_id](/engine/Classes/classtec_1_1_texture_object/#variable-texture_id)**  |
| bool | **[compare](/engine/Classes/classtec_1_1_texture_object/#variable-compare)**  |
| std::weak_ptr< [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) > | **[source_ptr](/engine/Classes/classtec_1_1_texture_object/#variable-source_ptr)**  |

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

Updated on 21 March 2021 at 16:29:13 UTC