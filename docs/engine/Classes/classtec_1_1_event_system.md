---
title: tec::EventSystem

---

# tec::EventSystem



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [EventSystem](Classes/classtec_1_1_event_system.md)< T > > | **[Get](Classes/classtec_1_1_event_system.md#function-get)**() |
| | **[~EventSystem](Classes/classtec_1_1_event_system.md#function-~eventsystem)**() |
| void | **[Subscribe](Classes/classtec_1_1_event_system.md#function-subscribe)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id, [EventQueue](Classes/classtec_1_1_event_queue.md)< T > * subscriber)<br>Subscribes to be notified of events.  |
| void | **[Subscribe](Classes/classtec_1_1_event_system.md#function-subscribe)**([EventQueue](Classes/classtec_1_1_event_queue.md)< T > * subscriber)<br>Subscribes to be notified of events for all entity IDs.  |
| void | **[Unsubscribe](Classes/classtec_1_1_event_system.md#function-unsubscribe)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id, [EventQueue](Classes/classtec_1_1_event_queue.md)< T > * subscriber)<br>Unsubscribes to notification of events.  |
| void | **[Unsubscribe](Classes/classtec_1_1_event_system.md#function-unsubscribe)**([EventQueue](Classes/classtec_1_1_event_queue.md)< T > * subscriber)<br>Unsubscribes to notification of events.  |
| void | **[Emit](Classes/classtec_1_1_event_system.md#function-emit)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id, std::shared_ptr< T > data)<br>Emits an event to subscribers for a given entity_id and to all subscribers listening for events for any entity_id.  |
| void | **[Emit](Classes/classtec_1_1_event_system.md#function-emit)**(std::shared_ptr< T > data)<br>Emits an event to all subscribers listening for events for any entity_id.  |

## Detailed Description

```cpp
template <typename T >
class tec::EventSystem;
```

## Public Functions Documentation

### function Get

```cpp
static inline std::shared_ptr< EventSystem< T > > Get()
```


### function ~EventSystem

```cpp
inline ~EventSystem()
```


### function Subscribe

```cpp
inline void Subscribe(
    const eid entity_id,
    EventQueue< T > * subscriber
)
```

Subscribes to be notified of events. 

**Parameters**: 

  * **const** unsigned int entity_id ID of the entity to subscribe to. 
  * **const** EventQueue<T>* subscriber The subscriber to add. 


**Return**: void 

### function Subscribe

```cpp
inline void Subscribe(
    EventQueue< T > * subscriber
)
```

Subscribes to be notified of events for all entity IDs. 

**Parameters**: 

  * **const** Receiver<T>* subscriber The subscriber to add. 


**Return**: void 

### function Unsubscribe

```cpp
inline void Unsubscribe(
    const eid entity_id,
    EventQueue< T > * subscriber
)
```

Unsubscribes to notification of events. 

**Parameters**: 

  * **const** unsigned int entity_id ID of the entity to unsubscribe from. 
  * **const** Receiver<T>* subscriber The subscriber to remove. 


**Return**: void 

### function Unsubscribe

```cpp
inline void Unsubscribe(
    EventQueue< T > * subscriber
)
```

Unsubscribes to notification of events. 

**Parameters**: 

  * **const** Receiver<T>* subscriber The subscriber to remove. 


**Return**: void 

### function Emit

```cpp
inline void Emit(
    const eid entity_id,
    std::shared_ptr< T > data
)
```

Emits an event to subscribers for a given entity_id and to all subscribers listening for events for any entity_id. 

**Parameters**: 

  * **const** unsigned int entity_id ID of the entity the event happen to. 
  * **const** T* data The event data. 


**Return**: void 

### function Emit

```cpp
inline void Emit(
    std::shared_ptr< T > data
)
```

Emits an event to all subscribers listening for events for any entity_id. 

**Parameters**: 

  * **const** T* data The changed data. 


**Return**: void 

-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC