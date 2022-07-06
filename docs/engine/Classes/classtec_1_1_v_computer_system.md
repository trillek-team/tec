---
title: tec::VComputerSystem

---

# tec::VComputerSystem





Inherits from [tec::CommandQueue< VComputerSystem >](/engine/Classes/classtec_1_1_command_queue/), [tec::EventQueue< KeyboardEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< MouseBtnEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< MouseClickEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< EntityCreated >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< EntityDestroyed >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[VComputerSystem](/engine/Classes/classtec_1_1_v_computer_system/#function-vcomputersystem)**() |
| void | **[SetDevice](/engine/Classes/classtec_1_1_v_computer_system/#function-setdevice)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, const unsigned int slot, std::shared_ptr< [DeviceBase](/engine/Classes/structtec_1_1_device_base/) > device)<br>Sets the specified device for the entity ID to device.  |
| void | **[RemoveDevice](/engine/Classes/classtec_1_1_v_computer_system/#function-removedevice)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, const unsigned int slot)<br>Remove a device from the specified slot.  |
| bool | **[LoadROMFile](/engine/Classes/classtec_1_1_v_computer_system/#function-loadromfile)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::string fname)<br>Load a ROM file from disk for the specified computer.  |
| void | **[TurnComptuerOn](/engine/Classes/classtec_1_1_v_computer_system/#function-turncomptueron)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id)<br>Turns specified the computer on.  |
| void | **[TurnComptuerOff](/engine/Classes/classtec_1_1_v_computer_system/#function-turncomptueroff)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id)<br>Turns specified the computer off.  |
| void | **[Update](/engine/Classes/classtec_1_1_v_computer_system/#function-update)**(double delta)<br>Handle incoming events to update data.  |
| virtual void | **[On](/engine/Classes/classtec_1_1_v_computer_system/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::shared_ptr< [KeyboardEvent](/engine/Classes/structtec_1_1_keyboard_event/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_v_computer_system/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::shared_ptr< [MouseBtnEvent](/engine/Classes/structtec_1_1_mouse_btn_event/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_v_computer_system/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::shared_ptr< [MouseClickEvent](/engine/Classes/structtec_1_1_mouse_click_event/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_v_computer_system/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [EntityCreated](/engine/Classes/structtec_1_1_entity_created/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_v_computer_system/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::shared_ptr< [EntityDestroyed](/engine/Classes/structtec_1_1_entity_destroyed/) > data) override |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< VComputerSystem >](/engine/Classes/classtec_1_1_command_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-commandqueue)**() |
| | **[~CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-processcommandqueue)**() |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**([Command](/engine/Classes/structtec_1_1_command/)< T > && command) |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< VComputerSystem >](/engine/Classes/classtec_1_1_command_queue/)**

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
    eid entity_id,
    std::shared_ptr< KeyboardEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    eid entity_id,
    std::shared_ptr< MouseBtnEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


### function On

```cpp
virtual void On(
    eid entity_id,
    std::shared_ptr< MouseClickEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


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


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000