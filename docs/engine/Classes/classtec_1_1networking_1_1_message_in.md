---
title: tec::networking::MessageIn

---

# tec::networking::MessageIn



Inherits from google::protobuf::io::ZeroCopyInputStream

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[MessageIn](Classes/classtec_1_1networking_1_1_message_in.md#function-messagein)**() |
| | **[MessageIn](Classes/classtec_1_1networking_1_1_message_in.md#function-messagein)**(MessageType msg_type) |
| void | **[ReadBuffer](Classes/classtec_1_1networking_1_1_message_in.md#function-readbuffer)**(void * body, size_t length) |
| void | **[ReadString](Classes/classtec_1_1networking_1_1_message_in.md#function-readstring)**(std::string & body) |
| std::string | **[ToString](Classes/classtec_1_1networking_1_1_message_in.md#function-tostring)**() |
| void | **[SetMessageType](Classes/classtec_1_1networking_1_1_message_in.md#function-setmessagetype)**(MessageType value) |
| void | **[SetMessageID](Classes/classtec_1_1networking_1_1_message_in.md#function-setmessageid)**(uint32_t value) |
| MessageType | **[GetMessageType](Classes/classtec_1_1networking_1_1_message_in.md#function-getmessagetype)**() const |
| uint32_t | **[GetMessageID](Classes/classtec_1_1networking_1_1_message_in.md#function-getmessageid)**() const |
| bool | **[IsEmpty](Classes/classtec_1_1networking_1_1_message_in.md#function-isempty)**() const |
| size_t | **[GetSize](Classes/classtec_1_1networking_1_1_message_in.md#function-getsize)**() const |
| bool | **[PushMessage](Classes/classtec_1_1networking_1_1_message_in.md#function-pushmessage)**(MessagePool::ptr_type msg) |
| bool | **[DecodeMessages](Classes/classtec_1_1networking_1_1_message_in.md#function-decodemessages)**() |
| bool | **[AssignMessages](Classes/classtec_1_1networking_1_1_message_in.md#function-assignmessages)**(MessagePool::list_type msgs) |
| [MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) | **[ToOut](Classes/classtec_1_1networking_1_1_message_in.md#function-toout)**() const |
| void | **[Reset](Classes/classtec_1_1networking_1_1_message_in.md#function-reset)**() |
| virtual bool | **[Next](Classes/classtec_1_1networking_1_1_message_in.md#function-next)**(const void ** data, int * size) |
| virtual void | **[BackUp](Classes/classtec_1_1networking_1_1_message_in.md#function-backup)**(int count) |
| virtual bool | **[Skip](Classes/classtec_1_1networking_1_1_message_in.md#function-skip)**(int count) |
| virtual int64_t | **[ByteCount](Classes/classtec_1_1networking_1_1_message_in.md#function-bytecount)**() const |

## Public Functions Documentation

### function MessageIn

```cpp
inline MessageIn()
```


### function MessageIn

```cpp
inline MessageIn(
    MessageType msg_type
)
```


### function ReadBuffer

```cpp
void ReadBuffer(
    void * body,
    size_t length
)
```


### function ReadString

```cpp
void ReadString(
    std::string & body
)
```


### function ToString

```cpp
inline std::string ToString()
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


### function GetSize

```cpp
size_t GetSize() const
```


### function PushMessage

```cpp
bool PushMessage(
    MessagePool::ptr_type msg
)
```


### function DecodeMessages

```cpp
bool DecodeMessages()
```


### function AssignMessages

```cpp
bool AssignMessages(
    MessagePool::list_type msgs
)
```


### function ToOut

```cpp
MessageOut ToOut() const
```


### function Reset

```cpp
inline void Reset()
```


### function Next

```cpp
virtual bool Next(
    const void ** data,
    int * size
)
```


### function BackUp

```cpp
virtual void BackUp(
    int count
)
```


### function Skip

```cpp
virtual bool Skip(
    int count
)
```


### function ByteCount

```cpp
inline virtual int64_t ByteCount() const
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC