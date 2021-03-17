---
title: tec::PhysicsSystem

---

# tec::PhysicsSystem



Inherits from [tec::CommandQueue< PhysicsSystem >](Classes/classtec_1_1_command_queue.md), [tec::EventQueue< MouseBtnEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< EntityCreated >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< EntityDestroyed >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[PhysicsSystem](Classes/classtec_1_1_physics_system.md#function-physicssystem)**() |
| | **[~PhysicsSystem](Classes/classtec_1_1_physics_system.md#function-~physicssystem)**() |
| void | **[SetSubstepping](Classes/classtec_1_1_physics_system.md#function-setsubstepping)**(int substep) |
| std::set< [eid](Namespaces/namespacetec.md#typedef-eid) > | **[Update](Classes/classtec_1_1_physics_system.md#function-update)**(const double delta, const [GameState](Classes/structtec_1_1_game_state.md) & state) |
| [eid](Namespaces/namespacetec.md#typedef-eid) | **[RayCastMousePick](Classes/classtec_1_1_physics_system.md#function-raycastmousepick)**([eid](Namespaces/namespacetec.md#typedef-eid) source_entity, double mouse_x =0.0f, double mouse_y =0.0f, float screen_width =1.0f, float screen_height =1.0f) |
| [eid](Namespaces/namespacetec.md#typedef-eid) | **[RayCastIgnore](Classes/classtec_1_1_physics_system.md#function-raycastignore)**([eid](Namespaces/namespacetec.md#typedef-eid) source_entity, [eid](Namespaces/namespacetec.md#typedef-eid) ignore_entity) |
| glm::vec3 | **[GetLastRayPos](Classes/classtec_1_1_physics_system.md#function-getlastraypos)**() const |
| double | **[GetLastRayDistance](Classes/classtec_1_1_physics_system.md#function-getlastraydistance)**() const |
| void | **[RaySetInvalid](Classes/classtec_1_1_physics_system.md#function-raysetinvalid)**() |
| void | **[DebugDraw](Classes/classtec_1_1_physics_system.md#function-debugdraw)**() |
| virtual void | **[On](Classes/classtec_1_1_physics_system.md#function-on)**(std::shared_ptr< [MouseBtnEvent](Classes/structtec_1_1_mouse_btn_event.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_physics_system.md#function-on)**(std::shared_ptr< [EntityCreated](Classes/structtec_1_1_entity_created.md) > data) |
| virtual void | **[On](Classes/classtec_1_1_physics_system.md#function-on)**(std::shared_ptr< [EntityDestroyed](Classes/structtec_1_1_entity_destroyed.md) > data) |
| [Position](Classes/structtec_1_1_position.md) | **[GetPosition](Classes/classtec_1_1_physics_system.md#function-getposition)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| [Orientation](Classes/structtec_1_1_orientation.md) | **[GetOrientation](Classes/classtec_1_1_physics_system.md#function-getorientation)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| void | **[SetGravity](Classes/classtec_1_1_physics_system.md#function-setgravity)**(const unsigned int entity_id, const btVector3 & f)<br>Set a rigid body's gravity.  |
| void | **[SetNormalGravity](Classes/classtec_1_1_physics_system.md#function-setnormalgravity)**(const unsigned int entity_id)<br>Set a rigid body's gravity to the world's gravity.  |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< PhysicsSystem >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](Classes/classtec_1_1_command_queue.md#function-commandqueue)**() |
| | **[~CommandQueue](Classes/classtec_1_1_command_queue.md#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](Classes/classtec_1_1_command_queue.md#function-processcommandqueue)**() |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**([Command](Classes/structtec_1_1_command.md)< T > && command) |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< PhysicsSystem >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * > | **[global_command_queue](Classes/classtec_1_1_command_queue.md#variable-global_command_queue)**  |
| std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * | **[local_queue](Classes/classtec_1_1_command_queue.md#variable-local_queue)**  |

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
    std::shared_ptr< MouseBtnEvent > data
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

Updated on 17 March 2021 at 23:08:11 UTC