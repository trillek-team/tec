---
title: tec::IMGUISystem

---

# tec::IMGUISystem





Inherits from [tec::CommandQueue< IMGUISystem >](/engine/Classes/classtec_1_1_command_queue/), [tec::EventQueue< KeyboardEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< MouseMoveEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< MouseScrollEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< MouseBtnEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< WindowResizedEvent >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[IMGUISystem](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-imguisystem)**(GLFWwindow * window) |
| | **[~IMGUISystem](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-~imguisystem)**() |
| void | **[CreateGUI](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-creategui)**() |
| void | **[Update](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-update)**(double delta) |
| void | **[CreateDeviceObjects](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-createdeviceobjects)**() |
| void | **[AddWindowDrawFunction](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-addwindowdrawfunction)**(std::string name, std::function< void()> && func) |
| void | **[ShowWindow](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-showwindow)**(const std::string name) |
| void | **[HideWindow](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-hidewindow)**(const std::string name) |
| bool | **[IsWindowVisible](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-iswindowvisible)**(const std::string & name) const |
| const char * | **[GetClipboardText](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-getclipboardtext)**(void * user_data) |
| void | **[SetClipboardText](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-setclipboardtext)**(void * user_data, const char * text) |
| void | **[RenderDrawLists](/engine/Classes/classtec_1_1_i_m_g_u_i_system/#function-renderdrawlists)**(ImDrawData * draw_data) |

## Additional inherited members

**Public Functions inherited from [tec::CommandQueue< IMGUISystem >](/engine/Classes/classtec_1_1_command_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-commandqueue)**() |
| | **[~CommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-~commandqueue)**() |
| void | **[ProcessCommandQueue](/engine/Classes/classtec_1_1_command_queue/#function-processcommandqueue)**() |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**([Command](/engine/Classes/structtec_1_1_command/)< T > && command) |
| void | **[QueueCommand](/engine/Classes/classtec_1_1_command_queue/#function-queuecommand)**(std::function< void(T *)> && command) |

**Protected Attributes inherited from [tec::CommandQueue< IMGUISystem >](/engine/Classes/classtec_1_1_command_queue/)**

|                | Name           |
| -------------- | -------------- |
| std::atomic< std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * > | **[global_command_queue](/engine/Classes/classtec_1_1_command_queue/#variable-global-command-queue)**  |
| std::queue< [Command](/engine/Classes/structtec_1_1_command/)< T > > * | **[local_queue](/engine/Classes/classtec_1_1_command_queue/#variable-local-queue)**  |

**Public Functions inherited from [tec::EventQueue< KeyboardEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< KeyboardEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseMoveEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseMoveEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseScrollEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseScrollEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< MouseBtnEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< MouseBtnEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |

**Public Functions inherited from [tec::EventQueue< WindowResizedEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**() |
| | **[EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-eventqueue)**([eid](/engine/Namespaces/namespacetec/#typedef-eid) entity_id) |
| virtual | **[~EventQueue](/engine/Classes/classtec_1_1_event_queue/#function-~eventqueue)**() |
| void | **[ProcessEventQueue](/engine/Classes/classtec_1_1_event_queue/#function-processeventqueue)**() |
| void | **[QueueEvent](/engine/Classes/classtec_1_1_event_queue/#function-queueevent)**([Event](/engine/Classes/structtec_1_1_event/)< T > && e) |

**Protected Attributes inherited from [tec::EventQueue< WindowResizedEvent >](/engine/Classes/classtec_1_1_event_queue/)**

|                | Name           |
| -------------- | -------------- |
| [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * | **[read_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-read-event-queue)**  |
| std::atomic< [Queue](/engine/Classes/structtec_1_1_queue/)< [Event](/engine/Classes/structtec_1_1_event/)< T > > * > | **[write_event_queue](/engine/Classes/classtec_1_1_event_queue/#variable-write-event-queue)**  |


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

Updated on 2022-07-06 at 04:15:07 +0000