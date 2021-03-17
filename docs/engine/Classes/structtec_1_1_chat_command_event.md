---
title: tec::ChatCommandEvent

---

# tec::ChatCommandEvent



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ChatCommandEvent](Classes/structtec_1_1_chat_command_event.md#function-chatcommandevent)**() =default |
| | **[ChatCommandEvent](Classes/structtec_1_1_chat_command_event.md#function-chatcommandevent)**(proto::ChatCommand chat_command) |
| void | **[In](Classes/structtec_1_1_chat_command_event.md#function-in)**(proto::ChatCommand chat_command) |
| void | **[Out](Classes/structtec_1_1_chat_command_event.md#function-out)**(proto::ChatCommand chat_command) |
| proto::ChatCommand | **[Out](Classes/structtec_1_1_chat_command_event.md#function-out)**() |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::string | **[command](Classes/structtec_1_1_chat_command_event.md#variable-command)**  |
| std::vector< std::string > | **[args](Classes/structtec_1_1_chat_command_event.md#variable-args)**  |

## Public Functions Documentation

### function ChatCommandEvent

```cpp
ChatCommandEvent() =default
```


### function ChatCommandEvent

```cpp
inline ChatCommandEvent(
    proto::ChatCommand chat_command
)
```


### function In

```cpp
inline void In(
    proto::ChatCommand chat_command
)
```


### function Out

```cpp
inline void Out(
    proto::ChatCommand chat_command
)
```


### function Out

```cpp
inline proto::ChatCommand Out()
```


## Public Attributes Documentation

### variable command

```cpp
std::string command;
```


### variable args

```cpp
std::vector< std::string > args;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC