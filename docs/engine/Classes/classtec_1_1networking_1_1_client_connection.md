---
title: tec::networking::ClientConnection

---

# tec::networking::ClientConnection



Inherits from std::enable_shared_from_this< ClientConnection >

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ClientConnection](Classes/classtec_1_1networking_1_1_client_connection.md#function-clientconnection)**(tcp::socket _socket, tcp::endpoint _endpoint, [Server](Classes/classtec_1_1networking_1_1_server.md) * server) |
| | **[~ClientConnection](Classes/classtec_1_1networking_1_1_client_connection.md#function-~clientconnection)**() |
| void | **[StartRead](Classes/classtec_1_1networking_1_1_client_connection.md#function-startread)**() |
| void | **[QueueWrite](Classes/classtec_1_1networking_1_1_client_connection.md#function-queuewrite)**(MessagePool::ptr_type msg) |
| void | **[QueueWrite](Classes/classtec_1_1networking_1_1_client_connection.md#function-queuewrite)**([MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) & msg) |
| void | **[QueueWrite](Classes/classtec_1_1networking_1_1_client_connection.md#function-queuewrite)**([MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) && msg) |
| [eid](Namespaces/namespacetec.md#typedef-eid) | **[GetID](Classes/classtec_1_1networking_1_1_client_connection.md#function-getid)**() |
| tcp::endpoint | **[GetEndpoint](Classes/classtec_1_1networking_1_1_client_connection.md#function-getendpoint)**() |
| void | **[SetID](Classes/classtec_1_1networking_1_1_client_connection.md#function-setid)**([eid](Namespaces/namespacetec.md#typedef-eid) id) |
| proto::Entity & | **[GetEntity](Classes/classtec_1_1networking_1_1_client_connection.md#function-getentity)**() |
| void | **[DoJoin](Classes/classtec_1_1networking_1_1_client_connection.md#function-dojoin)**() |
| void | **[DoLeave](Classes/classtec_1_1networking_1_1_client_connection.md#function-doleave)**() |
| void | **[OnClientLeave](Classes/classtec_1_1networking_1_1_client_connection.md#function-onclientleave)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| void | **[ConfirmStateID](Classes/classtec_1_1networking_1_1_client_connection.md#function-confirmstateid)**(state_id_t state_id) |
| state_id_t | **[GetLastConfirmedStateID](Classes/classtec_1_1networking_1_1_client_connection.md#function-getlastconfirmedstateid)**() |
| void | **[UpdateGameState](Classes/classtec_1_1networking_1_1_client_connection.md#function-updategamestate)**(const [GameState](Classes/structtec_1_1_game_state.md) & full_state) |
| [MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) | **[PrepareGameStateUpdateMessage](Classes/classtec_1_1networking_1_1_client_connection.md#function-preparegamestateupdatemessage)**(state_id_t current_state_id, uint64_t current_timestamp) |
| size_t | **[GetPartialMessageCount](Classes/classtec_1_1networking_1_1_client_connection.md#function-getpartialmessagecount)**() const |

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

Updated on 17 March 2021 at 23:08:11 UTC