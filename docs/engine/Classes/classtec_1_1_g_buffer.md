---
title: tec::GBuffer

---

# tec::GBuffer





## Public Types

|                | Name           |
| -------------- | -------------- |
| enum class int | **[TEXTURE_TYPE](/engine/Classes/classtec_1_1_g_buffer/#enum-texture-type)** { POSITION = 0, DIFFUSE, NORMAL, EMISSION, TEXTURE_TYPE_LAST} |
| enum class int | **[DEPTH_TYPE](/engine/Classes/classtec_1_1_g_buffer/#enum-depth-type)** { DEPTH = (int)TEXTURE_TYPE::TEXTURE_TYPE_LAST, STENCIL} |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[GBuffer](/engine/Classes/classtec_1_1_g_buffer/#function-gbuffer)**() |
| | **[~GBuffer](/engine/Classes/classtec_1_1_g_buffer/#function-~gbuffer)**() |
| void | **[AddColorAttachments](/engine/Classes/classtec_1_1_g_buffer/#function-addcolorattachments)**(const unsigned int window_width, const unsigned int window_height) |
| void | **[ResizeColorAttachments](/engine/Classes/classtec_1_1_g_buffer/#function-resizecolorattachments)**(const unsigned int window_width, const unsigned int window_height) |
| void | **[SetDepthAttachment](/engine/Classes/classtec_1_1_g_buffer/#function-setdepthattachment)**(DEPTH_TYPE type, const unsigned int width, const unsigned int height) |
| void | **[ResizeDepthAttachment](/engine/Classes/classtec_1_1_g_buffer/#function-resizedepthattachment)**(const unsigned int width, const unsigned int height) |
| bool | **[CheckCompletion](/engine/Classes/classtec_1_1_g_buffer/#function-checkcompletion)**() const |
| void | **[StartFrame](/engine/Classes/classtec_1_1_g_buffer/#function-startframe)**() const |
| void | **[BeginGeometryPass](/engine/Classes/classtec_1_1_g_buffer/#function-begingeometrypass)**() const |
| void | **[BeginLightPass](/engine/Classes/classtec_1_1_g_buffer/#function-beginlightpass)**() |
| void | **[BeginDirLightPass](/engine/Classes/classtec_1_1_g_buffer/#function-begindirlightpass)**() |
| void | **[BeginPointLightPass](/engine/Classes/classtec_1_1_g_buffer/#function-beginpointlightpass)**() |
| void | **[EndPointLightPass](/engine/Classes/classtec_1_1_g_buffer/#function-endpointlightpass)**() |
| void | **[FinalPass](/engine/Classes/classtec_1_1_g_buffer/#function-finalpass)**() const |
| void | **[BindForWriting](/engine/Classes/classtec_1_1_g_buffer/#function-bindforwriting)**() const |
| void | **[BindForRendering](/engine/Classes/classtec_1_1_g_buffer/#function-bindforrendering)**() const |
| GLuint | **[GetDepthTexture](/engine/Classes/classtec_1_1_g_buffer/#function-getdepthtexture)**() const |
| GLuint | **[GetColorTexture](/engine/Classes/classtec_1_1_g_buffer/#function-getcolortexture)**(unsigned short index) const |
| void | **[EndGeometryPass](/engine/Classes/classtec_1_1_g_buffer/#function-endgeometrypass)**() |
| void | **[StencilPass](/engine/Classes/classtec_1_1_g_buffer/#function-stencilpass)**() |
| void | **[SetReadBuffer](/engine/Classes/classtec_1_1_g_buffer/#function-setreadbuffer)**(TEXTURE_TYPE TextureType) |

## Public Types Documentation

### enum TEXTURE_TYPE

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| POSITION | 0|   |
| DIFFUSE | |   |
| NORMAL | |   |
| EMISSION | |   |
| TEXTURE_TYPE_LAST | |   |




### enum DEPTH_TYPE

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| DEPTH | (int)TEXTURE_TYPE::TEXTURE_TYPE_LAST|   |
| STENCIL | |   |




## Public Functions Documentation

### function GBuffer

```cpp
inline GBuffer()
```


### function ~GBuffer

```cpp
inline ~GBuffer()
```


### function AddColorAttachments

```cpp
void AddColorAttachments(
    const unsigned int window_width,
    const unsigned int window_height
)
```


### function ResizeColorAttachments

```cpp
void ResizeColorAttachments(
    const unsigned int window_width,
    const unsigned int window_height
)
```


### function SetDepthAttachment

```cpp
void SetDepthAttachment(
    DEPTH_TYPE type,
    const unsigned int width,
    const unsigned int height
)
```


### function ResizeDepthAttachment

```cpp
void ResizeDepthAttachment(
    const unsigned int width,
    const unsigned int height
)
```


### function CheckCompletion

```cpp
bool CheckCompletion() const
```


### function StartFrame

```cpp
void StartFrame() const
```


### function BeginGeometryPass

```cpp
void BeginGeometryPass() const
```


### function BeginLightPass

```cpp
void BeginLightPass()
```


### function BeginDirLightPass

```cpp
void BeginDirLightPass()
```


### function BeginPointLightPass

```cpp
void BeginPointLightPass()
```


### function EndPointLightPass

```cpp
void EndPointLightPass()
```


### function FinalPass

```cpp
void FinalPass() const
```


### function BindForWriting

```cpp
void BindForWriting() const
```


### function BindForRendering

```cpp
void BindForRendering() const
```


### function GetDepthTexture

```cpp
inline GLuint GetDepthTexture() const
```


### function GetColorTexture

```cpp
inline GLuint GetColorTexture(
    unsigned short index
) const
```


### function EndGeometryPass

```cpp
static void EndGeometryPass()
```


### function StencilPass

```cpp
static void StencilPass()
```


### function SetReadBuffer

```cpp
static void SetReadBuffer(
    TEXTURE_TYPE TextureType
)
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000