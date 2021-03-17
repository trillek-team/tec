---
title: tec::RenderSystem

---

# tec::RenderSystem



Inherits from [tec::CommandQueue< RenderSystem >](Classes/classtec_1_1_command_queue.md), [tec::EventQueue< WindowResizedEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< EntityDestroyed >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< EntityCreated >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[Startup](Classes/classtec_1_1_render_system.md#function-startup)**() |
| void | **[SetViewportSize](Classes/classtec_1_1_render_system.md#function-setviewportsize)**(const unsigned int width, const unsigned int height) |
| void | **[Update](Classes/classtec_1_1_render_system.md#function-update)**(const double delta, const [GameState](Classes/structtec_1_1_game_state.md) & state) |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< RenderSystem >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](Classes/classtec_1_1_command_queue.md#function-commandqueue)**() |
| | **[~CommandQueue](Classes/classtec_1_1_command_queue.md#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](Classes/classtec_1_1_command_queue.md#function-processcommandqueue)**() |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**([Command](Classes/structtec_1_1_command.md)< T > && command) |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< RenderSystem >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * > | **[global_command_queue](Classes/classtec_1_1_command_queue.md#variable-global_command_queue)**  |
| std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * | **[local_queue](Classes/classtec_1_1_command_queue.md#variable-local_queue)**  |

**Public Functions inherited from [tec::EventQueue< WindowResizedEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< WindowResizedEvent >](Classes/classtec_1_1_event_queue.md)**

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


## Public Functions Documentation

### function Startup

```cpp
void Startup()
```


### function SetViewportSize

```cpp
void SetViewportSize(
    const unsigned int width,
    const unsigned int height
)
```


### function Update

```cpp
void Update(
    const double delta,
    const GameState & state
)
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC