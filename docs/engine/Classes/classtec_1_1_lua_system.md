---
title: tec::LuaSystem

---

# tec::LuaSystem



Inherits from [tec::CommandQueue< LuaSystem >](/engine/Classes/classtec_1_1_command_queue/), [tec::EventQueue< EntityCreated >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< EntityDestroyed >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< ChatCommandEvent >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[LuaSystem](/engine/Classes/classtec_1_1_lua_system/#function-luasystem)**() |
| void | **[Update](/engine/Classes/classtec_1_1_lua_system/#function-update)**(const double delta) |
| void | **[ProcessEvents](/engine/Classes/classtec_1_1_lua_system/#function-processevents)**() |
| virtual void | **[On](/engine/Classes/classtec_1_1_lua_system/#function-on)**(std::shared_ptr< [EntityCreated](/engine/Classes/structtec_1_1_entity_created/) > data) |
| virtual void | **[On](/engine/Classes/classtec_1_1_lua_system/#function-on)**(std::shared_ptr< [EntityDestroyed](/engine/Classes/structtec_1_1_entity_destroyed/) > data) |
| virtual void | **[On](/engine/Classes/classtec_1_1_lua_system/#function-on)**(std::shared_ptr< [ChatCommandEvent](/engine/Classes/structtec_1_1_chat_command_event/) > data) |
| void | **[ExecuteString](/engine/Classes/classtec_1_1_lua_system/#function-executestring)**(std::string script_string) |
| std::shared_ptr< [LuaScript](/engine/Classes/structtec_1_1_lua_script/) > | **[LoadFile](/engine/Classes/classtec_1_1_lua_system/#function-loadfile)**([FilePath](/engine/Classes/classtec_1_1_file_path/) filepath) |
| sol::state & | **[GetGlobalState](/engine/Classes/classtec_1_1_lua_system/#function-getglobalstate)**() |
| template <typename... Args\> <br>void | **[CallFunctions](/engine/Classes/classtec_1_1_lua_system/#function-callfunctions)**(std::string function_name, Args &&... args) |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< LuaSystem >](/engine/Classes/classtec_1_1_command_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-commandqueue)**() |
| | **[~CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-processcommandqueue)**() |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**([Command](/engine/Classes/structtec_1_1_command/)< T > && command) |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< LuaSystem >](/engine/Classes/classtec_1_1_command_queue/)**

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * > | **[global_command_queue](/engine/Classes/classtec_1_1_command_queue/#variable-global_command_queue)**  |
| std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * | **[local_queue](/engine/Classes/classtec_1_1_command_queue/#variable-local_queue)**  |

**Public Functions inherited from [tec::EventQueue< EntityCreated >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< EntityCreated >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read_event_queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< EntityDestroyed >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< EntityDestroyed >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read_event_queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< ChatCommandEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< ChatCommandEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read_event_queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write_event_queue)**  |


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


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< EntityDestroyed > data
)
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< ChatCommandEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


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

Updated on 21 March 2021 at 16:55:55 UTC