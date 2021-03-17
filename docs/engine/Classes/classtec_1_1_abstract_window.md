---
title: tec::AbstractWindow

---

# tec::AbstractWindow



Inherited by [tec::ActiveEntityTooltip](Classes/classtec_1_1_active_entity_tooltip.md), [tec::Console](Classes/classtec_1_1_console.md), [tec::DebugInfo](Classes/classtec_1_1_debug_info.md), [tec::PingTimesWindow](Classes/classtec_1_1_ping_times_window.md), [tec::ServerConnectWindow](Classes/classtec_1_1_server_connect_window.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| virtual | **[~AbstractWindow](Classes/classtec_1_1_abstract_window.md#function-~abstractwindow)**() =default |
| virtual void | **[Update](Classes/classtec_1_1_abstract_window.md#function-update)**(double ) |
| virtual void | **[Draw](Classes/classtec_1_1_abstract_window.md#function-draw)**() =0 |
| void | **[SetAlpha](Classes/classtec_1_1_abstract_window.md#function-setalpha)**(float a) |
| float | **[GetAlpha](Classes/classtec_1_1_abstract_window.md#function-getalpha)**() const |
| virtual bool | **[isCollapsed](Classes/classtec_1_1_abstract_window.md#function-iscollapsed)**() const |
| bool | **[isVisible](Classes/classtec_1_1_abstract_window.md#function-isvisible)**() const |
| void | **[Hide](Classes/classtec_1_1_abstract_window.md#function-hide)**() |
| void | **[Show](Classes/classtec_1_1_abstract_window.md#function-show)**() |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| float | **[alpha](Classes/classtec_1_1_abstract_window.md#variable-alpha)**  |
| bool | **[collapsed](Classes/classtec_1_1_abstract_window.md#variable-collapsed)**  |
| bool | **[show](Classes/classtec_1_1_abstract_window.md#variable-show)** <br>Collapsed ?  |

## Public Functions Documentation

### function ~AbstractWindow

```cpp
virtual ~AbstractWindow() =default
```


### function Update

```cpp
inline virtual void Update(
    double 
)
```


**Reimplemented by**: [tec::Console::Update](Classes/classtec_1_1_console.md#function-update), [tec::ActiveEntityTooltip::Update](Classes/classtec_1_1_active_entity_tooltip.md#function-update), [tec::DebugInfo::Update](Classes/classtec_1_1_debug_info.md#function-update), [tec::ServerConnectWindow::Update](Classes/classtec_1_1_server_connect_window.md#function-update), [tec::PingTimesWindow::Update](Classes/classtec_1_1_ping_times_window.md#function-update)


### function Draw

```cpp
virtual void Draw() =0
```


**Reimplemented by**: [tec::ActiveEntityTooltip::Draw](Classes/classtec_1_1_active_entity_tooltip.md#function-draw), [tec::Console::Draw](Classes/classtec_1_1_console.md#function-draw), [tec::DebugInfo::Draw](Classes/classtec_1_1_debug_info.md#function-draw), [tec::ServerConnectWindow::Draw](Classes/classtec_1_1_server_connect_window.md#function-draw), [tec::PingTimesWindow::Draw](Classes/classtec_1_1_ping_times_window.md#function-draw)


### function SetAlpha

```cpp
inline void SetAlpha(
    float a
)
```


### function GetAlpha

```cpp
inline float GetAlpha() const
```


### function isCollapsed

```cpp
inline virtual bool isCollapsed() const
```


**Reimplemented by**: [tec::Console::isCollapsed](Classes/classtec_1_1_console.md#function-iscollapsed)


### function isVisible

```cpp
inline bool isVisible() const
```


### function Hide

```cpp
inline void Hide()
```


### function Show

```cpp
inline void Show()
```


## Protected Attributes Documentation

### variable alpha

```cpp
float alpha = 1.0f;
```


### variable collapsed

```cpp
bool collapsed = true;
```


### variable show

```cpp
bool show = true;
```

Collapsed ? 

-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC