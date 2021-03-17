---
title: tec::EventList

---

# tec::EventList



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[EventList](Classes/structtec_1_1_event_list.md#function-eventlist)**() |
| | **[EventList](Classes/structtec_1_1_event_list.md#function-eventlist)**(const [EventList](Classes/structtec_1_1_event_list.md) & ) |
| | **[EventList](Classes/structtec_1_1_event_list.md#function-eventlist)**([EventList](Classes/structtec_1_1_event_list.md) && other) |
| [EventList](Classes/structtec_1_1_event_list.md) & | **[operator=](Classes/structtec_1_1_event_list.md#function-operator=)**(const [EventList](Classes/structtec_1_1_event_list.md) & other) |
| [EventList](Classes/structtec_1_1_event_list.md) & | **[operator=](Classes/structtec_1_1_event_list.md#function-operator=)**([EventList](Classes/structtec_1_1_event_list.md) && other) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::list< [KeyboardEvent](Classes/structtec_1_1_keyboard_event.md) > | **[keyboard_events](Classes/structtec_1_1_event_list.md#variable-keyboard_events)**  |
| std::list< [MouseBtnEvent](Classes/structtec_1_1_mouse_btn_event.md) > | **[mouse_button_events](Classes/structtec_1_1_event_list.md#variable-mouse_button_events)**  |
| std::list< [MouseMoveEvent](Classes/structtec_1_1_mouse_move_event.md) > | **[mouse_move_events](Classes/structtec_1_1_event_list.md#variable-mouse_move_events)**  |
| std::list< [MouseClickEvent](Classes/structtec_1_1_mouse_click_event.md) > | **[mouse_click_events](Classes/structtec_1_1_event_list.md#variable-mouse_click_events)**  |

## Public Functions Documentation

### function EventList

```cpp
inline EventList()
```


### function EventList

```cpp
EventList(
    const EventList & 
)
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
)
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

Updated on 17 March 2021 at 23:08:11 UTC