---
title: tec::ClientGameStateQueue

---

# tec::ClientGameStateQueue





Inherits from [tec::EventQueue< EntityCreated >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< EntityDestroyed >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< NewGameStateEvent >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ClientGameStateQueue](/engine/Classes/classtec_1_1_client_game_state_queue/#function-clientgamestatequeue)**([ServerStats](/engine/Classes/classtec_1_1_server_stats/) & s) |
| void | **[Interpolate](/engine/Classes/classtec_1_1_client_game_state_queue/#function-interpolate)**(const double delta_time) |
| void | **[QueueServerState](/engine/Classes/classtec_1_1_client_game_state_queue/#function-queueserverstate)**([GameState](/engine/Classes/structtec_1_1_game_state/) && new_state) |
| void | **[UpdatePredictions](/engine/Classes/classtec_1_1_client_game_state_queue/#function-updatepredictions)**([GameState](/engine/Classes/structtec_1_1_game_state/) & new_state)<br>Explicitly update the current prediction.  |
| void | **[CheckPredictionResult](/engine/Classes/classtec_1_1_client_game_state_queue/#function-checkpredictionresult)**([GameState](/engine/Classes/structtec_1_1_game_state/) & new_state) |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_client_game_state_queue/#function-processeventqueue)**() |
| void | **[SetClientID](/engine/Classes/classtec_1_1_client_game_state_queue/#function-setclientid)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) _client_id) |
| void | **[SetCommandID](/engine/Classes/classtec_1_1_client_game_state_queue/#function-setcommandid)**(state_id_t _command_id) |
| virtual void | **[On](/engine/Classes/classtec_1_1_client_game_state_queue/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [EntityCreated](/engine/Classes/structtec_1_1_entity_created/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_client_game_state_queue/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::shared_ptr< [EntityDestroyed](/engine/Classes/structtec_1_1_entity_destroyed/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_client_game_state_queue/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [NewGameStateEvent](/engine/Classes/structtec_1_1_new_game_state_event/) > data) override |
| [GameState](/engine/Classes/structtec_1_1_game_state/) & | **[GetInterpolatedState](/engine/Classes/classtec_1_1_client_game_state_queue/#function-getinterpolatedstate)**() |
| [GameState](/engine/Classes/structtec_1_1_game_state/) & | **[GetBaseState](/engine/Classes/classtec_1_1_client_game_state_queue/#function-getbasestate)**() |
| void | **[SetBaseState](/engine/Classes/classtec_1_1_client_game_state_queue/#function-setbasestate)**([GameState](/engine/Classes/structtec_1_1_game_state/) && new_state) |
| [GameState](/engine/Classes/structtec_1_1_game_state/) * | **[GetGameState](/engine/Classes/classtec_1_1_client_game_state_queue/#function-getgamestate)**(int offset) |

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
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

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
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< NewGameStateEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< NewGameStateEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |


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


### function On

```cpp
virtual void On(
    eid ,
    std::shared_ptr< NewGameStateEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


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

Updated on 2022-07-06 at 04:15:07 +0000