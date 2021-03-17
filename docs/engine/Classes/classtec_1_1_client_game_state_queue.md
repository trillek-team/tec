---
title: tec::ClientGameStateQueue

---

# tec::ClientGameStateQueue



Inherits from [tec::EventQueue< EntityCreated >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< EntityDestroyed >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< NewGameStateEvent >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ClientGameStateQueue](Classes/classtec_1_1_client_game_state_queue.md#function-clientgamestatequeue)**([ServerStats](Classes/classtec_1_1_server_stats.md) & s) |
| void | **[Interpolate](Classes/classtec_1_1_client_game_state_queue.md#function-interpolate)**(const double delta_time) |
| void | **[QueueServerState](Classes/classtec_1_1_client_game_state_queue.md#function-queueserverstate)**([GameState](Classes/structtec_1_1_game_state.md) && new_state) |
| void | **[UpdatePredictions](Classes/classtec_1_1_client_game_state_queue.md#function-updatepredictions)**([GameState](Classes/structtec_1_1_game_state.md) & new_state)<br>Explicitly update the current prediction.  |
| void | **[CheckPredictionResult](Classes/classtec_1_1_client_game_state_queue.md#function-checkpredictionresult)**([GameState](Classes/structtec_1_1_game_state.md) & new_state) |
| void | **[ProcessEventQueue](Classes/classtec_1_1_client_game_state_queue.md#function-processeventqueue)**() |
| void | **[SetClientID](Classes/classtec_1_1_client_game_state_queue.md#function-setclientid)**([eid](Namespaces/namespacetec.md#typedef-eid) _client_id) |
| void | **[SetCommandID](Classes/classtec_1_1_client_game_state_queue.md#function-setcommandid)**(state_id_t _command_id) |
| virtual void | **[On](Classes/classtec_1_1_client_game_state_queue.md#function-on)**(std::shared_ptr< [EntityCreated](Classes/structtec_1_1_entity_created.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_client_game_state_queue.md#function-on)**(std::shared_ptr< [EntityDestroyed](Classes/structtec_1_1_entity_destroyed.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_client_game_state_queue.md#function-on)**(std::shared_ptr< [NewGameStateEvent](Classes/structtec_1_1_new_game_state_event.md) > data) |
| [GameState](Classes/structtec_1_1_game_state.md) & | **[GetInterpolatedState](Classes/classtec_1_1_client_game_state_queue.md#function-getinterpolatedstate)**() |
| [GameState](Classes/structtec_1_1_game_state.md) & | **[GetBaseState](Classes/classtec_1_1_client_game_state_queue.md#function-getbasestate)**() |
| void | **[SetBaseState](Classes/classtec_1_1_client_game_state_queue.md#function-setbasestate)**([GameState](Classes/structtec_1_1_game_state.md) && new_state) |
| [GameState](Classes/structtec_1_1_game_state.md) * | **[GetGameState](Classes/classtec_1_1_client_game_state_queue.md#function-getgamestate)**(int offset) |

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

**Public Functions inherited from [tec::EventQueue< NewGameStateEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< NewGameStateEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |


## Public Functions Documentation

### function ClientGameStateQueue

```cpp
ClientGameStateQueue(
    ServerStats & s
)
```


### function Interpolate

```cpp
void Interpolate(
    const double delta_time
)
```


### function QueueServerState

```cpp
void QueueServerState(
    GameState && new_state
)
```


### function UpdatePredictions

```cpp
void UpdatePredictions(
    GameState & new_state
)
```

Explicitly update the current prediction. 

### function CheckPredictionResult

```cpp
void CheckPredictionResult(
    GameState & new_state
)
```


### function ProcessEventQueue

```cpp
void ProcessEventQueue()
```


### function SetClientID

```cpp
inline void SetClientID(
    eid _client_id
)
```


### function SetCommandID

```cpp
inline void SetCommandID(
    state_id_t _command_id
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


### function On

```cpp
virtual void On(
    std::shared_ptr< NewGameStateEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function GetInterpolatedState

```cpp
inline GameState & GetInterpolatedState()
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


### function GetGameState

```cpp
inline GameState * GetGameState(
    int offset
)
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC