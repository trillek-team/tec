---
title: tec::EventQueue

---

# tec::EventQueue



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |
| virtual void | **[On](/engine/Classes/classtec_1_1_event_queue/#function-on)**(const eid, std::shared_ptr< T > ) |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

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


**Reimplemented by**: [tec::LuaSystem::On](/engine/Classes/classtec_1_1_lua_system/#function-on), [tec::Simulation::On](/engine/Classes/classtec_1_1_simulation/#function-on), [tec::Simulation::On](/engine/Classes/classtec_1_1_simulation/#function-on), [tec::Simulation::On](/engine/Classes/classtec_1_1_simulation/#function-on), [tec::ClientGameStateQueue::On](/engine/Classes/classtec_1_1_client_game_state_queue/#function-on), [tec::RenderSystem::On](/engine/Classes/classtec_1_1_render_system/#function-on), [tec::SoundSystem::On](/engine/Classes/classtec_1_1_sound_system/#function-on), [tec::LuaSystem::On](/engine/Classes/classtec_1_1_lua_system/#function-on), [tec::PhysicsSystem::On](/engine/Classes/classtec_1_1_physics_system/#function-on), [tec::VComputerSystem::On](/engine/Classes/classtec_1_1_v_computer_system/#function-on), [tec::ServerGameStateQueue::On](/engine/Classes/classtec_1_1_server_game_state_queue/#function-on), [tec::networking::Server::On](/engine/Classes/classtec_1_1networking_1_1_server/#function-on), [tec::ClientGameStateQueue::On](/engine/Classes/classtec_1_1_client_game_state_queue/#function-on), [tec::RenderSystem::On](/engine/Classes/classtec_1_1_render_system/#function-on), [tec::SoundSystem::On](/engine/Classes/classtec_1_1_sound_system/#function-on), [tec::LuaSystem::On](/engine/Classes/classtec_1_1_lua_system/#function-on), [tec::PhysicsSystem::On](/engine/Classes/classtec_1_1_physics_system/#function-on), [tec::VComputerSystem::On](/engine/Classes/classtec_1_1_v_computer_system/#function-on), [tec::ServerGameStateQueue::On](/engine/Classes/classtec_1_1_server_game_state_queue/#function-on), [tec::networking::Server::On](/engine/Classes/classtec_1_1networking_1_1_server/#function-on), [tec::Simulation::On](/engine/Classes/classtec_1_1_simulation/#function-on), [tec::Simulation::On](/engine/Classes/classtec_1_1_simulation/#function-on), [tec::Game::On](/engine/Classes/classtec_1_1_game/#function-on), [tec::Console::On](/engine/Classes/classtec_1_1_console/#function-on), [tec::IMGUISystem::On](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-on), [tec::Simulation::On](/engine/Classes/classtec_1_1_simulation/#function-on), [tec::VComputerSystem::On](/engine/Classes/classtec_1_1_v_computer_system/#function-on), [tec::IMGUISystem::On](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-on), [tec::PhysicsSystem::On](/engine/Classes/classtec_1_1_physics_system/#function-on), [tec::Simulation::On](/engine/Classes/classtec_1_1_simulation/#function-on), [tec::VComputerSystem::On](/engine/Classes/classtec_1_1_v_computer_system/#function-on), [tec::Game::On](/engine/Classes/classtec_1_1_game/#function-on), [tec::VoxelSystem::On](/engine/Classes/classtec_1_1_voxel_system/#function-on), [tec::Simulation::On](/engine/Classes/classtec_1_1_simulation/#function-on), [tec::VComputerSystem::On](/engine/Classes/classtec_1_1_v_computer_system/#function-on), [tec::IMGUISystem::On](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-on), [tec::Simulation::On](/engine/Classes/classtec_1_1_simulation/#function-on), [tec::IMGUISystem::On](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-on), [tec::ClientGameStateQueue::On](/engine/Classes/classtec_1_1_client_game_state_queue/#function-on), [tec::Console::On](/engine/Classes/classtec_1_1_console/#function-on), [tec::IMGUISystem::On](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-on), [tec::RenderSystem::On](/engine/Classes/classtec_1_1_render_system/#function-on)


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

Updated on 2022-07-06 at 04:15:07 +0000