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
| bool | **[OnConnect](/engine/Classes/classtec_1_1networking_1_1_server/#function-onconnect)**() |
| void | **[OnDisconnect](/engine/Classes/classtec_1_1networking_1_1_server/#function-ondisconnect)**(std::shared_ptr< [ClientConnection](/engine/Classes/classtec_1_1networking_1_1_client_connection/) > client) |
| void | **[Start](/engine/Classes/classtec_1_1networking_1_1_server/#function-start)**() |
| void | **[Stop](/engine/Classes/classtec_1_1networking_1_1_server/#function-stop)**() |
| const std::set< std::shared_ptr< [ClientConnection](/engine/Classes/classtec_1_1networking_1_1_client_connection/) > > & | **[GetClients](/engine/Classes/classtec_1_1networking_1_1_server/#function-getclients)**() |
| [LuaSystem](/engine/Classes/classtec_1_1_lua_system/) * | **[GetLuaSystem](/engine/Classes/classtec_1_1networking_1_1_server/#function-getluasystem)**() |
| void | **[ProcessEvents](/engine/Classes/classtec_1_1networking_1_1_server/#function-processevents)**() |
| virtual void | **[On](/engine/Classes/classtec_1_1networking_1_1_server/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [EntityCreated](/engine/Classes/structtec_1_1_entity_created/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1networking_1_1_server/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::shared_ptr< [EntityDestroyed](/engine/Classes/structtec_1_1_entity_destroyed/) > data) override |
| [system::UserAuthenticator](/engine/Classes/classtec_1_1system_1_1_user_authenticator/) & | **[GetAuthenticator](/engine/Classes/classtec_1_1networking_1_1_server/#function-getauthenticator)**() |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::mutex | **[client_list_mutex](/engine/Classes/classtec_1_1networking_1_1_server/#variable-client-list-mutex)**  |

## Friends

|                | Name           |
| -------------- | -------------- |
| class | **[ClientConnection](/engine/Classes/classtec_1_1networking_1_1_server/#friend-clientconnection)**  |

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
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

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
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |


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


### function OnConnect

```cpp
bool OnConnect()
```


### function OnDisconnect

```cpp
void OnDisconnect(
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


### function ProcessEvents

```cpp
void ProcessEvents()
```


### function On

```cpp
virtual void On(
    eid ,
    std::shared_ptr< EntityCreated > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    eid entity_id,
    std::shared_ptr< EntityDestroyed > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function GetAuthenticator

```cpp
inline system::UserAuthenticator & GetAuthenticator()
```


## Public Attributes Documentation

### variable client_list_mutex

```cpp
std::mutex client_list_mutex;
```


## Friends

### friend ClientConnection

```cpp
friend class ClientConnection(
    ClientConnection 
);
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000