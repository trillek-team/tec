---
title: tec::IMGUISystem

---

# tec::IMGUISystem



Inherits from [tec::CommandQueue< IMGUISystem >](Classes/classtec_1_1_command_queue.md), [tec::EventQueue< KeyboardEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< MouseMoveEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< MouseScrollEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< MouseBtnEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< WindowResizedEvent >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[IMGUISystem](Classes/classtec_1_1_i_m_g_u_i_system.md#function-imguisystem)**(GLFWwindow * window) |
| | **[~IMGUISystem](Classes/classtec_1_1_i_m_g_u_i_system.md#function-~imguisystem)**() |
| void | **[CreateGUI](Classes/classtec_1_1_i_m_g_u_i_system.md#function-creategui)**() |
| void | **[Update](Classes/classtec_1_1_i_m_g_u_i_system.md#function-update)**(double delta) |
| void | **[CreateDeviceObjects](Classes/classtec_1_1_i_m_g_u_i_system.md#function-createdeviceobjects)**() |
| void | **[AddWindowDrawFunction](Classes/classtec_1_1_i_m_g_u_i_system.md#function-addwindowdrawfunction)**(std::string name, std::function< void()> && func) |
| void | **[ShowWindow](Classes/classtec_1_1_i_m_g_u_i_system.md#function-showwindow)**(const std::string name) |
| void | **[HideWindow](Classes/classtec_1_1_i_m_g_u_i_system.md#function-hidewindow)**(const std::string name) |
| bool | **[IsWindowVisible](Classes/classtec_1_1_i_m_g_u_i_system.md#function-iswindowvisible)**(const std::string & name) const |
| const char * | **[GetClipboardText](Classes/classtec_1_1_i_m_g_u_i_system.md#function-getclipboardtext)**(void * user_data) |
| void | **[SetClipboardText](Classes/classtec_1_1_i_m_g_u_i_system.md#function-setclipboardtext)**(void * user_data, const char * text) |
| void | **[RenderDrawLists](Classes/classtec_1_1_i_m_g_u_i_system.md#function-renderdrawlists)**(ImDrawData * draw_data) |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< IMGUISystem >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](Classes/classtec_1_1_command_queue.md#function-commandqueue)**() |
| | **[~CommandQueue](Classes/classtec_1_1_command_queue.md#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](Classes/classtec_1_1_command_queue.md#function-processcommandqueue)**() |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**([Command](Classes/structtec_1_1_command.md)< T > && command) |
| void | **[QueueCommand](Classes/classtec_1_1_command_queue.md#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< IMGUISystem >](Classes/classtec_1_1_command_queue.md)**

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * > | **[global_command_queue](Classes/classtec_1_1_command_queue.md#variable-global_command_queue)**  |
| std::queue< [Command](Classes/structtec_1_1_command.md)< T > > * | **[local_queue](Classes/classtec_1_1_command_queue.md#variable-local_queue)**  |

**Public Functions inherited from [tec::EventQueue< KeyboardEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< KeyboardEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseMoveEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseMoveEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseScrollEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseScrollEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseBtnEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseBtnEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |

**Public Functions inherited from [tec::EventQueue< WindowResizedEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**() |
| | **[EventQueue](Classes/classtec_1_1_event_queue.md#function-eventqueue)**([eid](Namespaces/namespacetec.md#typedef-eid) entity_id) |
| virtual | **[~EventQueue](Classes/classtec_1_1_event_queue.md#function-~eventqueue)**() |
| void | **[ProcessEventQueue](Classes/classtec_1_1_event_queue.md#function-processeventqueue)**() |
| void | **[QueueEvent](Classes/classtec_1_1_event_queue.md#function-queueevent)**([Event](Classes/structtec_1_1_event.md)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< WindowResizedEvent >](Classes/classtec_1_1_event_queue.md)**

|                | Name           |
| -------------- | -------------- |
| [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * | **[read_event_queue](Classes/classtec_1_1_event_queue.md#variable-read_event_queue)**  |
| std::atomic< [Queue](Classes/structtec_1_1_queue.md)< [Event](Classes/structtec_1_1_event.md)< T > > * > | **[write_event_queue](Classes/classtec_1_1_event_queue.md#variable-write_event_queue)**  |


## Public Functions Documentation

### function IMGUISystem

```cpp
IMGUISystem(
    GLFWwindow * window
)
```


### function ~IMGUISystem

```cpp
~IMGUISystem()
```


### function CreateGUI

```cpp
void CreateGUI()
```


### function Update

```cpp
void Update(
    double delta
)
```


### function CreateDeviceObjects

```cpp
void CreateDeviceObjects()
```


### function AddWindowDrawFunction

```cpp
void AddWindowDrawFunction(
    std::string name,
    std::function< void()> && func
)
```


### function ShowWindow

```cpp
inline void ShowWindow(
    const std::string name
)
```


### function HideWindow

```cpp
inline void HideWindow(
    const std::string name
)
```


### function IsWindowVisible

```cpp
inline bool IsWindowVisible(
    const std::string & name
) const
```


### function GetClipboardText

```cpp
static const char * GetClipboardText(
    void * user_data
)
```


### function SetClipboardText

```cpp
static void SetClipboardText(
    void * user_data,
    const char * text
)
```


### function RenderDrawLists

```cpp
static void RenderDrawLists(
    ImDrawData * draw_data
)
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC