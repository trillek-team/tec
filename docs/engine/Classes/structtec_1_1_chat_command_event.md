---
title: tec::ChatCommandEvent

---

# tec::ChatCommandEvent





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ChatCommandEvent](/engine/Classes/structtec_1_1_chat_command_event/#function-chatcommandevent)**() =default |
| | **[ChatCommandEvent](/engine/Classes/structtec_1_1_chat_command_event/#function-chatcommandevent)**(proto::ChatCommand chat_command) |
| void | **[In](/engine/Classes/structtec_1_1_chat_command_event/#function-in)**(proto::ChatCommand chat_command) |
| void | **[Out](/engine/Classes/structtec_1_1_chat_command_event/#function-out)**(proto::ChatCommand & chat_command) |
| proto::ChatCommand | **[Out](/engine/Classes/structtec_1_1_chat_command_event/#function-out)**() |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::string | **[command](/engine/Classes/structtec_1_1_chat_command_event/#variable-command)**  |
| std::vector< std::string > | **[args](/engine/Classes/structtec_1_1_chat_command_event/#variable-args)**  |

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
    proto::ChatCommand & chat_command
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

Updated on 2022-07-06 at 04:15:07 +0000