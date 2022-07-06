---
title: tec::EventSystem

---

# tec::EventSystem



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [EventSystem](/engine/Classes/classtec_1_1_event_system/)< T > > | **[Get](/engine/Classes/classtec_1_1_event_system/#function-get)**() |
| | **[~EventSystem](/engine/Classes/classtec_1_1_event_system/#function-~eventsystem)**() |
| void | **[Subscribe](/engine/Classes/classtec_1_1_event_system/#function-subscribe)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, [EventQueue](/engine/Classes/classtec_1_1_event_queue/)< T > * subscriber)<br>Subscribes to be notified of events.  |
| void | **[Subscribe](/engine/Classes/classtec_1_1_event_system/#function-subscribe)**([EventQueue](/engine/Classes/classtec_1_1_event_queue/)< T > * subscriber)<br>Subscribes to be notified of events for all entity IDs.  |
| void | **[Unsubscribe](/engine/Classes/classtec_1_1_event_system/#function-unsubscribe)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, [EventQueue](/engine/Classes/classtec_1_1_event_queue/)< T > * subscriber)<br>Unsubscribes to notification of events.  |
| void | **[Unsubscribe](/engine/Classes/classtec_1_1_event_system/#function-unsubscribe)**([EventQueue](/engine/Classes/classtec_1_1_event_queue/)< T > * subscriber)<br>Unsubscribes to notification of events.  |
| void | **[Emit](/engine/Classes/classtec_1_1_event_system/#function-emit)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id, std::shared_ptr< T > data)<br>Emits an event to subscribers for a given entity_id and to all subscribers listening for events for any entity_id.  |
| void | **[Emit](/engine/Classes/classtec_1_1_event_system/#function-emit)**(std::shared_ptr< T > data)<br>Emits an event to all subscribers listening for events for any entity_id.  |

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

Updated on 2022-07-06 at 04:15:07 +0000