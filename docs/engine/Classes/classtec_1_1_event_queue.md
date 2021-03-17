---
title: tec::EventQueue

---

# tec::EventQueue



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |
| virtual void | **[On](Classes/classtec_1_1_event_queue.md#function-on)**(const eid, std::shared_ptr< T > ) |
| virtual void | **[On](Classes/classtec_1_1_event_queue.md#function-on)**(std::shared_ptr< T > ) |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

## Detailed Description

```cpp
template <class T >
class tec::EventQueue;
```

## Public Functions Documentation

### function EventQueue

```cpp
inline EventQueue()
```


### function EventQueue

```cpp
inline EventQueue(
    eid entity_id
)
```


### function ~EventQueue

```cpp
inline virtual ~EventQueue()
```


### function ProcessEventQueue

```cpp
inline void ProcessEventQueue()
```


### function QueueEvent

```cpp
inline void QueueEvent(
    Event< T > && e
)
```


### function On

```cpp
inline virtual void On(
    const eid,
    std::shared_ptr< T > 
)
```


### function On

```cpp
inline virtual void On(
    std::shared_ptr< T > 
)
```


**Reimplemented by**: [tec::LuaSystem::On](Classes/classtec_1_1_lua_system.md#function-on), [tec::Simulation::On](Classes/classtec_1_1_simulation.md#function-on), [tec::Simulation::On](Classes/classtec_1_1_simulation.md#function-on), [tec::Simulation::On](Classes/classtec_1_1_simulation.md#function-on), [tec::ClientGameStateQueue::On](Classes/classtec_1_1_client_game_state_queue.md#function-on), [tec::RenderSystem::On](Classes/classtec_1_1_render_system.md#function-on), [tec::SoundSystem::On](Classes/classtec_1_1_sound_system.md#function-on), [tec::LuaSystem::On](Classes/classtec_1_1_lua_system.md#function-on), [tec::PhysicsSystem::On](Classes/classtec_1_1_physics_system.md#function-on), [tec::VComputerSystem::On](Classes/classtec_1_1_v_computer_system.md#function-on), [tec::ServerGameStateQueue::On](Classes/classtec_1_1_server_game_state_queue.md#function-on), [tec::networking::Server::On](Classes/classtec_1_1networking_1_1_server.md#function-on), [tec::ClientGameStateQueue::On](Classes/classtec_1_1_client_game_state_queue.md#function-on), [tec::RenderSystem::On](Classes/classtec_1_1_render_system.md#function-on), [tec::SoundSystem::On](Classes/classtec_1_1_sound_system.md#function-on), [tec::LuaSystem::On](Classes/classtec_1_1_lua_system.md#function-on), [tec::PhysicsSystem::On](Classes/classtec_1_1_physics_system.md#function-on), [tec::VComputerSystem::On](Classes/classtec_1_1_v_computer_system.md#function-on), [tec::ServerGameStateQueue::On](Classes/classtec_1_1_server_game_state_queue.md#function-on), [tec::networking::Server::On](Classes/classtec_1_1networking_1_1_server.md#function-on), [tec::Simulation::On](Classes/classtec_1_1_simulation.md#function-on), [tec::Simulation::On](Classes/classtec_1_1_simulation.md#function-on), [tec::IMGUISystem::On](Classes/classtec_1_1_i_m_g_u_i_system.md#function-on), [tec::OS::On](Classes/classtec_1_1_o_s.md#function-on), [tec::Simulation::On](Classes/classtec_1_1_simulation.md#function-on), [tec::VComputerSystem::On](Classes/classtec_1_1_v_computer_system.md#function-on), [tec::Console::On](Classes/classtec_1_1_console.md#function-on), [tec::IMGUISystem::On](Classes/classtec_1_1_i_m_g_u_i_system.md#function-on), [tec::PhysicsSystem::On](Classes/classtec_1_1_physics_system.md#function-on), [tec::Simulation::On](Classes/classtec_1_1_simulation.md#function-on), [tec::VComputerSystem::On](Classes/classtec_1_1_v_computer_system.md#function-on), [tec::VoxelVolume::On](Classes/classtec_1_1_voxel_volume.md#function-on), [tec::Simulation::On](Classes/classtec_1_1_simulation.md#function-on), [tec::VComputerSystem::On](Classes/classtec_1_1_v_computer_system.md#function-on), [tec::IMGUISystem::On](Classes/classtec_1_1_i_m_g_u_i_system.md#function-on), [tec::Simulation::On](Classes/classtec_1_1_simulation.md#function-on), [tec::IMGUISystem::On](Classes/classtec_1_1_i_m_g_u_i_system.md#function-on), [tec::ClientGameStateQueue::On](Classes/classtec_1_1_client_game_state_queue.md#function-on), [tec::IMGUISystem::On](Classes/classtec_1_1_i_m_g_u_i_system.md#function-on), [tec::RenderSystem::On](Classes/classtec_1_1_render_system.md#function-on), [tec::Console::On](Classes/classtec_1_1_console.md#function-on)


## Protected Attributes Documentation

### variable read_event_queue

```cpp
Queue< Event< T > > * read_event_queue;
```


### variable write_event_queue

```cpp
std::atomic< Queue< Event< T > > * > write_event_queue;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC