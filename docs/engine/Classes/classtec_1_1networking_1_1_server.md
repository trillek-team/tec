---
title: tec::networking::Server

---

# tec::networking::Server



Inherits from [tec::EventQueue< EntityCreated >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< EntityDestroyed >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Server](/engine/Classes/classtec_1_1networking_1_1_server/#function-server)**(tcp::endpoint & endpoint) |
| void | **[Deliver](/engine/Classes/classtec_1_1networking_1_1_server/#function-deliver)**([MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) & msg, bool save_to_recent =true) |
| void | **[Deliver](/engine/Classes/classtec_1_1networking_1_1_server/#function-deliver)**([MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) && msg, bool save_to_recent =true) |
| void | **[Deliver](/engine/Classes/classtec_1_1networking_1_1_server/#function-deliver)**(std::shared_ptr< [ClientConnection](/engine/Classes/classtec_1_1networking_1_1_client_connection/) > client, [MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) & msg) |
| void | **[Deliver](/engine/Classes/classtec_1_1networking_1_1_server/#function-deliver)**(std::shared_ptr< [ClientConnection](/engine/Classes/classtec_1_1networking_1_1_client_connection/) > client, [MessageOut](/engine/Classes/classtec_1_1networking_1_1_message_out/) && msg) |
| void | **[Leave](/engine/Classes/classtec_1_1networking_1_1_server/#function-leave)**(std::shared_ptr< [ClientConnection](/engine/Classes/classtec_1_1networking_1_1_client_connection/) > client) |
| void | **[Start](/engine/Classes/classtec_1_1networking_1_1_server/#function-start)**() |
| void | **[Stop](/engine/Classes/classtec_1_1networking_1_1_server/#function-stop)**() |
| const std::set< std::shared_ptr< [ClientConnection](/engine/Classes/classtec_1_1networking_1_1_client_connection/) > > & | **[GetClients](/engine/Classes/classtec_1_1networking_1_1_server/#function-getclients)**() |
| [LuaSystem](/engine/Classes/classtec_1_1_lua_system/) * | **[GetLuaSystem](/engine/Classes/classtec_1_1networking_1_1_server/#function-getluasystem)**() |
| virtual void | **[On](/engine/Classes/classtec_1_1networking_1_1_server/#function-on)**(std::shared_ptr< [EntityCreated](/engine/Classes/structtec_1_1_entity_created/) > data) |
| virtual void | **[On](/engine/Classes/classtec_1_1networking_1_1_server/#function-on)**(std::shared_ptr< [EntityDestroyed](/engine/Classes/structtec_1_1_entity_destroyed/) > data) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::mutex | **[client_list_mutex](/engine/Classes/classtec_1_1networking_1_1_server/#variable-client_list_mutex)**  |

## Additional inherited members

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


## Public Functions Documentation

### function Server

```cpp
Server(
    tcp::endpoint & endpoint
)
```


### function Deliver

```cpp
void Deliver(
    MessageOut & msg,
    bool save_to_recent =true
)
```


### function Deliver

```cpp
void Deliver(
    MessageOut && msg,
    bool save_to_recent =true
)
```


### function Deliver

```cpp
void Deliver(
    std::shared_ptr< ClientConnection > client,
    MessageOut & msg
)
```


### function Deliver

```cpp
void Deliver(
    std::shared_ptr< ClientConnection > client,
    MessageOut && msg
)
```


### function Leave

```cpp
void Leave(
    std::shared_ptr< ClientConnection > client
)
```


### function Start

```cpp
void Start()
```


### function Stop

```cpp
void Stop()
```


### function GetClients

```cpp
inline const std::set< std::shared_ptr< ClientConnection > > & GetClients()
```


### function GetLuaSystem

```cpp
inline LuaSystem * GetLuaSystem()
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


## Public Attributes Documentation

### variable client_list_mutex

```cpp
std::mutex client_list_mutex;
```


-------------------------------

Updated on 17 March 2021 at 22:09:29 UTC