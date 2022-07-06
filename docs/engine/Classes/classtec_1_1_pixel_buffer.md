---
title: tec::PixelBuffer

---

# tec::PixelBuffer





## Public Types

|                | Name           |
| -------------- | -------------- |
| enum| **[FILTER_PREFERENCE](/engine/Classes/classtec_1_1_pixel_buffer/#enum-filter-preference)** { DEFAULT, NEAREST, LINEAR} |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/#function-pixelbuffer)**() =default |
| bool | **[Load](/engine/Classes/classtec_1_1_pixel_buffer/#function-load)**(const [Path](/engine/Classes/classtec_1_1_path/) & filename, bool gamma_space =false)<br>Returns a resource with the specified name.  |
| | **[PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/#function-pixelbuffer)**(const [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) & ) =delete |
| [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) & | **[operator=](/engine/Classes/classtec_1_1_pixel_buffer/#function-operator=)**(const [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) & ) =delete |
| | **[PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/#function-pixelbuffer)**([PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) && rv) |
| [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) & | **[operator=](/engine/Classes/classtec_1_1_pixel_buffer/#function-operator=)**([PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) && rv) |
| | **[PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/#function-pixelbuffer)**(std::uint32_t width, std::uint32_t height, std::uint32_t bitsperchannel, ImageColorMode mode)<br>Construct a new pixel buffer.  |
| bool | **[CreateCopy](/engine/Classes/classtec_1_1_pixel_buffer/#function-createcopy)**(const [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) & pbuf)<br>Create a copy of a pixel buffer.  |
| bool | **[CreateCopy](/engine/Classes/classtec_1_1_pixel_buffer/#function-createcopy)**(const [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) & pbuf, std::uint32_t bitsperchannel, ImageColorMode mode)<br>Create a copy of a pixel buffer in a different format.  |
| bool | **[Create](/engine/Classes/classtec_1_1_pixel_buffer/#function-create)**(uint32_t width, uint32_t height, uint32_t bitsperchannel, ImageColorMode mode)<br>Create a new pixel buffer, replacing the old buffer (if any).  |
| bool | **[IsDirty](/engine/Classes/classtec_1_1_pixel_buffer/#function-isdirty)**() const |
| void | **[Invalidate](/engine/Classes/classtec_1_1_pixel_buffer/#function-invalidate)**()<br>Mark dirty.  |
| void | **[Validate](/engine/Classes/classtec_1_1_pixel_buffer/#function-validate)**()<br>Mark not dirty.  |
| const std::uint8_t * | **[GetBlockBase](/engine/Classes/classtec_1_1_pixel_buffer/#function-getblockbase)**() const<br>Returns a pointer to the base address of image data for reading. This function is intended for loading pixel data from the buffer.  |
| std::mutex & | **[GetWritelock](/engine/Classes/classtec_1_1_pixel_buffer/#function-getwritelock)**() |
| std::uint8_t * | **[GetPtr](/engine/Classes/classtec_1_1_pixel_buffer/#function-getptr)**() |
| std::uint32_t | **[Width](/engine/Classes/classtec_1_1_pixel_buffer/#function-width)**() const |
| std::uint32_t | **[Height](/engine/Classes/classtec_1_1_pixel_buffer/#function-height)**() const |
| std::uint32_t | **[Pitch](/engine/Classes/classtec_1_1_pixel_buffer/#function-pitch)**() const |
| std::uint32_t | **[PixelSize](/engine/Classes/classtec_1_1_pixel_buffer/#function-pixelsize)**() const |
| ImageColorMode | **[GetFormat](/engine/Classes/classtec_1_1_pixel_buffer/#function-getformat)**() const |
| FILTER_PREFERENCE | **[GetFilter](/engine/Classes/classtec_1_1_pixel_buffer/#function-getfilter)**() const |
| void | **[SetFilter](/engine/Classes/classtec_1_1_pixel_buffer/#function-setfilter)**(FILTER_PREFERENCE f) |
| void | **[PPMDebug](/engine/Classes/classtec_1_1_pixel_buffer/#function-ppmdebug)**() |
| void | **[PPMDebug](/engine/Classes/classtec_1_1_pixel_buffer/#function-ppmdebug)**(const char * ofile) |
| std::shared_ptr< [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) > | **[Create](/engine/Classes/classtec_1_1_pixel_buffer/#function-create)**(const std::string name, const [Path](/engine/Classes/classtec_1_1_path/) & filename =[Path](/engine/Classes/classtec_1_1_path/)(), bool gamma_space =false)<br>Factory method that creates a [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) and stores it in the PixelBufferMap under name. It will optionally load a texture file with the given filename.  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::uint32_t | **[image_x](/engine/Classes/classtec_1_1_pixel_buffer/#variable-image-x)**  |
| std::uint32_t | **[image_y](/engine/Classes/classtec_1_1_pixel_buffer/#variable-image-y)**  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::int32_t | **[imagewidth](/engine/Classes/classtec_1_1_pixel_buffer/#variable-imagewidth)**  |
| std::int32_t | **[imageheight](/engine/Classes/classtec_1_1_pixel_buffer/#variable-imageheight)**  |
| std::uint32_t | **[bufferpitch](/engine/Classes/classtec_1_1_pixel_buffer/#variable-bufferpitch)** <br>number of bytes to move vertical 1 raster line  |
| std::uint32_t | **[imagepixelsize](/engine/Classes/classtec_1_1_pixel_buffer/#variable-imagepixelsize)**  |
| std::uint32_t | **[channelbitdepth](/engine/Classes/classtec_1_1_pixel_buffer/#variable-channelbitdepth)**  |
| ImageColorMode | **[imagemode](/engine/Classes/classtec_1_1_pixel_buffer/#variable-imagemode)**  |
| FILTER_PREFERENCE | **[imagefilter](/engine/Classes/classtec_1_1_pixel_buffer/#variable-imagefilter)**  |
| bool | **[dirty](/engine/Classes/classtec_1_1_pixel_buffer/#variable-dirty)**  |
| std::unique_ptr< std::uint8_t[]> | **[blockptr](/engine/Classes/classtec_1_1_pixel_buffer/#variable-blockptr)**  |
| std::mutex | **[writelock](/engine/Classes/classtec_1_1_pixel_buffer/#variable-writelock)**  |

## Public Types Documentation

### enum FILTER_PREFERENCE

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| DEFAULT | |   |
| NEAREST | |   |
| LINEAR | |   |




## Public Functions Documentation

### function PixelBuffer

```cpp
PixelBuffer() =default
```


### function Load

```cpp
bool Load(
    const Path & filename,
    bool gamma_space =false
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** [Path](/engine/Classes/classtec_1_1_path/) filename The filename of the image file to load. 


**Return**: bool True if initialization finished with no errors. 

### function PixelBuffer

```cpp
PixelBuffer(
    const PixelBuffer & 
) =delete
```


### function operator=

```cpp
PixelBuffer & operator=(
    const PixelBuffer & 
) =delete
```


### function PixelBuffer

```cpp
PixelBuffer(
    PixelBuffer && rv
)
```


### function operator=

```cpp
PixelBuffer & operator=(
    PixelBuffer && rv
)
```


### function PixelBuffer

```cpp
PixelBuffer(
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t bitsperchannel,
    ImageColorMode mode
)
```

Construct a new pixel buffer. 

**Parameters**: 

  * **width** the width of the buffer in pixels 
  * **height** the height of the buffer in pixels 
  * **bitsperchannel** the number of bits in a single channel, usually 8 
  * **mode** greyscale (Monochrome) or color (RGB), and whether there is alpha or no alpha 


### function CreateCopy

```cpp
bool CreateCopy(
    const PixelBuffer & pbuf
)
```

Create a copy of a pixel buffer. 

**Parameters**: 

  * **pbuf** the pixel buffer to copy from 


**Return**: true on success 

### function CreateCopy

```cpp
bool CreateCopy(
    const PixelBuffer & pbuf,
    std::uint32_t bitsperchannel,
    ImageColorMode mode
)
```

Create a copy of a pixel buffer in a different format. 

**Parameters**: 

  * **pbuf** the pixel buffer to copy from 
  * **bitsperchannel** the number of bits in a single channel, usually 8 
  * **mode** greyscale (Monochrome) or color (RGB), and whether there is alpha or no alpha 


**Return**: true on success 

### function Create

```cpp
bool Create(
    uint32_t width,
    uint32_t height,
    uint32_t bitsperchannel,
    ImageColorMode mode
)
```

Create a new pixel buffer, replacing the old buffer (if any). 

**Parameters**: 

  * **width** the width of the buffer in pixels 
  * **height** the height of the buffer in pixels 
  * **bitsperchannel** the number of bits in a single channel, usually 8 
  * **mode** greyscale (Monochrome) or color (RGB), and whether there is alpha or no alpha 


**Return**: true on success 

### function IsDirty

```cpp
bool IsDirty() const
```


### function Invalidate

```cpp
void Invalidate()
```

Mark dirty. 

### function Validate

```cpp
void Validate()
```

Mark not dirty. 

### function GetBlockBase

```cpp
const std::uint8_t * GetBlockBase() const
```

Returns a pointer to the base address of image data for reading. This function is intended for loading pixel data from the buffer. 

**Return**: uint8_t * base image address or nullptr if invalid. 

### function GetWritelock

```cpp
inline std::mutex & GetWritelock()
```


### function GetPtr

```cpp
inline std::uint8_t * GetPtr()
```


### function Width

```cpp
inline std::uint32_t Width() const
```


### function Height

```cpp
inline std::uint32_t Height() const
```


### function Pitch

```cpp
inline std::uint32_t Pitch() const
```


### function PixelSize

```cpp
inline std::uint32_t PixelSize() const
```


### function GetFormat

```cpp
inline ImageColorMode GetFormat() const
```


### function GetFilter

```cpp
inline FILTER_PREFERENCE GetFilter() const
```


### function SetFilter

```cpp
inline void SetFilter(
    FILTER_PREFERENCE f
)
```


### function PPMDebug

```cpp
void PPMDebug()
```


### function PPMDebug

```cpp
void PPMDebug(
    const char * ofile
)
```


### function Create

```cpp
static std::shared_ptr< PixelBuffer > Create(
    const std::string name,
    const Path & filename =Path(),
    bool gamma_space =false
)
```

Factory method that creates a [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) and stores it in the PixelBufferMap under name. It will optionally load a texture file with the given filename. 

**Parameters**: 

  * **const** std::string name The name to store the [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/) under. 
  * **const** [Path](/engine/Classes/classtec_1_1_path/) filename The optional filename of the image to load. 


**Return**: std::shared_ptr<PixelBuffer> The created [PixelBuffer](/engine/Classes/classtec_1_1_pixel_buffer/). 

## Public Attributes Documentation

### variable image_x

```cpp
std::uint32_t image_x {0};
```


### variable image_y

```cpp
std::uint32_t image_y {0};
```


## Protected Attributes Documentation

### variable imagewidth

```cpp
std::int32_t imagewidth {0};
```


### variable imageheight

```cpp
std::int32_t imageheight {0};
```


### variable bufferpitch

```cpp
std::uint32_t bufferpitch {0};
```

number of bytes to move vertical 1 raster line 

### variable imagepixelsize

```cpp
std::uint32_t imagepixelsize {0};
```


### variable channelbitdepth

```cpp
std::uint32_t channelbitdepth {0};
```


### variable imagemode

```cpp
ImageColorMode imagemode {ImageColorMode::MONOCHROME};
```


### variable imagefilter

```cpp
FILTER_PREFERENCE imagefilter {DEFAULT};
```


### variable dirty

```cpp
bool dirty {false};
```


### variable blockptr

```cpp
std::unique_ptr< std::uint8_t[]> blockptr;
```


### variable writelock

```cpp
std::mutex writelock;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000