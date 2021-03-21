---
title: tec::networking::client_connection_info

---

# tec::networking::client_connection_info



## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[from_endpoint](/engine/Classes/structtec_1_1networking_1_1client__connection__info/#function-from_endpoint)**(const asio::ip::tcp::endpoint & endpoint) |
| void | **[from_endpoint](/engine/Classes/structtec_1_1networking_1_1client__connection__info/#function-from_endpoint)**(const asio::ip::udp::endpoint & endpoint) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[cancel](/engine/Classes/structtec_1_1networking_1_1client__connection__info/#variable-cancel)**  |
| std::string | **[reason](/engine/Classes/structtec_1_1networking_1_1client__connection__info/#variable-reason)**  |
| int | **[port](/engine/Classes/structtec_1_1networking_1_1client__connection__info/#variable-port)**  |
| std::string | **[address](/engine/Classes/structtec_1_1networking_1_1client__connection__info/#variable-address)**  |
| std::string | **[family](/engine/Classes/structtec_1_1networking_1_1client__connection__info/#variable-family)**  |
| std::string | **[protocol](/engine/Classes/structtec_1_1networking_1_1client__connection__info/#variable-protocol)**  |

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

Updated on 21 March 2021 at 16:55:55 UTC