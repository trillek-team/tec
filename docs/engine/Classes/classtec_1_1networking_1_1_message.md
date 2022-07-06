---
title: tec::networking::Message

---

# tec::networking::Message





## Public Types

|                | Name           |
| -------------- | -------------- |
| enum| **[@2](/engine/Classes/classtec_1_1networking_1_1_message/#enum-@2)** { header_length = 16} |
| enum| **[@3](/engine/Classes/classtec_1_1networking_1_1_message/#enum-@3)** { max_body_length = 1024} |
| typedef const [Message](/engine/Classes/classtec_1_1networking_1_1_message/) * | **[cptr_type](/engine/Classes/classtec_1_1networking_1_1_message/#typedef-cptr-type)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Message](/engine/Classes/classtec_1_1networking_1_1_message/#function-message)**() |
| const uint8_t * | **[GetDataPTR](/engine/Classes/classtec_1_1networking_1_1_message/#function-getdataptr)**() const |
| uint8_t * | **[GetDataPTR](/engine/Classes/classtec_1_1networking_1_1_message/#function-getdataptr)**() |
| std::size_t | **[length](/engine/Classes/classtec_1_1networking_1_1_message/#function-length)**() const |
| asio::mutable_buffer | **[buffer_header](/engine/Classes/classtec_1_1networking_1_1_message/#function-buffer-header)**() |
| asio::mutable_buffer | **[buffer_body](/engine/Classes/classtec_1_1networking_1_1_message/#function-buffer-body)**() |
| asio::const_buffer | **[buffer](/engine/Classes/classtec_1_1networking_1_1_message/#function-buffer)**() const |
| const char * | **[GetBodyPTR](/engine/Classes/classtec_1_1networking_1_1_message/#function-getbodyptr)**() const |
| char * | **[GetBodyPTR](/engine/Classes/classtec_1_1networking_1_1_message/#function-getbodyptr)**() |
| std::size_t | **[GetBodyLength](/engine/Classes/classtec_1_1networking_1_1_message/#function-getbodylength)**() const |
| void | **[SetSequence](/engine/Classes/classtec_1_1networking_1_1_message/#function-setsequence)**(uint32_t value) |
| uint32_t | **[GetSequence](/engine/Classes/classtec_1_1networking_1_1_message/#function-getsequence)**() const |
| void | **[SetMessageType](/engine/Classes/classtec_1_1networking_1_1_message/#function-setmessagetype)**(MessageType value) |
| MessageType | **[GetMessageType](/engine/Classes/classtec_1_1networking_1_1_message/#function-getmessagetype)**() const |
| void | **[SetMessageID](/engine/Classes/classtec_1_1networking_1_1_message/#function-setmessageid)**(uint32_t value) |
| uint32_t | **[GetMessageID](/engine/Classes/classtec_1_1networking_1_1_message/#function-getmessageid)**() const |
| void | **[SetBodyLength](/engine/Classes/classtec_1_1networking_1_1_message/#function-setbodylength)**(std::size_t new_length) |
| bool | **[decode_header](/engine/Classes/classtec_1_1networking_1_1_message/#function-decode-header)**() |
| void | **[encode_header](/engine/Classes/classtec_1_1networking_1_1_message/#function-encode-header)**() |

## Public Types Documentation

### enum @2

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| header_length | 16|   |




### enum @3

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| max_body_length | 1024|   |




### typedef cptr_type

```cpp
typedef const Message* tec::networking::Message::cptr_type;
```


## Public Functions Documentation

### function Message

```cpp
inline Message()
```


### function GetDataPTR

```cpp
inline const uint8_t * GetDataPTR() const
```


### function GetDataPTR

```cpp
inline uint8_t * GetDataPTR()
```


### function length

```cpp
inline std::size_t length() const
```


### function buffer_header

```cpp
inline asio::mutable_buffer buffer_header()
```


### function buffer_body

```cpp
inline asio::mutable_buffer buffer_body()
```


### function buffer

```cpp
inline asio::const_buffer buffer() const
```


### function GetBodyPTR

```cpp
inline const char * GetBodyPTR() const
```


### function GetBodyPTR

```cpp
inline char * GetBodyPTR()
```


### function GetBodyLength

```cpp
inline std::size_t GetBodyLength() const
```


### function SetSequence

```cpp
inline void SetSequence(
    uint32_t value
)
```


### function GetSequence

```cpp
inline uint32_t GetSequence() const
```


### function SetMessageType

```cpp
inline void SetMessageType(
    MessageType value
)
```


### function GetMessageType

```cpp
inline MessageType GetMessageType() const
```


### function SetMessageID

```cpp
inline void SetMessageID(
    uint32_t value
)
```


### function GetMessageID

```cpp
inline uint32_t GetMessageID() const
```


### function SetBodyLength

```cpp
inline void SetBodyLength(
    std::size_t new_length
)
```


### function decode_header

```cpp
inline bool decode_header()
```


### function encode_header

```cpp
inline void encode_header()
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000