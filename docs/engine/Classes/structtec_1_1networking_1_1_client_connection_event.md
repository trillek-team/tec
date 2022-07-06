---
title: tec::networking::ClientConnectionEvent

---

# tec::networking::ClientConnectionEvent



 [More...](#detailed-description)


`#include <server.hpp>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[RegisterLuaType](/engine/Classes/structtec_1_1networking_1_1_client_connection_event/#function-registerluatype)**(sol::state & ) |
| void | **[from_endpoint](/engine/Classes/structtec_1_1networking_1_1_client_connection_event/#function-from-endpoint)**(const asio::ip::tcp::endpoint & endpoint)<br>load info from a TCP endpoint  |
| void | **[from_endpoint](/engine/Classes/structtec_1_1networking_1_1_client_connection_event/#function-from-endpoint)**(const asio::ip::udp::endpoint & endpoint)<br>load info from a UDP endpoint  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[cancel](/engine/Classes/structtec_1_1networking_1_1_client_connection_event/#variable-cancel)**  |
| std::string | **[reason](/engine/Classes/structtec_1_1networking_1_1_client_connection_event/#variable-reason)**  |
| int | **[port](/engine/Classes/structtec_1_1networking_1_1_client_connection_event/#variable-port)**  |
| std::string | **[address](/engine/Classes/structtec_1_1networking_1_1_client_connection_event/#variable-address)**  |
| std::string | **[family](/engine/Classes/structtec_1_1networking_1_1_client_connection_event/#variable-family)**  |
| std::string | **[protocol](/engine/Classes/structtec_1_1networking_1_1_client_connection_event/#variable-protocol)**  |

## Detailed Description

```cpp
struct tec::networking::ClientConnectionEvent;
```


this structure passed to lua functions: onClientConnected onClientDisconnected containing information about the event. for connect events: setting cancel to true will reject the connection, rejected connections will get the "reason" string sent to the client. 

## Public Functions Documentation

### function RegisterLuaType

```cpp
static void RegisterLuaType(
    sol::state & 
)
```


### function from_endpoint

```cpp
void from_endpoint(
    const asio::ip::tcp::endpoint & endpoint
)
```

load info from a TCP endpoint 

### function from_endpoint

```cpp
void from_endpoint(
    const asio::ip::udp::endpoint & endpoint
)
```

load info from a UDP endpoint 

## Public Attributes Documentation

### variable cancel

```cpp
bool cancel;
```


### variable reason

```cpp
std::string reason;
```


### variable port

```cpp
int port;
```


### variable address

```cpp
std::string address;
```


### variable family

```cpp
std::string family;
```


### variable protocol

```cpp
std::string protocol;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000