---
title: tec::networking

---

# tec::networking

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[tec::networking::ServerConnection](/engine/Classes/classtec_1_1networking_1_1_server_connection/)**  |
| class | **[tec::networking::ClientConnection](/engine/Classes/classtec_1_1networking_1_1_client_connection/)**  |
| struct | **[tec::networking::client_connection_info](/engine/Classes/structtec_1_1networking_1_1client__connection__info/)**  |
| class | **[tec::networking::Server](/engine/Classes/classtec_1_1networking_1_1_server/)**  |
| class | **[tec::networking::Message](/engine/Classes/classtec_1_1networking_1_1_message/)**  |
| class | **[tec::networking::MessagePool](/engine/Classes/classtec_1_1networking_1_1_message_pool/)**  |
| class | **[tec::networking::MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/)**  |
| class | **[tec::networking::MessageIn](/engine/Classes/classtec_1_1networking_1_1_message_in/)**  |

## Types

|                | Name           |
| -------------- | -------------- |
| enum int | **[MessageType](/engine/Namespaces/namespacetec_1_1networking/#enum-messagetype)** { SYNC, MULTI_PART, CLIENT_COMMAND, ENTITY_CREATE, ENTITY_DESTROY, CLIENT_JOIN, CLIENT_ID, CLIENT_LEAVE, GAME_STATE_UPDATE, CHAT_MESSAGE, CHAT_COMMAND} |
| typedef std::chrono::milliseconds::rep | **[ping_time_t](/engine/Namespaces/namespacetec_1_1networking/#typedef-ping_time_t)**  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| const std::string_view | **[SERVER_PORT](/engine/Namespaces/namespacetec_1_1networking/#variable-server_port)**  |
| const std::string_view | **[LOCAL_HOST](/engine/Namespaces/namespacetec_1_1networking/#variable-local_host)**  |
| const size_t | **[PING_HISTORY_SIZE](/engine/Namespaces/namespacetec_1_1networking/#variable-ping_history_size)**  |
| const size_t | **[DELAY_HISTORY_SIZE](/engine/Namespaces/namespacetec_1_1networking/#variable-delay_history_size)**  |
| unsigned short | **[PORT](/engine/Namespaces/namespacetec_1_1networking/#variable-port)**  |
| [eid](/engine/Namespaces/namespacetec/#typedef-eid) | **[BASE_ENTITY_ID](/engine/Namespaces/namespacetec_1_1networking/#variable-base_entity_id)**  |

## Types Documentation

### enum MessageType

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| SYNC | |   |
| MULTI_PART | |   |
| CLIENT_COMMAND | |   |
| ENTITY_CREATE | |   |
| ENTITY_DESTROY | |   |
| CLIENT_JOIN | |   |
| CLIENT_ID | |   |
| CLIENT_LEAVE | |   |
| GAME_STATE_UPDATE | |   |
| CHAT_MESSAGE | |   |
| CHAT_COMMAND | |   |




### typedef ping_time_t

```cpp
typedef std::chrono::milliseconds::rep tec::networking::ping_time_t;
```




## Attributes Documentation

### variable SERVER_PORT

```cpp
const std::string_view SERVER_PORT = "41228";
```


### variable LOCAL_HOST

```cpp
const std::string_view LOCAL_HOST = "127.0.0.1";
```


### variable PING_HISTORY_SIZE

```cpp
const size_t PING_HISTORY_SIZE = 10;
```


### variable DELAY_HISTORY_SIZE

```cpp
const size_t DELAY_HISTORY_SIZE = 10;
```


### variable PORT

```cpp
unsigned short PORT = 0xa10c;
```


### variable BASE_ENTITY_ID

```cpp
static eid BASE_ENTITY_ID = 10000;
```





-------------------------------

Updated on 21 March 2021 at 16:51:04 UTC