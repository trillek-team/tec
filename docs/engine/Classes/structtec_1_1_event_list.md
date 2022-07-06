---
title: tec::EventList

---

# tec::EventList





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[EventList](/engine/Classes/structtec_1_1_event_list/#function-eventlist)**() |
| | **[EventList](/engine/Classes/structtec_1_1_event_list/#function-eventlist)**(const [EventList](/engine/Classes/structtec_1_1_event_list/) & ) =delete |
| | **[EventList](/engine/Classes/structtec_1_1_event_list/#function-eventlist)**([EventList](/engine/Classes/structtec_1_1_event_list/) && other) |
| [EventList](/engine/Classes/structtec_1_1_event_list/) & | **[operator=](/engine/Classes/structtec_1_1_event_list/#function-operator=)**(const [EventList](/engine/Classes/structtec_1_1_event_list/) & other) =delete |
| [EventList](/engine/Classes/structtec_1_1_event_list/) & | **[operator=](/engine/Classes/structtec_1_1_event_list/#function-operator=)**([EventList](/engine/Classes/structtec_1_1_event_list/) && other) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::list< [KeyboardEvent](/engine/Classes/structtec_1_1_keyboard_event/) > | **[keyboard_events](/engine/Classes/structtec_1_1_event_list/#variable-keyboard-events)**  |
| std::list< [MouseBtnEvent](/engine/Classes/structtec_1_1_mouse_btn_event/) > | **[mouse_button_events](/engine/Classes/structtec_1_1_event_list/#variable-mouse-button-events)**  |
| std::list< [MouseMoveEvent](/engine/Classes/structtec_1_1_mouse_move_event/) > | **[mouse_move_events](/engine/Classes/structtec_1_1_event_list/#variable-mouse-move-events)**  |
| std::list< [MouseClickEvent](/engine/Classes/structtec_1_1_mouse_click_event/) > | **[mouse_click_events](/engine/Classes/structtec_1_1_event_list/#variable-mouse-click-events)**  |

## Public Functions Documentation

### function EventList

```cpp
inline EventList()
```


### function EventList

```cpp
EventList(
    const EventList & 
) =delete
```


### function EventList

```cpp
inline EventList(
    EventList && other
)
```


### function operator=

```cpp
EventList & operator=(
    const EventList & other
) =delete
```


### function operator=

```cpp
inline EventList & operator=(
    EventList && other
)
```


## Public Attributes Documentation

### variable keyboard_events

```cpp
std::list< KeyboardEvent > keyboard_events;
```


### variable mouse_button_events

```cpp
std::list< MouseBtnEvent > mouse_button_events;
```


### variable mouse_move_events

```cpp
std::list< MouseMoveEvent > mouse_move_events;
```


### variable mouse_click_events

```cpp
std::list< MouseClickEvent > mouse_click_events;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000