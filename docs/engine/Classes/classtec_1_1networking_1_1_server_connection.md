---
title: tec::networking::ServerConnection

---

# tec::networking::ServerConnection






`#include <server-connection.hpp>`

## Public Types

|                | Name           |
| -------------- | -------------- |
| typedef std::function< void([MessageIn](/engine/Classes/classtec_1_1networking_1_1_message_in/) &)> | **[messageHandlerFunc](/engine/Classes/classtec_1_1networking_1_1_server_connection/#typedef-messagehandlerfunc)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ServerConnection](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-serverconnection)**([ServerStats](/engine/Classes/classtec_1_1_server_stats/) & s) |
| bool | **[Connect](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-connect)**(std::string_view ip =[LOCAL_HOST](/engine/Namespaces/namespacetec_1_1networking/#variable-local-host)) |
| void | **[Disconnect](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-disconnect)**() |
| void | **[Stop](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-stop)**() |
| void | **[StartDispatch](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-startdispatch)**() |
| void | **[StartSync](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-startsync)**() |
| void | **[SendChatMessage](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-sendchatmessage)**(std::string message) |
| void | **[Send](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-send)**([MessagePool::ptr_type](/engine/Classes/classtec_1_1networking_1_1_message_pool/#typedef-ptr-type) msg) |
| void | **[Send](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-send)**([MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) & msg) |
| void | **[Send](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-send)**([MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) && msg) |
| [state_id_t](/engine/Namespaces/namespacetec/#typedef-state-id-t) | **[GetLastRecvStateID](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-getlastrecvstateid)**() |
| std::list< [ping_time_t](/engine/Namespaces/namespacetec_1_1networking/#typedef-ping-time-t) > | **[GetRecentPings](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-getrecentpings)**() |
| [ping_time_t](/engine/Namespaces/namespacetec_1_1networking/#typedef-ping-time-t) | **[GetAveragePing](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-getaverageping)**() |
| [ping_time_t](/engine/Namespaces/namespacetec_1_1networking/#typedef-ping-time-t) | **[GetEstimatedDelay](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-getestimateddelay)**() |
| [eid](/engine/Namespaces/namespacetec/#typedef-eid) | **[GetClientID](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-getclientid)**() |
| void | **[RegisterMessageHandler](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-registermessagehandler)**([MessageType](/engine/Namespaces/namespacetec_1_1networking/#enum-messagetype) type, [messageHandlerFunc](/engine/Classes/classtec_1_1networking_1_1_server_connection/#typedef-messagehandlerfunc) handler) |
| void | **[RegisterConnectFunc](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-registerconnectfunc)**(std::function< void()> func) |
| size_t | **[GetPartialMessageCount](/engine/Classes/classtec_1_1networking_1_1_server_connection/#function-getpartialmessagecount)**() const |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| [ServerStats](/engine/Classes/classtec_1_1_server_stats/) & | **[stats](/engine/Classes/classtec_1_1networking_1_1_server_connection/#variable-stats)**  |

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

Updated on 2022-07-24 at 19:27:35 +0000