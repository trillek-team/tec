---
title: tec::ServerGameStateQueue

---

# tec::ServerGameStateQueue



Inherits from [tec::EventQueue< EntityCreated >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< EntityDestroyed >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ServerGameStateQueue](Classes/classtec_1_1_server_game_state_queue.md#function-servergamestatequeue)**([ServerStats](Classes/classtec_1_1_server_stats.md) & s) |
| virtual void | **[On](Classes/classtec_1_1_server_game_state_queue.md#function-on)**(std::shared_ptr< [EntityCreated](Classes/structtec_1_1_entity_created.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_server_game_state_queue.md#function-on)**(std::shared_ptr< [EntityDestroyed](Classes/structtec_1_1_entity_destroyed.md) > data) |
| void | **[ProcessEventQueue](Classes/classtec_1_1_server_game_state_queue.md#function-processeventqueue)**() |
| [GameState](Classes/structtec_1_1_game_state.md) & | **[GetBaseState](Classes/classtec_1_1_server_game_state_queue.md#function-getbasestate)**() |
| void | **[SetBaseState](Classes/classtec_1_1_server_game_state_queue.md#function-setbasestate)**([GameState](Classes/structtec_1_1_game_state.md) && new_state) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| [ServerStats](Classes/classtec_1_1_server_stats.md) & | **[stats](Classes/classtec_1_1_server_game_state_queue.md#variable-stats)**  |

## Additional inherited members

**Public Functions inherited from [tec::EventQueue< EntityCreated >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
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
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< EntityDestroyed >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |


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


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< EntityDestroyed > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


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

Updated on 17 March 2021 at 23:08:11 UTC