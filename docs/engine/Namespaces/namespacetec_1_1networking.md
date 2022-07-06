---
title: tec::networking

---

# tec::networking



## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[tec::networking::ClientConnection](/engine/Classes/classtec_1_1networking_1_1_client_connection/)**  |
| struct | **[tec::networking::ClientConnectionEvent](/engine/Classes/structtec_1_1networking_1_1_client_connection_event/)**  |
| class | **[tec::networking::Message](/engine/Classes/classtec_1_1networking_1_1_message/)**  |
| class | **[tec::networking::MessageIn](/engine/Classes/classtec_1_1networking_1_1_message_in/)**  |
| class | **[tec::networking::MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/)**  |
| class | **[tec::networking::MessagePool](/engine/Classes/classtec_1_1networking_1_1_message_pool/)**  |
| class | **[tec::networking::Server](/engine/Classes/classtec_1_1networking_1_1_server/)**  |
| class | **[tec::networking::ServerConnection](/engine/Classes/classtec_1_1networking_1_1_server_connection/)**  |
| struct | **[tec::networking::UserLoginEvent](/engine/Classes/structtec_1_1networking_1_1_user_login_event/)**  |

## Types

|                | Name           |
| -------------- | -------------- |
| enum int | **[MessageType](/engine/Namespaces/namespacetec_1_1networking/#enum-messagetype)** { SYNC, MULTI_PART, CLIENT_COMMAND, ENTITY_CREATE, ENTITY_DESTROY, CLIENT_JOIN, CLIENT_ID, CLIENT_LEAVE, GAME_STATE_UPDATE, CHAT_MESSAGE, CHAT_COMMAND, LOGIN, AUTHENTICATED, WORLD_SENT, CLIENT_READY_TO_RECEIVE} |
| typedef std::chrono::milliseconds::rep | **[ping_time_t](/engine/Namespaces/namespacetec_1_1networking/#typedef-ping-time-t)**  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| const std::string_view | **[SERVER_PORT](/engine/Namespaces/namespacetec_1_1networking/#variable-server-port)**  |
| const std::string_view | **[LOCAL_HOST](/engine/Namespaces/namespacetec_1_1networking/#variable-local-host)**  |
| const size_t | **[PING_HISTORY_SIZE](/engine/Namespaces/namespacetec_1_1networking/#variable-ping-history-size)**  |
| const size_t | **[DELAY_HISTORY_SIZE](/engine/Namespaces/namespacetec_1_1networking/#variable-delay-history-size)**  |
| unsigned short | **[PORT](/engine/Namespaces/namespacetec_1_1networking/#variable-port)**  |

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
| LOGIN | |   |
| AUTHENTICATED | |   |
| WORLD_SENT | |   |
| CLIENT_READY_TO_RECEIVE | |   |




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





-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000