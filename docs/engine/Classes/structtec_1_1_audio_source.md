---
title: tec::AudioSource

---

# tec::AudioSource



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[AudioSource](/engine/Classes/structtec_1_1_audio_source/#function-audiosource)**(std::shared_ptr< [VorbisStream](/engine/Classes/classtec_1_1_vorbis_stream/) > stream, bool auto_play) |
| | **[AudioSource](/engine/Classes/structtec_1_1_audio_source/#function-audiosource)**() =default |
| void | **[Out](/engine/Classes/structtec_1_1_audio_source/#function-out)**(proto::Component * target) const |
| void | **[In](/engine/Classes/structtec_1_1_audio_source/#function-in)**(const proto::Component & _source) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| ALuint | **[source](/engine/Classes/structtec_1_1_audio_source/#variable-source)**  |
| ALuint | **[buffer](/engine/Classes/structtec_1_1_audio_source/#variable-buffer)**  |
| bool | **[looping](/engine/Classes/structtec_1_1_audio_source/#variable-looping)**  |
| std::shared_ptr< [VorbisStream](/engine/Classes/classtec_1_1_vorbis_stream/) > | **[vorbis_stream](/engine/Classes/structtec_1_1_audio_source/#variable-vorbis_stream)**  |
| AUDIOSOURCE_STATE | **[source_state](/engine/Classes/structtec_1_1_audio_source/#variable-source_state)**  |
| int | **[gain](/engine/Classes/structtec_1_1_audio_source/#variable-gain)**  |
| std::string | **[audio_name](/engine/Classes/structtec_1_1_audio_source/#variable-audio_name)**  |

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

Updated on 17 March 2021 at 22:09:29 UTC