---
title: tec::AudioSource

---

# tec::AudioSource



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[AudioSource](Classes/structtec_1_1_audio_source.md#function-audiosource)**(std::shared_ptr< [VorbisStream](Classes/classtec_1_1_vorbis_stream.md) > stream, bool auto_play) |
| | **[AudioSource](Classes/structtec_1_1_audio_source.md#function-audiosource)**() =default |
| void | **[Out](Classes/structtec_1_1_audio_source.md#function-out)**(proto::Component * target) const |
| void | **[In](Classes/structtec_1_1_audio_source.md#function-in)**(const proto::Component & _source) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| ALuint | **[source](Classes/structtec_1_1_audio_source.md#variable-source)**  |
| ALuint | **[buffer](Classes/structtec_1_1_audio_source.md#variable-buffer)**  |
| bool | **[looping](Classes/structtec_1_1_audio_source.md#variable-looping)**  |
| std::shared_ptr< [VorbisStream](Classes/classtec_1_1_vorbis_stream.md) > | **[vorbis_stream](Classes/structtec_1_1_audio_source.md#variable-vorbis_stream)**  |
| AUDIOSOURCE_STATE | **[source_state](Classes/structtec_1_1_audio_source.md#variable-source_state)**  |
| int | **[gain](Classes/structtec_1_1_audio_source.md#variable-gain)**  |
| std::string | **[audio_name](Classes/structtec_1_1_audio_source.md#variable-audio_name)**  |

## Public Functions Documentation

### function AudioSource

```cpp
inline AudioSource(
    std::shared_ptr< VorbisStream > stream,
    bool auto_play
)
```


### function AudioSource

```cpp
AudioSource() =default
```


### function Out

```cpp
inline void Out(
    proto::Component * target
) const
```


### function In

```cpp
inline void In(
    const proto::Component & _source
)
```


## Public Attributes Documentation

### variable source

```cpp
ALuint source {0};
```


### variable buffer

```cpp
ALuint buffer {0, 0};
```


### variable looping

```cpp
bool looping {false};
```


### variable vorbis_stream

```cpp
std::shared_ptr< VorbisStream > vorbis_stream;
```


### variable source_state

```cpp
AUDIOSOURCE_STATE source_state {AUDIOSOURCE_STATE::PAUSED};
```


### variable gain

```cpp
int gain {100};
```


### variable audio_name

```cpp
std::string audio_name;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC