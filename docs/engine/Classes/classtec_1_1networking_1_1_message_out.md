---
title: tec::networking::MessageOut

---

# tec::networking::MessageOut



Inherits from google::protobuf::io::ZeroCopyOutputStream

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[MessageOut](Classes/classtec_1_1networking_1_1_message_out.md#function-messageout)**() |
| | **[MessageOut](Classes/classtec_1_1networking_1_1_message_out.md#function-messageout)**(MessageType msg_type) |
| | **[MessageOut](Classes/classtec_1_1networking_1_1_message_out.md#function-messageout)**(const [MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) & other) |
| | **[MessageOut](Classes/classtec_1_1networking_1_1_message_out.md#function-messageout)**([MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) && other) |
| void | **[FromBuffer](Classes/classtec_1_1networking_1_1_message_out.md#function-frombuffer)**(const void * body, size_t length) |
| void | **[FromString](Classes/classtec_1_1networking_1_1_message_out.md#function-fromstring)**(const std::string & body) |
| void | **[SetMessageType](Classes/classtec_1_1networking_1_1_message_out.md#function-setmessagetype)**(MessageType value) |
| void | **[SetMessageID](Classes/classtec_1_1networking_1_1_message_out.md#function-setmessageid)**(uint32_t value) |
| MessageType | **[GetMessageType](Classes/classtec_1_1networking_1_1_message_out.md#function-getmessagetype)**() const |
| uint32_t | **[GetMessageID](Classes/classtec_1_1networking_1_1_message_out.md#function-getmessageid)**() const |
| bool | **[IsEmpty](Classes/classtec_1_1networking_1_1_message_out.md#function-isempty)**() const |
| MessagePool::list_type | **[GetMessages](Classes/classtec_1_1networking_1_1_message_out.md#function-getmessages)**() |
| virtual bool | **[Next](Classes/classtec_1_1networking_1_1_message_out.md#function-next)**(void ** data, int * size) |
| virtual void | **[BackUp](Classes/classtec_1_1networking_1_1_message_out.md#function-backup)**(int count) |
| virtual int64_t | **[ByteCount](Classes/classtec_1_1networking_1_1_message_out.md#function-bytecount)**() const |
| virtual bool | **[WriteAliasedRaw](Classes/classtec_1_1networking_1_1_message_out.md#function-writealiasedraw)**(const void * , int ) |
| virtual bool | **[AllowsAliasing](Classes/classtec_1_1networking_1_1_message_out.md#function-allowsaliasing)**() const |

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

Updated on 17 March 2021 at 23:08:11 UTC