---
title: tec::Simulation

---

# tec::Simulation



Inherits from [tec::CommandQueue< Simulation >](Classes/classtec_1_1_command_queue.md), [tec::EventQueue< KeyboardEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< MouseBtnEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< MouseMoveEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< MouseClickEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< ClientCommandsEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< ControllerAddedEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< ControllerRemovedEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< FocusCapturedEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< FocusBlurEvent >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| [GameState](Classes/structtec_1_1_game_state.md) | **[Simulate](Classes/classtec_1_1_simulation.md#function-simulate)**(const double delta_time, [GameState](Classes/structtec_1_1_game_state.md) & interpolated_state) |
| [PhysicsSystem](Classes/classtec_1_1_physics_system.md) & | **[GetPhysicsSystem](Classes/classtec_1_1_simulation.md#function-getphysicssystem)**() |
| [VComputerSystem](Classes/classtec_1_1_v_computer_system.md) & | **[GetVComputerSystem](Classes/classtec_1_1_simulation.md#function-getvcomputersystem)**() |
| void | **[AddController](Classes/classtec_1_1_simulation.md#function-addcontroller)**([Controller](Classes/structtec_1_1_controller.md) * controller) |
| void | **[RemoveController](Classes/classtec_1_1_simulation.md#function-removecontroller)**([Controller](Classes/structtec_1_1_controller.md) * controller) |
| virtual void | **[On](Classes/classtec_1_1_simulation.md#function-on)**(std::shared_ptr< [KeyboardEvent](Classes/structtec_1_1_keyboard_event.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_simulation.md#function-on)**(std::shared_ptr< [MouseBtnEvent](Classes/structtec_1_1_mouse_btn_event.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_simulation.md#function-on)**(std::shared_ptr< [MouseMoveEvent](Classes/structtec_1_1_mouse_move_event.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_simulation.md#function-on)**(std::shared_ptr< [MouseClickEvent](Classes/structtec_1_1_mouse_click_event.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_simulation.md#function-on)**(std::shared_ptr< [ClientCommandsEvent](Classes/structtec_1_1_client_commands_event.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_simulation.md#function-on)**(std::shared_ptr< [ControllerAddedEvent](Classes/structtec_1_1_controller_added_event.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_simulation.md#function-on)**(std::shared_ptr< [ControllerRemovedEvent](Classes/structtec_1_1_controller_removed_event.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_simulation.md#function-on)**(std::shared_ptr< [FocusCapturedEvent](Classes/structtec_1_1_focus_captured_event.md) > data)<br>This event is sent to indicate that focus had been captured by an entity it specifies which was captured: keyboard or mouse, if either of these parameters is true, then prevent normal processing of those events a controller implementing this should logically AND NOT these inputs with it's current settings.  |
| virtual void | **[On](Classes/classtec_1_1_simulation.md#function-on)**(std::shared_ptr< [FocusBlurEvent](Classes/structtec_1_1_focus_blur_event.md) > data)<br>This event is sent to indicate that focus had been released (blur) from an owning entity it specifies which was blurred: keyboard or mouse, if either of these parameters is true, then restore normal processing of those events a controller implementing this should logically OR these inputs with it's current settings.  |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< Simulation >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](Classes/classtec_1_1_command_queue.md#function-commandqueue)**() |
| | **[~CommandQueue](Classes/classtec_1_1_command_queue.md#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](Classes/classtec_1_1_command_queue.md#function-processcommandqueue)**() |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**([Command](Classes/structtec_1_1_command.md)< T > && command) |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< Simulation >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * > | **[global_command_queue](Classes/classtec_1_1_command_queue.md#variable-global_command_queue)**  |
| std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * | **[local_queue](Classes/classtec_1_1_command_queue.md#variable-local_queue)**  |

**Public Functions inherited from [tec::EventQueue< KeyboardEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< KeyboardEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseBtnEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseBtnEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseMoveEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseMoveEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseClickEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseClickEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< ClientCommandsEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< ClientCommandsEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< ControllerAddedEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< ControllerAddedEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< ControllerRemovedEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< ControllerRemovedEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< FocusCapturedEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< FocusCapturedEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< FocusBlurEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< FocusBlurEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |


## Public Functions Documentation

### function Simulate

```cpp
GameState Simulate(
    const double delta_time,
    GameState & interpolated_state
)
```


### function GetPhysicsSystem

```cpp
inline PhysicsSystem & GetPhysicsSystem()
```


### function GetVComputerSystem

```cpp
inline VComputerSystem & GetVComputerSystem()
```


### function AddController

```cpp
void AddController(
    Controller * controller
)
```


### function RemoveController

```cpp
void RemoveController(
    Controller * controller
)
```


### function On

```cpp
virtual void On(
    std::shared_ptr< KeyboardEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< MouseBtnEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< MouseMoveEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< MouseClickEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< ClientCommandsEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< ControllerAddedEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< ControllerRemovedEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< FocusCapturedEvent > data
)
```

This event is sent to indicate that focus had been captured by an entity it specifies which was captured: keyboard or mouse, if either of these parameters is true, then prevent normal processing of those events a controller implementing this should logically AND NOT these inputs with it's current settings. 

**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function On

```cpp
virtual void On(
    std::shared_ptr< FocusBlurEvent > data
)
```

This event is sent to indicate that focus had been released (blur) from an owning entity it specifies which was blurred: keyboard or mouse, if either of these parameters is true, then restore normal processing of those events a controller implementing this should logically OR these inputs with it's current settings. 

**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC