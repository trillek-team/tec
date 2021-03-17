---
title: tec::CommandQueue

---

# tec::CommandQueue



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](Classes/classtec_1_1_command_queue.md#function-commandqueue)**() |
| | **[~CommandQueue](Classes/classtec_1_1_command_queue.md#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](Classes/classtec_1_1_command_queue.md#function-processcommandqueue)**() |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**([Command](Classes/structtec_1_1_command.md)< T > && command) |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**(std::function< void(T *)> && command) |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * > | **[global_command_queue](Classes/classtec_1_1_command_queue.md#variable-global_command_queue)**  |
| std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * | **[local_queue](Classes/classtec_1_1_command_queue.md#variable-local_queue)**  |

## Detailed Description

```cpp
template <class T >
class tec::CommandQueue;
```

## Public Functions Documentation

### function CommandQueue

```cpp
inline CommandQueue()
```


### function ~CommandQueue

```cpp
inline ~CommandQueue()
```


### function ProcessCommandQueue

```cpp
inline void ProcessCommandQueue()
```


### function QueueCommand

```cpp
static inline void QueueCommand(
    Command< T > && command
)
```


### function QueueCommand

```cpp
static inline void QueueCommand(
    std::function< void(T *)> && command
)
```


## Protected Attributes Documentation

### variable global_command_queue

```cpp
static std::atomic< std::queue< Command< T > > * > global_command_queue;
```


### variable local_queue

```cpp
std::queue< Command< T > > * local_queue;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC