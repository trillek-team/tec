---
title: tec::networking::MessageIn

---

# tec::networking::MessageIn





Inherits from google::protobuf::io::ZeroCopyInputStream

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[MessageIn](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-messagein)**() |
| | **[MessageIn](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-messagein)**(MessageType msg_type) |
| void | **[ReadBuffer](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-readbuffer)**(void * body, size_t length) |
| void | **[ReadString](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-readstring)**(std::string & body) |
| std::string | **[ToString](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-tostring)**() |
| void | **[SetMessageType](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-setmessagetype)**(MessageType value) |
| void | **[SetMessageID](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-setmessageid)**(uint32_t value) |
| MessageType | **[GetMessageType](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-getmessagetype)**() const |
| uint32_t | **[GetMessageID](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-getmessageid)**() const |
| bool | **[IsEmpty](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-isempty)**() const |
| size_t | **[GetSize](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-getsize)**() const |
| bool | **[PushMessage](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-pushmessage)**(MessagePool::ptr_type msg) |
| bool | **[DecodeMessages](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-decodemessages)**() |
| bool | **[AssignMessages](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-assignmessages)**(MessagePool::list_type msgs) |
| [MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) | **[ToOut](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-toout)**() const |
| void | **[Reset](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-reset)**() |
| virtual bool | **[Next](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-next)**(const void ** data, int * size) |
| virtual void | **[BackUp](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-backup)**(int count) |
| virtual bool | **[Skip](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-skip)**(int count) |
| virtual int64_t | **[ByteCount](/engine/Classes/classtec_1_1networking_1_1_message_in/#function-bytecount)**() const |

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

Updated on 2022-07-06 at 04:15:07 +0000