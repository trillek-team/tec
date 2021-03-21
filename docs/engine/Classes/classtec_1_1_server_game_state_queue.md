---
title: tec::ServerGameStateQueue

---

# tec::ServerGameStateQueue



Inherits from [tec::EventQueue< EntityCreated >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< EntityDestroyed >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ServerGameStateQueue](/engine/Classes/classtec_1_1_server_game_state_queue/#function-servergamestatequeue)**([ServerStats](/engine/Classes/classtec_1_1_server_stats/) & s) |
| virtual void | **[On](/engine/Classes/classtec_1_1_server_game_state_queue/#function-on)**(std::shared_ptr< [EntityCreated](/engine/Classes/structtec_1_1_entity_created/) > data) |
| virtual void | **[On](/engine/Classes/classtec_1_1_server_game_state_queue/#function-on)**(std::shared_ptr< [EntityDestroyed](/engine/Classes/structtec_1_1_entity_destroyed/) > data) |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_server_game_state_queue/#function-processeventqueue)**() |
| [GameState](/engine/Classes/structtec_1_1_game_state/) & | **[GetBaseState](/engine/Classes/classtec_1_1_server_game_state_queue/#function-getbasestate)**() |
| void | **[SetBaseState](/engine/Classes/classtec_1_1_server_game_state_queue/#function-setbasestate)**([GameState](/engine/Classes/structtec_1_1_game_state/) && new_state) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| [ServerStats](/engine/Classes/classtec_1_1_server_stats/) & | **[stats](/engine/Classes/classtec_1_1_server_game_state_queue/#variable-stats)**  |

## Additional inherited members

**Public Functions inherited from [tec::EventQueue< EntityCreated >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
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
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< EntityDestroyed >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read_event_queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write_event_queue)**  |


## Public Functions Documentation

### function ServerGameStateQueue

```cpp
ServerGameStateQueue(
    ServerStats & s
)
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


### function ProcessEventQueue

```cpp
void ProcessEventQueue()
```


### function GetBaseState

```cpp
inline GameState & GetBaseState()
```


### function SetBaseState

```cpp
inline void SetBaseState(
    GameState && new_state
)
```


## Public Attributes Documentation

### variable stats

```cpp
ServerStats & stats;
```


-------------------------------

Updated on 21 March 2021 at 16:41:46 UTC