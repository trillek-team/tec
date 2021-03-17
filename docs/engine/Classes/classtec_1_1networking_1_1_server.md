---
title: tec::networking::Server

---

# tec::networking::Server



Inherits from [tec::EventQueue< EntityCreated >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< EntityDestroyed >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Server](Classes/classtec_1_1networking_1_1_server.md#function-server)**(tcp::endpoint & endpoint) |
| void | **[Deliver](Classes/classtec_1_1networking_1_1_server.md#function-deliver)**([MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) & msg, bool save_to_recent =true) |
| void | **[Deliver](Classes/classtec_1_1networking_1_1_server.md#function-deliver)**([MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) && msg, bool save_to_recent =true) |
| void | **[Deliver](Classes/classtec_1_1networking_1_1_server.md#function-deliver)**(std::shared_ptr< [ClientConnection](Classes/classtec_1_1networking_1_1_client_connection.md) > client, [MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) & msg) |
| void | **[Deliver](Classes/classtec_1_1networking_1_1_server.md#function-deliver)**(std::shared_ptr< [ClientConnection](Classes/classtec_1_1networking_1_1_client_connection.md) > client, [MessageOut](Classes/classtec_1_1networking_1_1_message_out.md) && msg) |
| void | **[Leave](Classes/classtec_1_1networking_1_1_server.md#function-leave)**(std::shared_ptr< [ClientConnection](Classes/classtec_1_1networking_1_1_client_connection.md) > client) |
| void | **[Start](Classes/classtec_1_1networking_1_1_server.md#function-start)**() |
| void | **[Stop](Classes/classtec_1_1networking_1_1_server.md#function-stop)**() |
| const std::set< std::shared_ptr< [ClientConnection](Classes/classtec_1_1networking_1_1_client_connection.md) > > & | **[GetClients](Classes/classtec_1_1networking_1_1_server.md#function-getclients)**() |
| [LuaSystem](Classes/classtec_1_1_lua_system.md) * | **[GetLuaSystem](Classes/classtec_1_1networking_1_1_server.md#function-getluasystem)**() |
| virtual void | **[On](Classes/classtec_1_1networking_1_1_server.md#function-on)**(std::shared_ptr< [EntityCreated](Classes/structtec_1_1_entity_created.md) > data) |
| virtual void | **[On](Classes/classtec_1_1networking_1_1_server.md#function-on)**(std::shared_ptr< [EntityDestroyed](Classes/structtec_1_1_entity_destroyed.md) > data) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::mutex | **[client_list_mutex](Classes/classtec_1_1networking_1_1_server.md#variable-client_list_mutex)**  |

## Additional inherited members

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


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< EntityDestroyed > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


## Public Attributes Documentation

### variable client_list_mutex

```cpp
std::mutex client_list_mutex;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC