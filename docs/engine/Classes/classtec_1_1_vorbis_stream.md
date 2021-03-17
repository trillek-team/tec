---
title: tec::VorbisStream

---

# tec::VorbisStream



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[VorbisStream](Classes/classtec_1_1_vorbis_stream.md#function-vorbisstream)**(int buffer_size =32768) |
| | **[~VorbisStream](Classes/classtec_1_1_vorbis_stream.md#function-~vorbisstream)**() |
| std::size_t | **[BufferStream](Classes/classtec_1_1_vorbis_stream.md#function-bufferstream)**(ALint buffer) |
| void | **[Reset](Classes/classtec_1_1_vorbis_stream.md#function-reset)**() |
| const std::string | **[GetName](Classes/classtec_1_1_vorbis_stream.md#function-getname)**() const |
| void | **[SetName](Classes/classtec_1_1_vorbis_stream.md#function-setname)**(const std::string & _name) |
| std::shared_ptr< [VorbisStream](Classes/classtec_1_1_vorbis_stream.md) > | **[Create](Classes/classtec_1_1_vorbis_stream.md#function-create)**(const [FilePath](Classes/classtec_1_1_file_path.md) & filename)<br>Creates a [VorbisStream](Classes/classtec_1_1_vorbis_stream.md) resource from the given filename.  |

## Public Functions Documentation

### function VorbisStream

```cpp
VorbisStream(
    int buffer_size =32768
)
```


### function ~VorbisStream

```cpp
~VorbisStream()
```


### function BufferStream

```cpp
std::size_t BufferStream(
    ALint buffer
)
```


### function Reset

```cpp
void Reset()
```


### function GetName

```cpp
inline const std::string GetName() const
```


### function SetName

```cpp
inline void SetName(
    const std::string & _name
)
```


### function Create

```cpp
static std::shared_ptr< VorbisStream > Create(
    const FilePath & filename
)
```

Creates a [VorbisStream](Classes/classtec_1_1_vorbis_stream.md) resource from the given filename. 

**Parameters**: 

  * **const** std::vector<Property>& properties The creation properties for the resource. 


**Return**: std::shared_ptr<VorbisStream> The created [VorbisStream](Classes/classtec_1_1_vorbis_stream.md) resource. 

The only used initialization property is "filename". 


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC