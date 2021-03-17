---
title: tec::networking::client_connection_info

---

# tec::networking::client_connection_info



## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[from_endpoint](Classes/structtec_1_1networking_1_1client__connection__info.md#function-from_endpoint)**(const asio::ip::tcp::endpoint & endpoint) |
| void | **[from_endpoint](Classes/structtec_1_1networking_1_1client__connection__info.md#function-from_endpoint)**(const asio::ip::udp::endpoint & endpoint) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[cancel](Classes/structtec_1_1networking_1_1client__connection__info.md#variable-cancel)**  |
| std::string | **[reason](Classes/structtec_1_1networking_1_1client__connection__info.md#variable-reason)**  |
| int | **[port](Classes/structtec_1_1networking_1_1client__connection__info.md#variable-port)**  |
| std::string | **[address](Classes/structtec_1_1networking_1_1client__connection__info.md#variable-address)**  |
| std::string | **[family](Classes/structtec_1_1networking_1_1client__connection__info.md#variable-family)**  |
| std::string | **[protocol](Classes/structtec_1_1networking_1_1client__connection__info.md#variable-protocol)**  |

## Public Functions Documentation

### function from_endpoint

```cpp
inline void from_endpoint(
    const asio::ip::tcp::endpoint & endpoint
)
```


### function from_endpoint

```cpp
inline void from_endpoint(
    const asio::ip::udp::endpoint & endpoint
)
```


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

Updated on 17 March 2021 at 23:08:11 UTC