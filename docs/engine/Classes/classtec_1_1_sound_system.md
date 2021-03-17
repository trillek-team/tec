---
title: tec::SoundSystem

---

# tec::SoundSystem



Inherits from [tec::CommandQueue< SoundSystem >](Classes/classtec_1_1_command_queue.md), [tec::EventQueue< EntityCreated >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< EntityDestroyed >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[SoundSystem](Classes/classtec_1_1_sound_system.md#function-soundsystem)**() |
| void | **[Update](Classes/classtec_1_1_sound_system.md#function-update)**() |
| void | **[SetDelta](Classes/classtec_1_1_sound_system.md#function-setdelta)**(const double _delta) |
| void | **[Stop](Classes/classtec_1_1_sound_system.md#function-stop)**() |
| virtual void | **[On](Classes/classtec_1_1_sound_system.md#function-on)**(std::shared_ptr< [EntityCreated](Classes/structtec_1_1_entity_created.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_sound_system.md#function-on)**(std::shared_ptr< [EntityDestroyed](Classes/structtec_1_1_entity_destroyed.md) > data) |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< SoundSystem >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](Classes/classtec_1_1_command_queue.md#function-commandqueue)**() |
| | **[~CommandQueue](Classes/classtec_1_1_command_queue.md#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](Classes/classtec_1_1_command_queue.md#function-processcommandqueue)**() |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**([Command](Classes/structtec_1_1_command.md)< T > && command) |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< SoundSystem >](Classes/classtec_1_1_command_queue.md)**

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


## Public Functions Documentation

### function SoundSystem

```cpp
SoundSystem()
```


### function Update

```cpp
void Update()
```


### function SetDelta

```cpp
inline void SetDelta(
    const double _delta
)
```


### function Stop

```cpp
inline void Stop()
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


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC