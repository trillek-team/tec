---
title: tec::networking::MessageOut

---

# tec::networking::MessageOut





Inherits from google::protobuf::io::ZeroCopyOutputStream

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-messageout)**() |
| | **[MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-messageout)**(MessageType msg_type) |
| | **[MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-messageout)**(const [MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) & other) |
| | **[MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-messageout)**([MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) && other) |
| void | **[FromBuffer](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-frombuffer)**(const void * body, size_t length) |
| void | **[FromString](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-fromstring)**(const std::string & body) |
| void | **[SetMessageType](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-setmessagetype)**(MessageType value) |
| void | **[SetMessageID](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-setmessageid)**(uint32_t value) |
| MessageType | **[GetMessageType](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-getmessagetype)**() const |
| uint32_t | **[GetMessageID](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-getmessageid)**() const |
| bool | **[IsEmpty](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-isempty)**() const |
| MessagePool::list_type | **[GetMessages](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-getmessages)**() |
| virtual bool | **[Next](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-next)**(void ** data, int * size) |
| virtual void | **[BackUp](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-backup)**(int count) |
| virtual int64_t | **[ByteCount](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-bytecount)**() const |
| virtual bool | **[WriteAliasedRaw](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-writealiasedraw)**(const void * , int ) |
| virtual bool | **[AllowsAliasing](/engine/Classes/classtec_1_1networking_1_1_message_out/#function-allowsaliasing)**() const |

## Public Functions Documentation

### function MessageOut

```cpp
inline MessageOut()
```


### function MessageOut

```cpp
inline MessageOut(
    MessageType msg_type
)
```


### function MessageOut

```cpp
inline MessageOut(
    const MessageOut & other
)
```


### function MessageOut

```cpp
inline MessageOut(
    MessageOut && other
)
```


### function FromBuffer

```cpp
void FromBuffer(
    const void * body,
    size_t length
)
```


### function FromString

```cpp
inline void FromString(
    const std::string & body
)
```


### function SetMessageType

```cpp
inline void SetMessageType(
    MessageType value
)
```


### function SetMessageID

```cpp
inline void SetMessageID(
    uint32_t value
)
```


### function GetMessageType

```cpp
inline MessageType GetMessageType() const
```


### function GetMessageID

```cpp
inline uint32_t GetMessageID() const
```


### function IsEmpty

```cpp
inline bool IsEmpty() const
```


### function GetMessages

```cpp
MessagePool::list_type GetMessages()
```


### function Next

```cpp
virtual bool Next(
    void ** data,
    int * size
)
```


### function BackUp

```cpp
virtual void BackUp(
    int count
)
```


### function ByteCount

```cpp
inline virtual int64_t ByteCount() const
```


### function WriteAliasedRaw

```cpp
inline virtual bool WriteAliasedRaw(
    const void * ,
    int 
)
```


### function AllowsAliasing

```cpp
inline virtual bool AllowsAliasing() const
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000