---
title: tec::DebugInfo

---

# tec::DebugInfo



Inherits from [tec::AbstractWindow](/engine/Classes/classtec_1_1_abstract_window/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[DebugInfo](/engine/Classes/classtec_1_1_debug_info/#function-debuginfo)**([Game](/engine/Classes/classtec_1_1_game/) & game) |
| virtual void | **[Update](/engine/Classes/classtec_1_1_debug_info/#function-update)**(double ) override |
| virtual void | **[Draw](/engine/Classes/classtec_1_1_debug_info/#function-draw)**() override |

## Additional inherited members

**Public Functions inherited from [tec::AbstractWindow](/engine/Classes/classtec_1_1_abstract_window/)**

|                | Name           |
| -------------- | -------------- |
| virtual | **[~AbstractWindow](/engine/Classes/classtec_1_1_abstract_window/#function-~abstractwindow)**() =default |
| void | **[SetAlpha](/engine/Classes/classtec_1_1_abstract_window/#function-setalpha)**(float a) |
| float | **[GetAlpha](/engine/Classes/classtec_1_1_abstract_window/#function-getalpha)**() const |
| virtual bool | **[isCollapsed](/engine/Classes/classtec_1_1_abstract_window/#function-iscollapsed)**() const |
| bool | **[isVisible](/engine/Classes/classtec_1_1_abstract_window/#function-isvisible)**() const |
| void | **[Hide](/engine/Classes/classtec_1_1_abstract_window/#function-hide)**() |
| void | **[Show](/engine/Classes/classtec_1_1_abstract_window/#function-show)**() |

**Protected Attributes inherited from [tec::AbstractWindow](/engine/Classes/classtec_1_1_abstract_window/)**

|                | Name           |
| -------------- | -------------- |
| float | **[alpha](/engine/Classes/classtec_1_1_abstract_window/#variable-alpha)**  |
| bool | **[collapsed](/engine/Classes/classtec_1_1_abstract_window/#variable-collapsed)**  |
| bool | **[show](/engine/Classes/classtec_1_1_abstract_window/#variable-show)** <br>Collapsed ?  |


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


**Reimplements**: [tec::AbstractWindow::Update](/engine/Classes/classtec_1_1_abstract_window/#function-update)


### function Draw

```cpp
virtual void Draw() override
```


**Reimplements**: [tec::AbstractWindow::Draw](/engine/Classes/classtec_1_1_abstract_window/#function-draw)


-------------------------------

Updated on 21 March 2021 at 16:51:04 UTC