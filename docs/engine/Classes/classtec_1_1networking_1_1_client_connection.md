---
title: tec::networking::ClientConnection

---

# tec::networking::ClientConnection



Inherits from std::enable_shared_from_this< ClientConnection >

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ClientConnection](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-clientconnection)**(tcp::socket _socket, tcp::endpoint _endpoint, [Server](/engine/Classes/classtec_1_1networking_1_1_server/) * server) |
| | **[~ClientConnection](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-~clientconnection)**() |
| void | **[StartRead](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-startread)**() |
| void | **[QueueWrite](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-queuewrite)**(MessagePool::ptr_type msg) |
| void | **[QueueWrite](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-queuewrite)**([MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) & msg) |
| void | **[QueueWrite](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-queuewrite)**([MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) && msg) |
| [eid](/engine/Namespaces/namespacetec/#typedef-eid) | **[GetID](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-getid)**() |
| tcp::endpoint | **[GetEndpoint](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-getendpoint)**() |
| void | **[SetID](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-setid)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) id) |
| proto::Entity & | **[GetEntity](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-getentity)**() |
| void | **[DoJoin](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-dojoin)**() |
| void | **[DoLeave](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-doleave)**() |
| void | **[OnClientLeave](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-onclientleave)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| void | **[ConfirmStateID](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-confirmstateid)**(state_id_t state_id) |
| state_id_t | **[GetLastConfirmedStateID](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-getlastconfirmedstateid)**() |
| void | **[UpdateGameState](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-updategamestate)**(const [GameState](/engine/Classes/structtec_1_1_game_state/) & full_state) |
| [MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) | **[PrepareGameStateUpdateMessage](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-preparegamestateupdatemessage)**(state_id_t current_state_id, uint64_t current_timestamp) |
| size_t | **[GetPartialMessageCount](/engine/Classes/classtec_1_1networking_1_1_client_connection/#function-getpartialmessagecount)**() const |

## Public Functions Documentation

### function ClientConnection

```cpp
ClientConnection(
    tcp::socket _socket,
    tcp::endpoint _endpoint,
    Server * server
)
```


### function ~ClientConnection

```cpp
~ClientConnection()
```


### function StartRead

```cpp
void StartRead()
```


### function QueueWrite

```cpp
void QueueWrite(
    MessagePool::ptr_type msg
)
```


### function QueueWrite

```cpp
void QueueWrite(
    MessageOut & msg
)
```


### function QueueWrite

```cpp
void QueueWrite(
    MessageOut && msg
)
```


### function GetID

```cpp
inline eid GetID()
```


### function GetEndpoint

```cpp
inline tcp::endpoint GetEndpoint()
```


### function SetID

```cpp
void SetID(
    eid id
)
```


### function GetEntity

```cpp
inline proto::Entity & GetEntity()
```


### function DoJoin

```cpp
void DoJoin()
```


### function DoLeave

```cpp
void DoLeave()
```


### function OnClientLeave

```cpp
void OnClientLeave(
    eid entity_id
)
```


### function ConfirmStateID

```cpp
inline void ConfirmStateID(
    state_id_t state_id
)
```


### function GetLastConfirmedStateID

```cpp
inline state_id_t GetLastConfirmedStateID()
```


### function UpdateGameState

```cpp
void UpdateGameState(
    const GameState & full_state
)
```


### function PrepareGameStateUpdateMessage

```cpp
MessageOut PrepareGameStateUpdateMessage(
    state_id_t current_state_id,
    uint64_t current_timestamp
)
```


### function GetPartialMessageCount

```cpp
inline size_t GetPartialMessageCount() const
```


-------------------------------

Updated on 21 March 2021 at 16:46:06 UTC