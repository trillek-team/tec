---
title: tec::Controller

---

# tec::Controller



Inherited by [tec::FPSController](Classes/structtec_1_1_f_p_s_controller.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Controller](Classes/structtec_1_1_controller.md#function-controller)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual void | **[Update](Classes/structtec_1_1_controller.md#function-update)**(double , [GameState](Classes/structtec_1_1_game_state.md) & , [EventList](Classes/structtec_1_1_event_list.md) & ) |
| virtual | **[~Controller](Classes/structtec_1_1_controller.md#function-~controller)**() =default |
| virtual proto::ClientCommands | **[GetClientCommands](Classes/structtec_1_1_controller.md#function-getclientcommands)**() =0 |
| virtual void | **[ApplyClientCommands](Classes/structtec_1_1_controller.md#function-applyclientcommands)**(proto::ClientCommands ) =0 |
| virtual void | **[SetFocus](Classes/structtec_1_1_controller.md#function-setfocus)**(bool keyboard, bool mouse)<br>called to indicate focus has been restored to controller  |
| virtual void | **[ClearFocus](Classes/structtec_1_1_controller.md#function-clearfocus)**(bool keyboard, bool mouse)<br>called to indicate focus has been captured from controller  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| [eid](Namespaces/namespacetec.md#typedef-eid) | **[entity_id](Classes/structtec_1_1_controller.md#variable-entity_id)**  |
| bool | **[keyboard_focus](Classes/structtec_1_1_controller.md#variable-keyboard_focus)**  |
| bool | **[mouse_focus](Classes/structtec_1_1_controller.md#variable-mouse_focus)**  |

## Public Functions Documentation

### function Controller

```cpp
inline Controller(
    eid entity_id
)
```


### function Update

```cpp
inline virtual void Update(
    double ,
    GameState & ,
    EventList & 
)
```


**Reimplemented by**: [tec::FPSController::Update](Classes/structtec_1_1_f_p_s_controller.md#function-update)


### function ~Controller

```cpp
virtual ~Controller() =default
```


### function GetClientCommands

```cpp
virtual proto::ClientCommands GetClientCommands() =0
```


**Reimplemented by**: [tec::FPSController::GetClientCommands](Classes/structtec_1_1_f_p_s_controller.md#function-getclientcommands)


### function ApplyClientCommands

```cpp
virtual void ApplyClientCommands(
    proto::ClientCommands 
) =0
```


**Reimplemented by**: [tec::FPSController::ApplyClientCommands](Classes/structtec_1_1_f_p_s_controller.md#function-applyclientcommands)


### function SetFocus

```cpp
inline virtual void SetFocus(
    bool keyboard,
    bool mouse
)
```

called to indicate focus has been restored to controller 

### function ClearFocus

```cpp
inline virtual void ClearFocus(
    bool keyboard,
    bool mouse
)
```

called to indicate focus has been captured from controller 

## Public Attributes Documentation

### variable entity_id

```cpp
eid entity_id;
```


### variable keyboard_focus

```cpp
bool keyboard_focus = true;
```


### variable mouse_focus

```cpp
bool mouse_focus = true;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC