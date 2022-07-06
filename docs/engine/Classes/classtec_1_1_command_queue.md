---
title: tec::CommandQueue

---

# tec::CommandQueue



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-commandqueue)**() |
| | **[~CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-processcommandqueue)**() |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**([Command](/engine/Classes/structtec_1_1_command/)< T > && command) |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**(std::function< void(T *)> && command) |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * > | **[global_command_queue](/engine/Classes/classtec_1_1_command_queue/#variable-global-command-queue)**  |
| std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * | **[local_queue](/engine/Classes/classtec_1_1_command_queue/#variable-local-queue)**  |

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

Updated on 2022-07-06 at 04:15:07 +0000