---
title: tec::Material

---

# tec::Material





## Public Functions

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [Material](/engine/Classes/classtec_1_1_material/) > | **[Create](/engine/Classes/classtec_1_1_material/#function-create)**(const std::string name)<br>Factory method that creates a [Material](/engine/Classes/classtec_1_1_material/) and stores it in the MaterialMap under name.  |
| void | **[AddTexture](/engine/Classes/classtec_1_1_material/#function-addtexture)**(std::shared_ptr< [TextureObject](/engine/Classes/classtec_1_1_texture_object/) > tex)<br>Adds a [TextureObject]() to this material.  |
| std::shared_ptr< [TextureObject](/engine/Classes/classtec_1_1_texture_object/) > | **[GetTexture](/engine/Classes/classtec_1_1_material/#function-gettexture)**(std::size_t index)<br>Gets a [TextureObject]() from this material.  |
| void | **[RemoveTexture](/engine/Classes/classtec_1_1_material/#function-removetexture)**(std::shared_ptr< [TextureObject](/engine/Classes/classtec_1_1_texture_object/) > tex)<br>Removes a [TextureObject]() from this material.  |
| void | **[SetPolygonMode](/engine/Classes/classtec_1_1_material/#function-setpolygonmode)**(const GLenum mode)<br>Sets the PolygonMode used when this material is in use.  |
| const GLenum | **[GetPolygonMode](/engine/Classes/classtec_1_1_material/#function-getpolygonmode)**()<br>Gets the PolygonMode of this material.  |
| void | **[SetDrawElementsMode](/engine/Classes/classtec_1_1_material/#function-setdrawelementsmode)**(const GLenum mode)<br>Sets the DrawElements type used when this material is in use.  |
| const GLenum | **[GetDrawElementsMode](/engine/Classes/classtec_1_1_material/#function-getdrawelementsmode)**() const<br>Gets the DrawElements mode of this material.  |

## Friends

|                | Name           |
| -------------- | -------------- |
| class | **[RenderSystem](/engine/Classes/classtec_1_1_material/#friend-rendersystem)**  |

## Public Functions Documentation

### function Create

```cpp
static std::shared_ptr< Material > Create(
    const std::string name
)
```

Factory method that creates a [Material](/engine/Classes/classtec_1_1_material/) and stores it in the MaterialMap under name. 

**Parameters**: 

  * **const** std::string name The name to store the [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) under. 
  * **std::shared_ptr<Shader>** shader The shader this material uses. 


**Return**: std::shared_ptr<Material> The created [Material](/engine/Classes/classtec_1_1_material/). 

### function AddTexture

```cpp
void AddTexture(
    std::shared_ptr< TextureObject > tex
)
```

Adds a [TextureObject]() to this material. 

**Parameters**: 

  * **std::shared_ptr<TextureObject>** tex The texture to add. 


**Return**: void 

### function GetTexture

```cpp
std::shared_ptr< TextureObject > GetTexture(
    std::size_t index
)
```

Gets a [TextureObject]() from this material. 

**Parameters**: 

  * **size_t** index The index of the texture to grab (0-based). 


**Return**: std::shared_ptr<TextureObject> The request texture, or nullptr if index is invalid. 

### function RemoveTexture

```cpp
void RemoveTexture(
    std::shared_ptr< TextureObject > tex
)
```

Removes a [TextureObject]() from this material. 

**Parameters**: 

  * **std::shared_ptr<TextureObject>** tex The texture to remove. 


**Return**: void 

### function SetPolygonMode

```cpp
void SetPolygonMode(
    const GLenum mode
)
```

Sets the PolygonMode used when this material is in use. 

**Parameters**: 

  * **const** GLenum mode The PolygonMode (GL_POINT, GL_LINE, or GL_FILL). 


**Return**: void 

### function GetPolygonMode

```cpp
const GLenum GetPolygonMode()
```

Gets the PolygonMode of this material. 

**Return**: 

  * const GLenum The PolygonMode of this material. 
  * void 


### function SetDrawElementsMode

```cpp
void SetDrawElementsMode(
    const GLenum mode
)
```

Sets the DrawElements type used when this material is in use. 

**Parameters**: 

  * **const** GLenum mode The DrawElements (GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, or GL_PATCHES). 


**Return**: void 

### function GetDrawElementsMode

```cpp
const GLenum GetDrawElementsMode() const
```

Gets the DrawElements mode of this material. 

**Return**: 

  * const GLenum The DrawElements mode of this material. 
  * void 


## Friends

### friend RenderSystem

```cpp
friend class RenderSystem(
    RenderSystem 
);
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000