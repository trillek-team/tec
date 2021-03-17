---
title: tec::networking

---

# tec::networking

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[tec::networking::ServerConnection](Classes/classtec_1_1networking_1_1_server_connection.md)**  |
| class | **[tec::networking::ClientConnection](Classes/classtec_1_1networking_1_1_client_connection.md)**  |
| struct | **[tec::networking::client_connection_info](Classes/structtec_1_1networking_1_1client__connection__info.md)**  |
| class | **[tec::networking::Server](Classes/classtec_1_1networking_1_1_server.md)**  |
| class | **[tec::networking::Message](Classes/classtec_1_1networking_1_1_message.md)**  |
| class | **[tec::networking::MessagePool](Classes/classtec_1_1networking_1_1_message_pool.md)**  |
| class | **[tec::networking::MessageOut](Classes/classtec_1_1networking_1_1_message_out.md)**  |
| class | **[tec::networking::MessageIn](Classes/classtec_1_1networking_1_1_message_in.md)**  |

## Types

|                | Name           |
| -------------- | -------------- |
| enum int | **[MessageType](Namespaces/namespacetec_1_1networking.md#enum-messagetype)** { SYNC, MULTI_PART, CLIENT_COMMAND, ENTITY_CREATE, ENTITY_DESTROY, CLIENT_JOIN, CLIENT_ID, CLIENT_LEAVE, GAME_STATE_UPDATE, CHAT_MESSAGE, CHAT_COMMAND} |
| typedef std::chrono::milliseconds::rep | **[ping_time_t](Namespaces/namespacetec_1_1networking.md#typedef-ping_time_t)**  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| const std::string_view | **[SERVER_PORT](Namespaces/namespacetec_1_1networking.md#variable-server_port)**  |
| const std::string_view | **[LOCAL_HOST](Namespaces/namespacetec_1_1networking.md#variable-local_host)**  |
| const size_t | **[PING_HISTORY_SIZE](Namespaces/namespacetec_1_1networking.md#variable-ping_history_size)**  |
| const size_t | **[DELAY_HISTORY_SIZE](Namespaces/namespacetec_1_1networking.md#variable-delay_history_size)**  |
| unsigned short | **[PORT](Namespaces/namespacetec_1_1networking.md#variable-port)**  |
| [eid](Namespaces/namespacetec.md#typedef-eid) | **[BASE_ENTITY_ID](Namespaces/namespacetec_1_1networking.md#variable-base_entity_id)**  |

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

Updated on 17 March 2021 at 23:08:11 UTC