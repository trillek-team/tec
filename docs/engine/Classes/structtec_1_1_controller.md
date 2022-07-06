---
title: tec::Controller

---

# tec::Controller





Inherited by [tec::FPSController](/engine/Classes/structtec_1_1_f_p_s_controller/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Controller](/engine/Classes/structtec_1_1_controller/#function-controller)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual void | **[Update](/engine/Classes/structtec_1_1_controller/#function-update)**(double , [GameState](/engine/Classes/structtec_1_1_game_state/) & , [EventList](/engine/Classes/structtec_1_1_event_list/) & ) |
| virtual | **[~Controller](/engine/Classes/structtec_1_1_controller/#function-~controller)**() =default |
| virtual proto::ClientCommands | **[GetClientCommands](/engine/Classes/structtec_1_1_controller/#function-getclientcommands)**() =0 |
| virtual void | **[ApplyClientCommands](/engine/Classes/structtec_1_1_controller/#function-applyclientcommands)**(proto::ClientCommands ) =0 |
| virtual void | **[SetFocus](/engine/Classes/structtec_1_1_controller/#function-setfocus)**(bool keyboard, bool mouse)<br>called to indicate focus has been restored to controller  |
| virtual void | **[ClearFocus](/engine/Classes/structtec_1_1_controller/#function-clearfocus)**(bool keyboard, bool mouse)<br>called to indicate focus has been captured from controller  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| [eid](/engine/Namespaces/namespacetec/#typedef-eid) | **[entity_id](/engine/Classes/structtec_1_1_controller/#variable-entity-id)**  |
| bool | **[keyboard_focus](/engine/Classes/structtec_1_1_controller/#variable-keyboard-focus)**  |
| bool | **[mouse_focus](/engine/Classes/structtec_1_1_controller/#variable-mouse-focus)**  |

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


**Reimplemented by**: [tec::FPSController::Update](/engine/Classes/structtec_1_1_f_p_s_controller/#function-update)


### function ~Controller

```cpp
virtual ~Controller() =default
```


### function GetClientCommands

```cpp
virtual proto::ClientCommands GetClientCommands() =0
```


**Reimplemented by**: [tec::FPSController::GetClientCommands](/engine/Classes/structtec_1_1_f_p_s_controller/#function-getclientcommands)


### function ApplyClientCommands

```cpp
virtual void ApplyClientCommands(
    proto::ClientCommands 
) =0
```


**Reimplemented by**: [tec::FPSController::ApplyClientCommands](/engine/Classes/structtec_1_1_f_p_s_controller/#function-applyclientcommands)


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

Updated on 2022-07-06 at 04:15:07 +0000