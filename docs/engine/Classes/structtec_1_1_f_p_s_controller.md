---
title: tec::FPSController

---

# tec::FPSController



Inherits from [tec::Controller](Classes/structtec_1_1_controller.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[FPSController](Classes/structtec_1_1_f_p_s_controller.md#function-fpscontroller)**(const [eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~FPSController](Classes/structtec_1_1_f_p_s_controller.md#function-~fpscontroller)**() =default |
| void | **[Handle](Classes/structtec_1_1_f_p_s_controller.md#function-handle)**(const [KeyboardEvent](Classes/structtec_1_1_keyboard_event.md) & data, const [GameState](Classes/structtec_1_1_game_state.md) & state) |
| void | **[Handle](Classes/structtec_1_1_f_p_s_controller.md#function-handle)**(const [MouseBtnEvent](Classes/structtec_1_1_mouse_btn_event.md) & data, const [GameState](Classes/structtec_1_1_game_state.md) & state) |
| void | **[Handle](Classes/structtec_1_1_f_p_s_controller.md#function-handle)**(const [MouseMoveEvent](Classes/structtec_1_1_mouse_move_event.md) & data, const [GameState](Classes/structtec_1_1_game_state.md) & state) |
| virtual void | **[Update](Classes/structtec_1_1_f_p_s_controller.md#function-update)**(double delta, [GameState](Classes/structtec_1_1_game_state.md) & state, [EventList](Classes/structtec_1_1_event_list.md) & commands) override |
| virtual proto::ClientCommands | **[GetClientCommands](Classes/structtec_1_1_f_p_s_controller.md#function-getclientcommands)**() override |
| virtual void | **[ApplyClientCommands](Classes/structtec_1_1_f_p_s_controller.md#function-applyclientcommands)**(proto::ClientCommands proto_client_commands) override |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[forward](Classes/structtec_1_1_f_p_s_controller.md#variable-forward)**  |
| bool | **[backward](Classes/structtec_1_1_f_p_s_controller.md#variable-backward)**  |
| bool | **[right_strafe](Classes/structtec_1_1_f_p_s_controller.md#variable-right_strafe)**  |
| bool | **[left_strafe](Classes/structtec_1_1_f_p_s_controller.md#variable-left_strafe)**  |
| double | **[current_delta](Classes/structtec_1_1_f_p_s_controller.md#variable-current_delta)**  |
| bool | **[mouse_look](Classes/structtec_1_1_f_p_s_controller.md#variable-mouse_look)**  |
| std::unique_ptr< [Orientation](Classes/structtec_1_1_orientation.md) > | **[orientation](Classes/structtec_1_1_f_p_s_controller.md#variable-orientation)**  |
| bool | **[KEY_A_FIRST](Classes/structtec_1_1_f_p_s_controller.md#variable-key_a_first)**  |
| bool | **[KEY_W_FIRST](Classes/structtec_1_1_f_p_s_controller.md#variable-key_w_first)**  |
| bool | **[KEY_W_DOWN](Classes/structtec_1_1_f_p_s_controller.md#variable-key_w_down)**  |
| bool | **[KEY_A_DOWN](Classes/structtec_1_1_f_p_s_controller.md#variable-key_a_down)**  |
| bool | **[KEY_S_DOWN](Classes/structtec_1_1_f_p_s_controller.md#variable-key_s_down)**  |
| bool | **[KEY_D_DOWN](Classes/structtec_1_1_f_p_s_controller.md#variable-key_d_down)**  |

## Additional inherited members

**Public Functions inherited from [tec::Controller](Classes/structtec_1_1_controller.md)**

|                | Name           |
| -------------- | -------------- |
| | **[Controller](Classes/structtec_1_1_controller.md#function-controller)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~Controller](Classes/structtec_1_1_controller.md#function-~controller)**() =default |
| virtual void | **[SetFocus](Classes/structtec_1_1_controller.md#function-setfocus)**(bool keyboard, bool mouse)<br>called to indicate focus has been restored to controller  |
| virtual void | **[ClearFocus](Classes/structtec_1_1_controller.md#function-clearfocus)**(bool keyboard, bool mouse)<br>called to indicate focus has been captured from controller  |

**Public Attributes inherited from [tec::Controller](Classes/structtec_1_1_controller.md)**

|                | Name           |
| -------------- | -------------- |
| [eid](Namespaces/namespacetec.md#typedef-eid) | **[entity_id](Classes/structtec_1_1_controller.md#variable-entity_id)**  |
| bool | **[keyboard_focus](Classes/structtec_1_1_controller.md#variable-keyboard_focus)**  |
| bool | **[mouse_focus](Classes/structtec_1_1_controller.md#variable-mouse_focus)**  |


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


**Reimplements**: [tec::Controller::Update](Classes/structtec_1_1_controller.md#function-update)


### function GetClientCommands

```cpp
virtual proto::ClientCommands GetClientCommands() override
```


**Reimplements**: [tec::Controller::GetClientCommands](Classes/structtec_1_1_controller.md#function-getclientcommands)


### function ApplyClientCommands

```cpp
virtual void ApplyClientCommands(
    proto::ClientCommands proto_client_commands
) override
```


**Reimplements**: [tec::Controller::ApplyClientCommands](Classes/structtec_1_1_controller.md#function-applyclientcommands)


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

Updated on 17 March 2021 at 23:08:11 UTC