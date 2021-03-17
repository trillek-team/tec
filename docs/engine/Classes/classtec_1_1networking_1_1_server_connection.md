---
title: tec::networking::ServerConnection

---

# tec::networking::ServerConnection



## Public Types

|                | Name           |
| -------------- | -------------- |
| typedef std::function< void([MessageIn](Classes/classtec_1_1networking_1_1_message_in.md) &)> | **[messageHandlerFunc](Classes/classtec_1_1networking_1_1_server_connection.md#typedef-messagehandlerfunc)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ServerConnection](Classes/classtec_1_1networking_1_1_server_connection.md#function-serverconnection)**([ServerStats](Classes/classtec_1_1_server_stats.md) & s) |
| bool | **[Connect](Classes/classtec_1_1networking_1_1_server_connection.md#function-connect)**(std::string_view ip =LOCAL_HOST) |
| void | **[Disconnect](Classes/classtec_1_1networking_1_1_server_connection.md#function-disconnect)**() |
| void | **[Stop](Classes/classtec_1_1networking_1_1_server_connection.md#function-stop)**() |
| void | **[StartDispatch](Classes/classtec_1_1networking_1_1_server_connection.md#function-startdispatch)**() |
| void | **[StartSync](Classes/classtec_1_1networking_1_1_server_connection.md#function-startsync)**() |
| void | **[SendChatMessage](Classes/classtec_1_1networking_1_1_server_connection.md#function-sendchatmessage)**(std::string message) |
| void | **[Send](Classes/classtec_1_1networking_1_1_server_connection.md#function-send)**(MessagePool::ptr_type msg) |
| void | **[Send](Classes/classtec_1_1networking_1_1_server_connection.md#function-send)**([MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) & msg) |
| void | **[Send](Classes/classtec_1_1networking_1_1_server_connection.md#function-send)**([MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) && msg) |
| state_id_t | **[GetLastRecvStateID](Classes/classtec_1_1networking_1_1_server_connection.md#function-getlastrecvstateid)**() |
| std::list< ping_time_t > | **[GetRecentPings](Classes/classtec_1_1networking_1_1_server_connection.md#function-getrecentpings)**() |
| ping_time_t | **[GetAveragePing](Classes/classtec_1_1networking_1_1_server_connection.md#function-getaverageping)**() |
| ping_time_t | **[GetEstimatedDelay](Classes/classtec_1_1networking_1_1_server_connection.md#function-getestimateddelay)**() |
| [eid](Namespaces/namespacetec.md#typedef-eid) | **[GetClientID](Classes/classtec_1_1networking_1_1_server_connection.md#function-getclientid)**() |
| void | **[RegisterMessageHandler](Classes/classtec_1_1networking_1_1_server_connection.md#function-registermessagehandler)**(MessageType type, messageHandlerFunc handler) |
| void | **[RegisterConnectFunc](Classes/classtec_1_1networking_1_1_server_connection.md#function-registerconnectfunc)**(std::function< void()> func) |
| size_t | **[GetPartialMessageCount](Classes/classtec_1_1networking_1_1_server_connection.md#function-getpartialmessagecount)**() const |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| [ServerStats](Classes/classtec_1_1_server_stats.md) & | **[stats](Classes/classtec_1_1networking_1_1_server_connection.md#variable-stats)**  |

## Public Types Documentation

### typedef messageHandlerFunc

```cpp
typedef std::function<void(MessageIn&)> tec::networking::ServerConnection::messageHandlerFunc;
```


## Public Functions Documentation

### function ServerConnection

```cpp
ServerConnection(
    ServerStats & s
)
```


### function Connect

```cpp
bool Connect(
    std::string_view ip =LOCAL_HOST
)
```


### function Disconnect

```cpp
void Disconnect()
```


### function Stop

```cpp
void Stop()
```


### function StartDispatch

```cpp
void StartDispatch()
```


### function StartSync

```cpp
void StartSync()
```


### function SendChatMessage

```cpp
void SendChatMessage(
    std::string message
)
```


### function Send

```cpp
void Send(
    MessagePool::ptr_type msg
)
```


### function Send

```cpp
void Send(
    MessageOut & msg
)
```


### function Send

```cpp
void Send(
    MessageOut && msg
)
```


### function GetLastRecvStateID

```cpp
inline state_id_t GetLastRecvStateID()
```


### function GetRecentPings

```cpp
inline std::list< ping_time_t > GetRecentPings()
```


### function GetAveragePing

```cpp
inline ping_time_t GetAveragePing()
```


### function GetEstimatedDelay

```cpp
inline ping_time_t GetEstimatedDelay()
```


### function GetClientID

```cpp
inline eid GetClientID()
```


### function RegisterMessageHandler

```cpp
inline void RegisterMessageHandler(
    MessageType type,
    messageHandlerFunc handler
)
```


### function RegisterConnectFunc

```cpp
void RegisterConnectFunc(
    std::function< void()> func
)
```


### function GetPartialMessageCount

```cpp
inline size_t GetPartialMessageCount() const
```


## Public Attributes Documentation

### variable stats

```cpp
ServerStats & stats;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC