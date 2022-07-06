---
title: tec::PhysicsSystem

---

# tec::PhysicsSystem





Inherits from [tec::CommandQueue< PhysicsSystem >](/engine/Classes/classtec_1_1_command_queue/), [tec::EventQueue< MouseBtnEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< EntityCreated >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< EntityDestroyed >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[PhysicsSystem](/engine/Classes/classtec_1_1_physics_system/#function-physicssystem)**() |
| | **[~PhysicsSystem](/engine/Classes/classtec_1_1_physics_system/#function-~physicssystem)**() |
| void | **[SetSubstepping](/engine/Classes/classtec_1_1_physics_system/#function-setsubstepping)**(int substep) |
| std::set< [eid](/engine/Namespaces/namespacetec/#typedef-eid) > | **[Update](/engine/Classes/classtec_1_1_physics_system/#function-update)**(const double delta, const [GameState](/engine/Classes/structtec_1_1_game_state/) & state) |
| [eid](/engine/Namespaces/namespacetec/#typedef-eid) | **[RayCastMousePick](/engine/Classes/classtec_1_1_physics_system/#function-raycastmousepick)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) source_entity, double mouse_x =0.0f, double mouse_y =0.0f, float screen_width =1.0f, float screen_height =1.0f) |
| [eid](/engine/Namespaces/namespacetec/#typedef-eid) | **[RayCastIgnore](/engine/Classes/classtec_1_1_physics_system/#function-raycastignore)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) source_entity, [eid](/engine/Namespaces/namespacetec/#typedef-eid) ignore_entity) |
| glm::vec3 | **[GetLastRayPos](/engine/Classes/classtec_1_1_physics_system/#function-getlastraypos)**() const |
| double | **[GetLastRayDistance](/engine/Classes/classtec_1_1_physics_system/#function-getlastraydistance)**() const |
| void | **[RaySetInvalid](/engine/Classes/classtec_1_1_physics_system/#function-raysetinvalid)**() |
| void | **[DebugDraw](/engine/Classes/classtec_1_1_physics_system/#function-debugdraw)**() |
| virtual void | **[On](/engine/Classes/classtec_1_1_physics_system/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [MouseBtnEvent](/engine/Classes/structtec_1_1_mouse_btn_event/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_physics_system/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) , std::shared_ptr< [EntityCreated](/engine/Classes/structtec_1_1_entity_created/) > data) override |
| virtual void | **[On](/engine/Classes/classtec_1_1_physics_system/#function-on)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::shared_ptr< [EntityDestroyed](/engine/Classes/structtec_1_1_entity_destroyed/) > data) override |
| [Position](/engine/Classes/structtec_1_1_position/) | **[GetPosition](/engine/Classes/classtec_1_1_physics_system/#function-getposition)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| [Orientation](/engine/Classes/structtec_1_1_orientation/) | **[GetOrientation](/engine/Classes/classtec_1_1_physics_system/#function-getorientation)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| void | **[SetGravity](/engine/Classes/classtec_1_1_physics_system/#function-setgravity)**(const unsigned int entity_id, const btVector3 & f)<br>Set a rigid body's gravity.  |
| void | **[SetNormalGravity](/engine/Classes/classtec_1_1_physics_system/#function-setnormalgravity)**(const unsigned int entity_id)<br>Set a rigid body's gravity to the world's gravity.  |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< PhysicsSystem >](/engine/Classes/classtec_1_1_command_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-commandqueue)**() |
| | **[~CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-processcommandqueue)**() |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**([Command](/engine/Classes/structtec_1_1_command/)< T > && command) |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< PhysicsSystem >](/engine/Classes/classtec_1_1_command_queue/)**

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * > | **[global_command_queue](/engine/Classes/classtec_1_1_command_queue/#variable-global-command-queue)**  |
| std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * | **[local_queue](/engine/Classes/classtec_1_1_command_queue/#variable-local-queue)**  |

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

### function PhysicsSystem

```cpp
PhysicsSystem()
```


### function ~PhysicsSystem

```cpp
~PhysicsSystem()
```


### function SetSubstepping

```cpp
inline void SetSubstepping(
    int substep
)
```


### function Update

```cpp
std::set< eid > Update(
    const double delta,
    const GameState & state
)
```


### function RayCastMousePick

```cpp
eid RayCastMousePick(
    eid source_entity,
    double mouse_x =0.0f,
    double mouse_y =0.0f,
    float screen_width =1.0f,
    float screen_height =1.0f
)
```


### function RayCastIgnore

```cpp
eid RayCastIgnore(
    eid source_entity,
    eid ignore_entity
)
```


### function GetLastRayPos

```cpp
inline glm::vec3 GetLastRayPos() const
```


### function GetLastRayDistance

```cpp
inline double GetLastRayDistance() const
```


### function RaySetInvalid

```cpp
inline void RaySetInvalid()
```


### function DebugDraw

```cpp
void DebugDraw()
```


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


### function GetPosition

```cpp
Position GetPosition(
    eid entity_id
)
```


### function GetOrientation

```cpp
Orientation GetOrientation(
    eid entity_id
)
```


## Protected Functions Documentation

### function SetGravity

```cpp
void SetGravity(
    const unsigned int entity_id,
    const btVector3 & f
)
```

Set a rigid body's gravity. 

**Parameters**: 

  * **const** unsigned int entity_id The entity ID of the rigid body. 
  * **btVector3** f The rigid body's new gravity. 


### function SetNormalGravity

```cpp
void SetNormalGravity(
    const unsigned int entity_id
)
```

Set a rigid body's gravity to the world's gravity. 

**Parameters**: 

  * **const** unsigned int entity_id The entity ID of the rigid body. 


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000