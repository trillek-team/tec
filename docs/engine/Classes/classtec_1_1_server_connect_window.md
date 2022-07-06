---
title: tec::ServerConnectWindow

---

# tec::ServerConnectWindow





Inherits from [tec::AbstractWindow](/engine/Classes/classtec_1_1_abstract_window/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ServerConnectWindow](/engine/Classes/classtec_1_1_server_connect_window/#function-serverconnectwindow)**([ServerConnection](/engine/Classes/classtec_1_1networking_1_1_server_connection/) & server_connection) |
| virtual void | **[Update](/engine/Classes/classtec_1_1_server_connect_window/#function-update)**(double ) override |
| virtual void | **[Draw](/engine/Classes/classtec_1_1_server_connect_window/#function-draw)**([IMGUISystem](/engine/Classes/classtec_1_1_i_m_g_u_i_system/) * gui) override |
| void | **[SetUsername](/engine/Classes/classtec_1_1_server_connect_window/#function-setusername)**(const std::string & user) |

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
| std::string | **[GetWindowName](/engine/Classes/classtec_1_1_abstract_window/#function-getwindowname)**() |

**Protected Attributes inherited from [tec::AbstractWindow](/engine/Classes/classtec_1_1_abstract_window/)**

|                | Name           |
| -------------- | -------------- |
| float | **[alpha](/engine/Classes/classtec_1_1_abstract_window/#variable-alpha)**  |
| bool | **[collapsed](/engine/Classes/classtec_1_1_abstract_window/#variable-collapsed)**  |
| bool | **[show](/engine/Classes/classtec_1_1_abstract_window/#variable-show)** <br>Collapsed ?  |
| std::string | **[window_name](/engine/Classes/classtec_1_1_abstract_window/#variable-window-name)** <br>Visible ?  |


## Public Functions Documentation

### function ServerConnectWindow

```cpp
ServerConnectWindow(
    ServerConnection & server_connection
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
virtual void Draw(
    IMGUISystem * gui
) override
```


**Reimplements**: [tec::AbstractWindow::Draw](/engine/Classes/classtec_1_1_abstract_window/#function-draw)


### function SetUsername

```cpp
static void SetUsername(
    const std::string & user
)
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000