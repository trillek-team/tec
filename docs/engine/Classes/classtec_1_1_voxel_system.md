---
title: tec::VoxelSystem

---

# tec::VoxelSystem





Inherits from [tec::EventQueue< MouseClickEvent >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[Update](/engine/Classes/classtec_1_1_voxel_system/#function-update)**(double delta) |
| virtual void | **[On](/engine/Classes/classtec_1_1_voxel_system/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::shared_ptr< [MouseClickEvent](/engine/Classes/structtec_1_1_mouse_click_event/) > data) override |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[edit_allowed](/engine/Classes/classtec_1_1_voxel_system/#variable-edit-allowed)**  |

## Additional inherited members

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


## Public Functions Documentation

### function Update

```cpp
void Update(
    double delta
)
```


### function On

```cpp
virtual void On(
    eid entity_id,
    std::shared_ptr< MouseClickEvent > data
) override
```


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


## Public Attributes Documentation

### variable edit_allowed

```cpp
bool edit_allowed {false};
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000