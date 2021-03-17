---
title: tec::networking::Message

---

# tec::networking::Message



## Public Types

|                | Name           |
| -------------- | -------------- |
| enum| **[@2](Classes/classtec_1_1networking_1_1_message.md#enum-@2)** { header_length = 16} |
| enum| **[@3](Classes/classtec_1_1networking_1_1_message.md#enum-@3)** { max_body_length = 1024} |
| typedef const [Message](Classes/classtec_1_1networking_1_1_message.md) * | **[cptr_type](Classes/classtec_1_1networking_1_1_message.md#typedef-cptr_type)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Message](Classes/classtec_1_1networking_1_1_message.md#function-message)**() |
| const uint8_t * | **[GetDataPTR](Classes/classtec_1_1networking_1_1_message.md#function-getdataptr)**() const |
| uint8_t * | **[GetDataPTR](Classes/classtec_1_1networking_1_1_message.md#function-getdataptr)**() |
| std::size_t | **[length](Classes/classtec_1_1networking_1_1_message.md#function-length)**() const |
| asio::mutable_buffer | **[buffer_header](Classes/classtec_1_1networking_1_1_message.md#function-buffer_header)**() |
| asio::mutable_buffer | **[buffer_body](Classes/classtec_1_1networking_1_1_message.md#function-buffer_body)**() |
| asio::const_buffer | **[buffer](Classes/classtec_1_1networking_1_1_message.md#function-buffer)**() const |
| const char * | **[GetBodyPTR](Classes/classtec_1_1networking_1_1_message.md#function-getbodyptr)**() const |
| char * | **[GetBodyPTR](Classes/classtec_1_1networking_1_1_message.md#function-getbodyptr)**() |
| std::size_t | **[GetBodyLength](Classes/classtec_1_1networking_1_1_message.md#function-getbodylength)**() const |
| void | **[SetSequence](Classes/classtec_1_1networking_1_1_message.md#function-setsequence)**(uint32_t value) |
| uint32_t | **[GetSequence](Classes/classtec_1_1networking_1_1_message.md#function-getsequence)**() const |
| void | **[SetMessageType](Classes/classtec_1_1networking_1_1_message.md#function-setmessagetype)**(MessageType value) |
| MessageType | **[GetMessageType](Classes/classtec_1_1networking_1_1_message.md#function-getmessagetype)**() const |
| void | **[SetMessageID](Classes/classtec_1_1networking_1_1_message.md#function-setmessageid)**(uint32_t value) |
| uint32_t | **[GetMessageID](Classes/classtec_1_1networking_1_1_message.md#function-getmessageid)**() const |
| void | **[SetBodyLength](Classes/classtec_1_1networking_1_1_message.md#function-setbodylength)**(std::size_t new_length) |
| bool | **[decode_header](Classes/classtec_1_1networking_1_1_message.md#function-decode_header)**() |
| void | **[encode_header](Classes/classtec_1_1networking_1_1_message.md#function-encode_header)**() |

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

Updated on 17 March 2021 at 23:08:11 UTC