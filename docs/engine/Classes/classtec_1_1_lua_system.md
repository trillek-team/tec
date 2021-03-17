---
title: tec::LuaSystem

---

# tec::LuaSystem



Inherits from [tec::CommandQueue< LuaSystem >](Classes/classtec_1_1_command_queue.md), [tec::EventQueue< EntityCreated >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< EntityDestroyed >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< ChatCommandEvent >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[LuaSystem](Classes/classtec_1_1_lua_system.md#function-luasystem)**() |
| void | **[Update](Classes/classtec_1_1_lua_system.md#function-update)**(const double delta) |
| void | **[ProcessEvents](Classes/classtec_1_1_lua_system.md#function-processevents)**() |
| virtual void | **[On](Classes/classtec_1_1_lua_system.md#function-on)**(std::shared_ptr< [EntityCreated](Classes/structtec_1_1_entity_created.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_lua_system.md#function-on)**(std::shared_ptr< [EntityDestroyed](Classes/structtec_1_1_entity_destroyed.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_lua_system.md#function-on)**(std::shared_ptr< [ChatCommandEvent](Classes/structtec_1_1_chat_command_event.md) > data) |
| void | **[ExecuteString](Classes/classtec_1_1_lua_system.md#function-executestring)**(std::string script_string) |
| std::shared_ptr< [LuaScript](Classes/structtec_1_1_lua_script.md) > | **[LoadFile](Classes/classtec_1_1_lua_system.md#function-loadfile)**([FilePath](Classes/classtec_1_1_file_path.md) filepath) |
| sol::state & | **[GetGlobalState](Classes/classtec_1_1_lua_system.md#function-getglobalstate)**() |
| template <typename... Args\> <br>void | **[CallFunctions](Classes/classtec_1_1_lua_system.md#function-callfunctions)**(std::string function_name, Args &&... args) |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< LuaSystem >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](Classes/classtec_1_1_command_queue.md#function-commandqueue)**() |
| | **[~CommandQueue](Classes/classtec_1_1_command_queue.md#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](Classes/classtec_1_1_command_queue.md#function-processcommandqueue)**() |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**([Command](Classes/structtec_1_1_command.md)< T > && command) |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< LuaSystem >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * > | **[global_command_queue](Classes/classtec_1_1_command_queue.md#variable-global_command_queue)**  |
| std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * | **[local_queue](Classes/classtec_1_1_command_queue.md#variable-local_queue)**  |

**Public Functions inherited from [tec::EventQueue< EntityCreated >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< EntityCreated >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< EntityDestroyed >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< EntityDestroyed >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< ChatCommandEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< ChatCommandEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |


## Public Functions Documentation

### function LuaSystem

```cpp
LuaSystem()
```


### function Update

```cpp
void Update(
    const double delta
)
```


### function ProcessEvents

```cpp
void ProcessEvents()
```


### function On

```cpp
virtual void On(
    std::shared_ptr< EntityCreated > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< EntityDestroyed > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< ChatCommandEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function ExecuteString

```cpp
void ExecuteString(
    std::string script_string
)
```


### function LoadFile

```cpp
std::shared_ptr< LuaScript > LoadFile(
    FilePath filepath
)
```


### function GetGlobalState

```cpp
inline sol::state & GetGlobalState()
```


### function CallFunctions

```cpp
template <typename... Args>
inline void CallFunctions(
    std::string function_name,
    Args &&... args
)
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC