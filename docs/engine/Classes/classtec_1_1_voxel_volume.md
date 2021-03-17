---
title: tec::VoxelVolume

---

# tec::VoxelVolume



Inherits from [tec::CommandQueue< VoxelVolume >](/engine/Classes/classtec_1_1_command_queue/), [tec::EventQueue< MouseClickEvent >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[VoxelVolume](/engine/Classes/classtec_1_1_voxel_volume/#function-voxelvolume)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::weak_ptr< [MeshFile](/engine/Classes/classtec_1_1_mesh_file/) > mesh) |
| | **[~VoxelVolume](/engine/Classes/classtec_1_1_voxel_volume/#function-~voxelvolume)**() |
| void | **[Update](/engine/Classes/classtec_1_1_voxel_volume/#function-update)**(double delta) |
| void | **[Out](/engine/Classes/classtec_1_1_voxel_volume/#function-out)**(proto::Component * ) |
| void | **[UpdateMesh](/engine/Classes/classtec_1_1_voxel_volume/#function-updatemesh)**() |
| std::weak_ptr< [MeshFile](/engine/Classes/classtec_1_1_mesh_file/) > | **[GetMesh](/engine/Classes/classtec_1_1_voxel_volume/#function-getmesh)**() |
| void | **[AddVoxel](/engine/Classes/classtec_1_1_voxel_volume/#function-addvoxel)**(const std::int16_t row, const std::int16_t column, const std::int16_t slice) |
| void | **[RemoveVoxel](/engine/Classes/classtec_1_1_voxel_volume/#function-removevoxel)**(const std::int16_t row, const std::int16_t column, const std::int16_t slice) |
| virtual void | **[On](/engine/Classes/classtec_1_1_voxel_volume/#function-on)**(std::shared_ptr< [MouseClickEvent](/engine/Classes/structtec_1_1_mouse_click_event/) > data) |
| std::weak_ptr< [VoxelVolume](/engine/Classes/classtec_1_1_voxel_volume/) > | **[Create](/engine/Classes/classtec_1_1_voxel_volume/#function-create)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, const std::string name) |
| std::weak_ptr< [VoxelVolume](/engine/Classes/classtec_1_1_voxel_volume/) > | **[Create](/engine/Classes/classtec_1_1_voxel_volume/#function-create)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::weak_ptr< [MeshFile](/engine/Classes/classtec_1_1_mesh_file/) > mesh =std::weak_ptr< [MeshFile](/engine/Classes/classtec_1_1_mesh_file/) >()) |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< VoxelVolume >](/engine/Classes/classtec_1_1_command_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-commandqueue)**() |
| | **[~CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-processcommandqueue)**() |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**([Command](/engine/Classes/structtec_1_1_command/)< T > && command) |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< VoxelVolume >](/engine/Classes/classtec_1_1_command_queue/)**

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * > | **[global_command_queue](/engine/Classes/classtec_1_1_command_queue/#variable-global_command_queue)**  |
| std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * | **[local_queue](/engine/Classes/classtec_1_1_command_queue/#variable-local_queue)**  |

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
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read_event_queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write_event_queue)**  |


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


**Reimplements**: [tec::EventQueue::On](/engine/Classes/classtec_1_1_event_queue/#function-on)


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

Updated on 17 March 2021 at 23:25:12 UTC