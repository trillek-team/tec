---
title: tec::VorbisStream

---

# tec::VorbisStream





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[VorbisStream](/engine/Classes/classtec_1_1_vorbis_stream/#function-vorbisstream)**(unsigned buffer_size =32768) |
| | **[~VorbisStream](/engine/Classes/classtec_1_1_vorbis_stream/#function-~vorbisstream)**() |
| std::size_t | **[BufferStream](/engine/Classes/classtec_1_1_vorbis_stream/#function-bufferstream)**(ALint buffer) |
| void | **[Reset](/engine/Classes/classtec_1_1_vorbis_stream/#function-reset)**() |
| const std::string | **[GetName](/engine/Classes/classtec_1_1_vorbis_stream/#function-getname)**() const |
| void | **[SetName](/engine/Classes/classtec_1_1_vorbis_stream/#function-setname)**(const std::string & _name) |
| std::shared_ptr< [VorbisStream](/engine/Classes/classtec_1_1_vorbis_stream/) > | **[Create](/engine/Classes/classtec_1_1_vorbis_stream/#function-create)**(const [Path](/engine/Classes/classtec_1_1_path/) & filename)<br>Creates a [VorbisStream](/engine/Classes/classtec_1_1_vorbis_stream/) resource from the given filename.  |

## Public Functions Documentation

### function VorbisStream

```cpp
VorbisStream(
    unsigned buffer_size =32768
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
    const Path & filename
)
```

Creates a [VorbisStream](/engine/Classes/classtec_1_1_vorbis_stream/) resource from the given filename. 

**Parameters**: 

  * **const** std::vector<Property>& properties The creation properties for the resource. 


**Return**: std::shared_ptr<VorbisStream> The created [VorbisStream](/engine/Classes/classtec_1_1_vorbis_stream/) resource. 

The only used initialization property is "filename". 


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000