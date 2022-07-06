---
title: tec::FPSController

---

# tec::FPSController





Inherits from [tec::Controller](/engine/Classes/structtec_1_1_controller/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[FPSController](/engine/Classes/structtec_1_1_f_p_s_controller/#function-fpscontroller)**(const [eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~FPSController](/engine/Classes/structtec_1_1_f_p_s_controller/#function-~fpscontroller)**() =default |
| void | **[Handle](/engine/Classes/structtec_1_1_f_p_s_controller/#function-handle)**(const [KeyboardEvent](/engine/Classes/structtec_1_1_keyboard_event/) & data, const [GameState](/engine/Classes/structtec_1_1_game_state/) & state) |
| void | **[Handle](/engine/Classes/structtec_1_1_f_p_s_controller/#function-handle)**(const [MouseBtnEvent](/engine/Classes/structtec_1_1_mouse_btn_event/) & data, const [GameState](/engine/Classes/structtec_1_1_game_state/) & state) |
| void | **[Handle](/engine/Classes/structtec_1_1_f_p_s_controller/#function-handle)**(const [MouseMoveEvent](/engine/Classes/structtec_1_1_mouse_move_event/) & data, const [GameState](/engine/Classes/structtec_1_1_game_state/) & state) |
| virtual void | **[Update](/engine/Classes/structtec_1_1_f_p_s_controller/#function-update)**(double delta, [GameState](/engine/Classes/structtec_1_1_game_state/) & state, [EventList](/engine/Classes/structtec_1_1_event_list/) & commands) override |
| virtual proto::ClientCommands | **[GetClientCommands](/engine/Classes/structtec_1_1_f_p_s_controller/#function-getclientcommands)**() override |
| virtual void | **[ApplyClientCommands](/engine/Classes/structtec_1_1_f_p_s_controller/#function-applyclientcommands)**(proto::ClientCommands proto_client_commands) override |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[forward](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-forward)**  |
| bool | **[backward](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-backward)**  |
| bool | **[right_strafe](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-right-strafe)**  |
| bool | **[left_strafe](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-left-strafe)**  |
| double | **[current_delta](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-current-delta)**  |
| bool | **[mouse_look](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-mouse-look)**  |
| std::unique_ptr< [Orientation](/engine/Classes/structtec_1_1_orientation/) > | **[orientation](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-orientation)**  |
| bool | **[KEY_A_FIRST](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-key-a-first)**  |
| bool | **[KEY_W_FIRST](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-key-w-first)**  |
| bool | **[KEY_W_DOWN](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-key-w-down)**  |
| bool | **[KEY_A_DOWN](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-key-a-down)**  |
| bool | **[KEY_S_DOWN](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-key-s-down)**  |
| bool | **[KEY_D_DOWN](/engine/Classes/structtec_1_1_f_p_s_controller/#variable-key-d-down)**  |

## Additional inherited members

**Public Functions inherited from [tec::Controller](/engine/Classes/structtec_1_1_controller/)**

|                | Name           |
| -------------- | -------------- |
| | **[Controller](/engine/Classes/structtec_1_1_controller/#function-controller)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~Controller](/engine/Classes/structtec_1_1_controller/#function-~controller)**() =default |
| virtual void | **[SetFocus](/engine/Classes/structtec_1_1_controller/#function-setfocus)**(bool keyboard, bool mouse)<br>called to indicate focus has been restored to controller  |
| virtual void | **[ClearFocus](/engine/Classes/structtec_1_1_controller/#function-clearfocus)**(bool keyboard, bool mouse)<br>called to indicate focus has been captured from controller  |

**Public Attributes inherited from [tec::Controller](/engine/Classes/structtec_1_1_controller/)**

|                | Name           |
| -------------- | -------------- |
| [eid](/engine/Namespaces/namespacetec/#typedef-eid) | **[entity_id](/engine/Classes/structtec_1_1_controller/#variable-entity-id)**  |
| bool | **[keyboard_focus](/engine/Classes/structtec_1_1_controller/#variable-keyboard-focus)**  |
| bool | **[mouse_focus](/engine/Classes/structtec_1_1_controller/#variable-mouse-focus)**  |


## Public Functions Documentation

### function FPSController

```cpp
inline FPSController(
    const eid entity_id
)
```


### function ~FPSController

```cpp
virtual ~FPSController() =default
```


### function Handle

```cpp
void Handle(
    const KeyboardEvent & data,
    const GameState & state
)
```


### function Handle

```cpp
void Handle(
    const MouseBtnEvent & data,
    const GameState & state
)
```


### function Handle

```cpp
void Handle(
    const MouseMoveEvent & data,
    const GameState & state
)
```


### function Update

```cpp
virtual void Update(
    double delta,
    GameState & state,
    EventList & commands
) override
```


**Reimplements**: [tec::Controller::Update](/engine/Classes/structtec_1_1_controller/#function-update)


### function GetClientCommands

```cpp
virtual proto::ClientCommands GetClientCommands() override
```


**Reimplements**: [tec::Controller::GetClientCommands](/engine/Classes/structtec_1_1_controller/#function-getclientcommands)


### function ApplyClientCommands

```cpp
virtual void ApplyClientCommands(
    proto::ClientCommands proto_client_commands
) override
```


**Reimplements**: [tec::Controller::ApplyClientCommands](/engine/Classes/structtec_1_1_controller/#function-applyclientcommands)


## Public Attributes Documentation

### variable forward

```cpp
bool forward {false};
```


### variable backward

```cpp
bool backward {false};
```


### variable right_strafe

```cpp
bool right_strafe {false};
```


### variable left_strafe

```cpp
bool left_strafe {false};
```


### variable current_delta

```cpp
double current_delta {0.0};
```


### variable mouse_look

```cpp
bool mouse_look {false};
```


### variable orientation

```cpp
std::unique_ptr< Orientation > orientation;
```


### variable KEY_A_FIRST

```cpp
bool KEY_A_FIRST {false};
```


### variable KEY_W_FIRST

```cpp
bool KEY_W_FIRST {false};
```


### variable KEY_W_DOWN

```cpp
bool KEY_W_DOWN {false};
```


### variable KEY_A_DOWN

```cpp
bool KEY_A_DOWN {false};
```


### variable KEY_S_DOWN

```cpp
bool KEY_S_DOWN {false};
```


### variable KEY_D_DOWN

```cpp
bool KEY_D_DOWN {false};
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000