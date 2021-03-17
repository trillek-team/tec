---
title: tec::DebugInfo

---

# tec::DebugInfo



Inherits from [tec::AbstractWindow](Classes/classtec_1_1_abstract_window.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[DebugInfo](Classes/classtec_1_1_debug_info.md#function-debuginfo)**([Game](Classes/classtec_1_1_game.md) & game) |
| virtual void | **[Update](Classes/classtec_1_1_debug_info.md#function-update)**(double ) override |
| virtual void | **[Draw](Classes/classtec_1_1_debug_info.md#function-draw)**() override |

## Additional inherited members

**Public Functions inherited from [tec::AbstractWindow](Classes/classtec_1_1_abstract_window.md)**

|                | Name           |
| -------------- | -------------- |
| virtual | **[~AbstractWindow](Classes/classtec_1_1_abstract_window.md#function-~abstractwindow)**() =default |
| void | **[SetAlpha](Classes/classtec_1_1_abstract_window.md#function-setalpha)**(float a) |
| float | **[GetAlpha](Classes/classtec_1_1_abstract_window.md#function-getalpha)**() const |
| virtual bool | **[isCollapsed](Classes/classtec_1_1_abstract_window.md#function-iscollapsed)**() const |
| bool | **[isVisible](Classes/classtec_1_1_abstract_window.md#function-isvisible)**() const |
| void | **[Hide](Classes/classtec_1_1_abstract_window.md#function-hide)**() |
| void | **[Show](Classes/classtec_1_1_abstract_window.md#function-show)**() |

**Protected Attributes inherited from [tec::AbstractWindow](Classes/classtec_1_1_abstract_window.md)**

|                | Name           |
| -------------- | -------------- |
| float | **[alpha](Classes/classtec_1_1_abstract_window.md#variable-alpha)**  |
| bool | **[collapsed](Classes/classtec_1_1_abstract_window.md#variable-collapsed)**  |
| bool | **[show](Classes/classtec_1_1_abstract_window.md#variable-show)** <br>Collapsed ?  |


## Public Functions Documentation

### function DebugInfo

```cpp
DebugInfo(
    Game & game
)
```


### function Update

```cpp
virtual void Update(
    double 
) override
```


**Reimplements**: [tec::AbstractWindow::Update](Classes/classtec_1_1_abstract_window.md#function-update)


### function Draw

```cpp
virtual void Draw() override
```


**Reimplements**: [tec::AbstractWindow::Draw](Classes/classtec_1_1_abstract_window.md#function-draw)


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC