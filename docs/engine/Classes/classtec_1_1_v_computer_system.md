---
title: tec::VComputerSystem

---

# tec::VComputerSystem



Inherits from [tec::CommandQueue< VComputerSystem >](Classes/classtec_1_1_command_queue.md), [tec::EventQueue< KeyboardEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< MouseBtnEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< MouseClickEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< EntityCreated >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< EntityDestroyed >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[VComputerSystem](Classes/classtec_1_1_v_computer_system.md#function-vcomputersystem)**() |
| void | **[SetDevice](Classes/classtec_1_1_v_computer_system.md#function-setdevice)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id, const unsigned int slot, std::shared_ptr< [DeviceBase](Classes/structtec_1_1_device_base.md) > device)<br>Sets the specified device for the entity ID to device.  |
| void | **[RemoveDevice](Classes/classtec_1_1_v_computer_system.md#function-removedevice)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id, const unsigned int slot)<br>Remove a device from the specified slot.  |
| bool | **[LoadROMFile](Classes/classtec_1_1_v_computer_system.md#function-loadromfile)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id, std::string fname)<br>Load a ROM file from disk for the specified computer.  |
| void | **[TurnComptuerOn](Classes/classtec_1_1_v_computer_system.md#function-turncomptueron)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id)<br>Turns specified the computer on.  |
| void | **[TurnComptuerOff](Classes/classtec_1_1_v_computer_system.md#function-turncomptueroff)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id)<br>Turns specified the computer off.  |
| void | **[Update](Classes/classtec_1_1_v_computer_system.md#function-update)**(double delta)<br>Handle incoming events to update data.  |
| virtual void | **[On](Classes/classtec_1_1_v_computer_system.md#function-on)**(std::shared_ptr< [KeyboardEvent](Classes/structtec_1_1_keyboard_event.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_v_computer_system.md#function-on)**(std::shared_ptr< [MouseBtnEvent](Classes/structtec_1_1_mouse_btn_event.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_v_computer_system.md#function-on)**(std::shared_ptr< [MouseClickEvent](Classes/structtec_1_1_mouse_click_event.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_v_computer_system.md#function-on)**(std::shared_ptr< [EntityCreated](Classes/structtec_1_1_entity_created.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_v_computer_system.md#function-on)**(std::shared_ptr< [EntityDestroyed](Classes/structtec_1_1_entity_destroyed.md) > data) |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< VComputerSystem >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](Classes/classtec_1_1_command_queue.md#function-commandqueue)**() |
| | **[~CommandQueue](Classes/classtec_1_1_command_queue.md#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](Classes/classtec_1_1_command_queue.md#function-processcommandqueue)**() |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**([Command](Classes/structtec_1_1_command.md)< T > && command) |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< VComputerSystem >](Classes/classtec_1_1_command_queue.md)**

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

### function VComputerSystem

```cpp
VComputerSystem()
```


### function SetDevice

```cpp
void SetDevice(
    const eid entity_id,
    const unsigned int slot,
    std::shared_ptr< DeviceBase > device
)
```

Sets the specified device for the entity ID to device. 

**Parameters**: 

  * **const** eid entityID The entity ID the computer/CPU belongs to. 
  * **const** unsigned int The slot to assign device to. 
  * **std::shared_ptr<IDevice>** device The device to install. 


### function RemoveDevice

```cpp
void RemoveDevice(
    const eid entity_id,
    const unsigned int slot
)
```

Remove a device from the specified slot. 

**Parameters**: 

  * **const** eid entityID The entity ID the computer/CPU belongs to. 
  * **const** unsigned int The slot to remove the device from. 


### function LoadROMFile

```cpp
bool LoadROMFile(
    const eid entity_id,
    std::string fname
)
```

Load a ROM file from disk for the specified computer. 

**Parameters**: 

  * **const** eid entityID The entity ID the computer belongs to. 
  * **const** std::string fname The name of the ROM file to load. 


**Return**: bool Whether or not the file loaded successfully (also returns false if no computer exists for the given entity_id). 

### function TurnComptuerOn

```cpp
void TurnComptuerOn(
    const eid entity_id
)
```

Turns specified the computer on. 

**Parameters**: 

  * **const** eid entityID The entity ID the computer belongs to. 


### function TurnComptuerOff

```cpp
void TurnComptuerOff(
    const eid entity_id
)
```

Turns specified the computer off. 

**Parameters**: 

  * **const** eid entityID The entity ID the computer belongs to. 


### function Update

```cpp
void Update(
    double delta
)
```

Handle incoming events to update data. 

This function is called once every frame. It is the only function that can write data. This function is in the critical path, job done here must be simple.

If event handling need some batch processing, a task list must be prepared and stored temporarily to be retrieved by RunBatch(). 


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
    std::shared_ptr< MouseClickEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


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