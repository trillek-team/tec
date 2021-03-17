---
title: tec::GBuffer

---

# tec::GBuffer



## Public Types

|                | Name           |
| -------------- | -------------- |
| enum int | **[GBUFFER_TEXTURE_TYPE](Classes/classtec_1_1_g_buffer.md#enum-gbuffer_texture_type)** { GBUFFER_TEXTURE_TYPE_POSITION = 0, GBUFFER_TEXTURE_TYPE_DIFFUSE, GBUFFER_TEXTURE_TYPE_NORMAL} |
| enum| **[GBUFFER_DEPTH_TYPE](Classes/classtec_1_1_g_buffer.md#enum-gbuffer_depth_type)** { GBUFFER_DEPTH_TYPE_STENCIL} |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[GBuffer](Classes/classtec_1_1_g_buffer.md#function-gbuffer)**() |
| | **[~GBuffer](Classes/classtec_1_1_g_buffer.md#function-~gbuffer)**() |
| void | **[AddColorAttachments](Classes/classtec_1_1_g_buffer.md#function-addcolorattachments)**(const unsigned int window_width, const unsigned int window_height) |
| void | **[ResizeColorAttachments](Classes/classtec_1_1_g_buffer.md#function-resizecolorattachments)**(const unsigned int window_width, const unsigned int window_height) |
| void | **[SetDepthAttachment](Classes/classtec_1_1_g_buffer.md#function-setdepthattachment)**(GBUFFER_DEPTH_TYPE type, const unsigned int width, const unsigned int height) |
| void | **[ResizeDepthAttachment](Classes/classtec_1_1_g_buffer.md#function-resizedepthattachment)**(const unsigned int width, const unsigned int height) |
| bool | **[CheckCompletion](Classes/classtec_1_1_g_buffer.md#function-checkcompletion)**() const |
| void | **[StartFrame](Classes/classtec_1_1_g_buffer.md#function-startframe)**() const |
| void | **[BeginGeometryPass](Classes/classtec_1_1_g_buffer.md#function-begingeometrypass)**() const |
| void | **[BeginLightPass](Classes/classtec_1_1_g_buffer.md#function-beginlightpass)**() |
| void | **[BeginDirLightPass](Classes/classtec_1_1_g_buffer.md#function-begindirlightpass)**() |
| void | **[BeginPointLightPass](Classes/classtec_1_1_g_buffer.md#function-beginpointlightpass)**() |
| void | **[EndPointLightPass](Classes/classtec_1_1_g_buffer.md#function-endpointlightpass)**() |
| void | **[FinalPass](Classes/classtec_1_1_g_buffer.md#function-finalpass)**() const |
| void | **[BindForWriting](Classes/classtec_1_1_g_buffer.md#function-bindforwriting)**() const |
| void | **[BindForRendering](Classes/classtec_1_1_g_buffer.md#function-bindforrendering)**() const |
| GLuint | **[GetDepthTexture](Classes/classtec_1_1_g_buffer.md#function-getdepthtexture)**() const |
| GLuint | **[GetColorTexture](Classes/classtec_1_1_g_buffer.md#function-getcolortexture)**(unsigned short index) const |
| void | **[EndGeometryPass](Classes/classtec_1_1_g_buffer.md#function-endgeometrypass)**() |
| void | **[StencilPass](Classes/classtec_1_1_g_buffer.md#function-stencilpass)**() |
| void | **[SetReadBuffer](Classes/classtec_1_1_g_buffer.md#function-setreadbuffer)**(GBUFFER_TEXTURE_TYPE TextureType) |

## Public Types Documentation

### enum GBUFFER_TEXTURE_TYPE

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| GBUFFER_TEXTURE_TYPE_POSITION | 0|   |
| GBUFFER_TEXTURE_TYPE_DIFFUSE | |   |
| GBUFFER_TEXTURE_TYPE_NORMAL | |   |




### enum GBUFFER_DEPTH_TYPE

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| GBUFFER_DEPTH_TYPE_STENCIL | |   |




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
    GBUFFER_DEPTH_TYPE type,
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
    GBUFFER_TEXTURE_TYPE TextureType
)
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC