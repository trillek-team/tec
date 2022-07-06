---
title: tec::Simulation

---

# tec::Simulation





Inherits from [tec::CommandQueue< Simulation >](/engine/Classes/classtec_1_1_command_queue/), [tec::EventQueue< KeyboardEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< MouseBtnEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< MouseMoveEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< MouseClickEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< ClientCommandsEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< ControllerAddedEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< ControllerRemovedEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< FocusCapturedEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< FocusBlurEvent >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Simulation](/engine/Classes/classtec_1_1_simulation/#function-simulation)**() |
| | **[~Simulation](/engine/Classes/classtec_1_1_simulation/#function-~simulation)**() |
| [GameState](/engine/Classes/structtec_1_1_game_state/) | **[Simulate](/engine/Classes/classtec_1_1_simulation/#function-simulate)**(const double delta_time, [GameState](/engine/Classes/structtec_1_1_game_state/) & interpolated_state) |
| [PhysicsSystem](/engine/Classes/classtec_1_1_physics_system/) & | **[GetPhysicsSystem](/engine/Classes/classtec_1_1_simulation/#function-getphysicssystem)**() |
| [VComputerSystem](/engine/Classes/classtec_1_1_v_computer_system/) & | **[GetVComputerSystem](/engine/Classes/classtec_1_1_simulation/#function-getvcomputersystem)**() |
| void | **[AddController](/engine/Classes/classtec_1_1_simulation/#function-addcontroller)**([Controller](/engine/Classes/structtec_1_1_controller/) * controller) |
| void | **[RemoveController](/engine/Classes/classtec_1_1_simulation/#function-removecontroller)**([Controller](/engine/Classes/structtec_1_1_controller/) * controller) |
| virtual void | **[On](/engine/Classes/classtec_1_1_simulation/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [KeyboardEvent](/engine/Classes/structtec_1_1_keyboard_event/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_simulation/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [MouseBtnEvent](/engine/Classes/structtec_1_1_mouse_btn_event/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_simulation/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [MouseMoveEvent](/engine/Classes/structtec_1_1_mouse_move_event/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_simulation/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [MouseClickEvent](/engine/Classes/structtec_1_1_mouse_click_event/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_simulation/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [ClientCommandsEvent](/engine/Classes/structtec_1_1_client_commands_event/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_simulation/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [ControllerAddedEvent](/engine/Classes/structtec_1_1_controller_added_event/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_simulation/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [ControllerRemovedEvent](/engine/Classes/structtec_1_1_controller_removed_event/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_simulation/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [FocusCapturedEvent](/engine/Classes/structtec_1_1_focus_captured_event/) > data) override<br>This event is sent to indicate that focus had been captured by an entity it specifies which was captured: keyboard or mouse, if either of these parameters is true, then prevent normal processing of those events a controller implementing this should logically AND NOT these inputs with it's current settings.  |
| virtual void | **[On](/engine/Classes/classtec_1_1_simulation/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [FocusBlurEvent](/engine/Classes/structtec_1_1_focus_blur_event/) > data) override<br>This event is sent to indicate that focus had been released (blur) from an owning entity it specifies which was blurred: keyboard or mouse, if either of these parameters is true, then restore normal processing of those events a controller implementing this should logically OR these inputs with it's current settings.  |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< Simulation >](/engine/Classes/classtec_1_1_command_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-commandqueue)**() |
| | **[~CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-processcommandqueue)**() |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**([Command](/engine/Classes/structtec_1_1_command/)< T > && command) |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< Simulation >](/engine/Classes/classtec_1_1_command_queue/)**

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * > | **[global_command_queue](/engine/Classes/classtec_1_1_command_queue/#variable-global-command-queue)**  |
| std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * | **[local_queue](/engine/Classes/classtec_1_1_command_queue/#variable-local-queue)**  |

**Public Functions inherited from [tec::EventQueue< KeyboardEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< KeyboardEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseBtnEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseBtnEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseMoveEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseMoveEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseClickEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseClickEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< ClientCommandsEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< ClientCommandsEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< ControllerAddedEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< ControllerAddedEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< ControllerRemovedEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< ControllerRemovedEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< FocusCapturedEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< FocusCapturedEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< FocusBlurEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< FocusBlurEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |


## Public Functions Documentation

### function Simulation

```cpp
Simulation()
```


### function ~Simulation

```cpp
~Simulation()
```


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
    eid ,
    std::shared_ptr< KeyboardEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    eid ,
    std::shared_ptr< MouseBtnEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    eid ,
    std::shared_ptr< MouseMoveEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    eid ,
    std::shared_ptr< MouseClickEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    eid ,
    std::shared_ptr< ClientCommandsEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    eid ,
    std::shared_ptr< ControllerAddedEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    eid ,
    std::shared_ptr< ControllerRemovedEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    eid ,
    std::shared_ptr< FocusCapturedEvent > data
) override
```

This event is sent to indicate that focus had been captured by an entity it specifies which was captured: keyboard or mouse, if either of these parameters is true, then prevent normal processing of those events a controller implementing this should logically AND NOT these inputs with it's current settings. 

**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    eid ,
    std::shared_ptr< FocusBlurEvent > data
) override
```

This event is sent to indicate that focus had been released (blur) from an owning entity it specifies which was blurred: keyboard or mouse, if either of these parameters is true, then restore normal processing of those events a controller implementing this should logically OR these inputs with it's current settings. 

**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000