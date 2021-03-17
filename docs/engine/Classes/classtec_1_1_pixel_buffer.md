---
title: tec::PixelBuffer

---

# tec::PixelBuffer



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[PixelBuffer](Classes/classtec_1_1_pixel_buffer.md#function-pixelbuffer)**() =default |
| bool | **[Load](Classes/classtec_1_1_pixel_buffer.md#function-load)**(const [FilePath](Classes/classtec_1_1_file_path.md) & filename)<br>Returns a resource with the specified name.  |
| | **[PixelBuffer](Classes/classtec_1_1_pixel_buffer.md#function-pixelbuffer)**(const [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) & ) |
| [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) & | **[operator=](Classes/classtec_1_1_pixel_buffer.md#function-operator=)**(const [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) & ) |
| | **[PixelBuffer](Classes/classtec_1_1_pixel_buffer.md#function-pixelbuffer)**([PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) && rv) |
| [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) & | **[operator=](Classes/classtec_1_1_pixel_buffer.md#function-operator=)**([PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) && rv) |
| | **[PixelBuffer](Classes/classtec_1_1_pixel_buffer.md#function-pixelbuffer)**(std::uint32_t width, std::uint32_t height, std::uint32_t bitspersample, ImageColorMode mode)<br>Construct a new pixel buffer.  |
| bool | **[CreateCopy](Classes/classtec_1_1_pixel_buffer.md#function-createcopy)**(const [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) & pbuf)<br>Create a copy of a pixel buffer.  |
| bool | **[CreateCopy](Classes/classtec_1_1_pixel_buffer.md#function-createcopy)**(const [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) & pbuf, std::uint32_t bitspersample, ImageColorMode mode)<br>Create a copy of a pixel buffer in a different format.  |
| bool | **[Create](Classes/classtec_1_1_pixel_buffer.md#function-create)**(uint32_t width, uint32_t height, uint32_t bitspersample, ImageColorMode mode)<br>Create a new pixel buffer, replacing the old buffer (if any).  |
| bool | **[IsDirty](Classes/classtec_1_1_pixel_buffer.md#function-isdirty)**() const |
| void | **[Invalidate](Classes/classtec_1_1_pixel_buffer.md#function-invalidate)**()<br>Mark dirty.  |
| void | **[Validate](Classes/classtec_1_1_pixel_buffer.md#function-validate)**()<br>Mark not dirty.  |
| const std::uint8_t * | **[GetBlockBase](Classes/classtec_1_1_pixel_buffer.md#function-getblockbase)**() const<br>Returns a pointer to the base address of image data for reading. This function is intended for loading pixel data from the buffer.  |
| std::mutex & | **[GetWritelock](Classes/classtec_1_1_pixel_buffer.md#function-getwritelock)**() |
| std::uint8_t * | **[GetPtr](Classes/classtec_1_1_pixel_buffer.md#function-getptr)**() |
| std::uint32_t | **[Width](Classes/classtec_1_1_pixel_buffer.md#function-width)**() const |
| std::uint32_t | **[Height](Classes/classtec_1_1_pixel_buffer.md#function-height)**() const |
| std::uint32_t | **[Pitch](Classes/classtec_1_1_pixel_buffer.md#function-pitch)**() const |
| std::uint32_t | **[PixelSize](Classes/classtec_1_1_pixel_buffer.md#function-pixelsize)**() const |
| ImageColorMode | **[GetFormat](Classes/classtec_1_1_pixel_buffer.md#function-getformat)**() const |
| void | **[PPMDebug](Classes/classtec_1_1_pixel_buffer.md#function-ppmdebug)**() |
| void | **[PPMDebug](Classes/classtec_1_1_pixel_buffer.md#function-ppmdebug)**(const char * ofile) |
| std::shared_ptr< [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) > | **[Create](Classes/classtec_1_1_pixel_buffer.md#function-create)**(const std::string name, const [FilePath](Classes/classtec_1_1_file_path.md) & filename =[FilePath](Classes/classtec_1_1_file_path.md)())<br>Factory method that creates a [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) and stores it in the PixelBufferMap under name. It will optionally load a texture file with the given filename.  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::uint32_t | **[image_x](Classes/classtec_1_1_pixel_buffer.md#variable-image_x)**  |
| std::uint32_t | **[image_y](Classes/classtec_1_1_pixel_buffer.md#variable-image_y)**  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::int32_t | **[imagewidth](Classes/classtec_1_1_pixel_buffer.md#variable-imagewidth)**  |
| std::int32_t | **[imageheight](Classes/classtec_1_1_pixel_buffer.md#variable-imageheight)**  |
| std::uint32_t | **[bufferpitch](Classes/classtec_1_1_pixel_buffer.md#variable-bufferpitch)** <br>number of bytes to move vertical 1 raster line  |
| std::uint32_t | **[imagepixelsize](Classes/classtec_1_1_pixel_buffer.md#variable-imagepixelsize)**  |
| std::uint32_t | **[imagebitdepth](Classes/classtec_1_1_pixel_buffer.md#variable-imagebitdepth)**  |
| ImageColorMode | **[imagemode](Classes/classtec_1_1_pixel_buffer.md#variable-imagemode)**  |
| bool | **[dirty](Classes/classtec_1_1_pixel_buffer.md#variable-dirty)**  |
| std::unique_ptr< std::uint8_t[]> | **[blockptr](Classes/classtec_1_1_pixel_buffer.md#variable-blockptr)**  |
| std::mutex | **[writelock](Classes/classtec_1_1_pixel_buffer.md#variable-writelock)**  |

## Public Functions Documentation

### function PixelBuffer

```cpp
PixelBuffer() =default
```


### function Load

```cpp
bool Load(
    const FilePath & filename
)
```

Returns a resource with the specified name. 

**Parameters**: 

  * **const** [FilePath](Classes/classtec_1_1_file_path.md) filename The filename of the image file to load. 


**Return**: bool True if initialization finished with no errors. 

### function PixelBuffer

```cpp
PixelBuffer(
    const PixelBuffer & 
)
```


### function operator=

```cpp
PixelBuffer & operator=(
    const PixelBuffer & 
)
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
    std::uint32_t bitspersample,
    ImageColorMode mode
)
```

Construct a new pixel buffer. 

**Parameters**: 

  * **width** the width of the buffer in pixels 
  * **height** the height of the buffer in pixels 
  * **bitspersample** the number of bits in a single channel, usually 8 
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
    std::uint32_t bitspersample,
    ImageColorMode mode
)
```

Create a copy of a pixel buffer in a different format. 

**Parameters**: 

  * **pbuf** the pixel buffer to copy from 
  * **bitspersample** the number of bits in a single channel, usually 8 
  * **mode** greyscale (Monochrome) or color (RGB), and whether there is alpha or no alpha 


**Return**: true on success 

### function Create

```cpp
bool Create(
    uint32_t width,
    uint32_t height,
    uint32_t bitspersample,
    ImageColorMode mode
)
```

Create a new pixel buffer, replacing the old buffer (if any). 

**Parameters**: 

  * **width** the width of the buffer in pixels 
  * **height** the height of the buffer in pixels 
  * **bitspersample** the number of bits in a single channel, usually 8 
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
    const FilePath & filename =FilePath()
)
```

Factory method that creates a [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) and stores it in the PixelBufferMap under name. It will optionally load a texture file with the given filename. 

**Parameters**: 

  * **const** std::string name The name to store the [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md) under. 
  * **const** [FilePath](Classes/classtec_1_1_file_path.md) filename The optional filename of the image to load. 


**Return**: std::shared_ptr<PixelBuffer> The created [PixelBuffer](Classes/classtec_1_1_pixel_buffer.md). 

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


### variable imagebitdepth

```cpp
std::uint32_t imagebitdepth {0};
```


### variable imagemode

```cpp
ImageColorMode imagemode {ImageColorMode::MONOCHROME};
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

Updated on 17 March 2021 at 23:08:11 UTC