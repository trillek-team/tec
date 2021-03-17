---
title: tec::VoxelVolume

---

# tec::VoxelVolume



Inherits from [tec::CommandQueue< VoxelVolume >](Classes/classtec_1_1_command_queue.md), [tec::EventQueue< MouseClickEvent >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[VoxelVolume](Classes/classtec_1_1_voxel_volume.md#function-voxelvolume)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id, std::weak_ptr< [MeshFile](Classes/classtec_1_1_mesh_file.md) > mesh) |
| | **[~VoxelVolume](Classes/classtec_1_1_voxel_volume.md#function-~voxelvolume)**() |
| void | **[Update](Classes/classtec_1_1_voxel_volume.md#function-update)**(double delta) |
| void | **[Out](Classes/classtec_1_1_voxel_volume.md#function-out)**(proto::Component * ) |
| void | **[UpdateMesh](Classes/classtec_1_1_voxel_volume.md#function-updatemesh)**() |
| std::weak_ptr< [MeshFile](Classes/classtec_1_1_mesh_file.md) > | **[GetMesh](Classes/classtec_1_1_voxel_volume.md#function-getmesh)**() |
| void | **[AddVoxel](Classes/classtec_1_1_voxel_volume.md#function-addvoxel)**(const std::int16_t row, const std::int16_t column, const std::int16_t slice) |
| void | **[RemoveVoxel](Classes/classtec_1_1_voxel_volume.md#function-removevoxel)**(const std::int16_t row, const std::int16_t column, const std::int16_t slice) |
| virtual void | **[On](Classes/classtec_1_1_voxel_volume.md#function-on)**(std::shared_ptr< [MouseClickEvent](Classes/structtec_1_1_mouse_click_event.md) > data) |
| std::weak_ptr< [VoxelVolume](Classes/classtec_1_1_voxel_volume.md) > | **[Create](Classes/classtec_1_1_voxel_volume.md#function-create)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id, const std::string name) |
| std::weak_ptr< [VoxelVolume](Classes/classtec_1_1_voxel_volume.md) > | **[Create](Classes/classtec_1_1_voxel_volume.md#function-create)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id, std::weak_ptr< [MeshFile](Classes/classtec_1_1_mesh_file.md) > mesh =std::weak_ptr< [MeshFile](Classes/classtec_1_1_mesh_file.md) >()) |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< VoxelVolume >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](Classes/classtec_1_1_command_queue.md#function-commandqueue)**() |
| | **[~CommandQueue](Classes/classtec_1_1_command_queue.md#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](Classes/classtec_1_1_command_queue.md#function-processcommandqueue)**() |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**([Command](Classes/structtec_1_1_command.md)< T > && command) |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< VoxelVolume >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * > | **[global_command_queue](Classes/classtec_1_1_command_queue.md#variable-global_command_queue)**  |
| std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * | **[local_queue](Classes/classtec_1_1_command_queue.md#variable-local_queue)**  |

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


## Public Functions Documentation

### function VoxelVolume

```cpp
VoxelVolume(
    const eid entity_id,
    std::weak_ptr< MeshFile > mesh
)
```


### function ~VoxelVolume

```cpp
~VoxelVolume()
```


### function Update

```cpp
void Update(
    double delta
)
```


### function Out

```cpp
inline void Out(
    proto::Component * 
)
```


### function UpdateMesh

```cpp
void UpdateMesh()
```


### function GetMesh

```cpp
std::weak_ptr< MeshFile > GetMesh()
```


### function AddVoxel

```cpp
void AddVoxel(
    const std::int16_t row,
    const std::int16_t column,
    const std::int16_t slice
)
```


### function RemoveVoxel

```cpp
void RemoveVoxel(
    const std::int16_t row,
    const std::int16_t column,
    const std::int16_t slice
)
```


### function On

```cpp
virtual void On(
    std::shared_ptr< MouseClickEvent > data
)
```


**Reimplements**: [tec::EventQueue::On](Classes/classtec_1_1_event_queue.md#function-on)


### function Create

```cpp
static std::weak_ptr< VoxelVolume > Create(
    const eid entity_id,
    const std::string name
)
```


### function Create

```cpp
static std::weak_ptr< VoxelVolume > Create(
    const eid entity_id,
    std::weak_ptr< MeshFile > mesh =std::weak_ptr< MeshFile >()
)
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC