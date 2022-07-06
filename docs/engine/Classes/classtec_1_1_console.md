---
title: tec::Console

---

# tec::Console





Inherits from [tec::AbstractWindow](/engine/Classes/classtec_1_1_abstract_window/), [tec::EventQueue< KeyboardEvent >](/engine/Classes/classtec_1_1_event_queue/), [tec::EventQueue< WindowResizedEvent >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Console](/engine/Classes/classtec_1_1_console/#function-console)**() |
| virtual void | **[Update](/engine/Classes/classtec_1_1_console/#function-update)**(double delta) override |
| void | **[Clear](/engine/Classes/classtec_1_1_console/#function-clear)**() |
| void | **[Println](/engine/Classes/classtec_1_1_console/#function-println)**(const std::string & str, ImVec4 color =ImVec4(255, 255, 255, 255)) |
| void | **[Println](/engine/Classes/classtec_1_1_console/#function-println)**(const char * cstr, ImVec4 color =ImVec4(255, 255, 255, 255)) |
| void | **[Printfln](/engine/Classes/classtec_1_1_console/#function-printfln)**(const char * cstr, ... ) |
| virtual void | **[Draw](/engine/Classes/classtec_1_1_console/#function-draw)**([IMGUISystem](/engine/Classes/classtec_1_1_i_m_g_u_i_system/) * ) override |
| virtual bool | **[isCollapsed](/engine/Classes/classtec_1_1_console/#function-iscollapsed)**() const override |
| void | **[AddConsoleCommand](/engine/Classes/classtec_1_1_console/#function-addconsolecommand)**(std::string name, std::string help, std::function< void(const std::string &)> && func) |
| void | **[AddSlashHandler](/engine/Classes/classtec_1_1_console/#function-addslashhandler)**(std::function< void(const std::string &)> && func) |

## Additional inherited members

**Public Functions inherited from [tec::AbstractWindow](/engine/Classes/classtec_1_1_abstract_window/)**

|                | Name           |
| -------------- | -------------- |
| virtual | **[~AbstractWindow](/engine/Classes/classtec_1_1_abstract_window/#function-~abstractwindow)**() =default |
| void | **[SetAlpha](/engine/Classes/classtec_1_1_abstract_window/#function-setalpha)**(float a) |
| float | **[GetAlpha](/engine/Classes/classtec_1_1_abstract_window/#function-getalpha)**() const |
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

### function Console

```cpp
Console()
```


### function Update

```cpp
virtual void Update(
    double delta
) override
```


**Reimplements**: [tec::AbstractWindow::Update](/engine/Classes/classtec_1_1_abstract_window/#function-update)


### function Clear

```cpp
void Clear()
```


Clears console output 


### function Println

```cpp
void Println(
    const std::string & str,
    ImVec4 color =ImVec4(255, 255, 255, 255)
)
```


### function Println

```cpp
void Println(
    const char * cstr,
    ImVec4 color =ImVec4(255, 255, 255, 255)
)
```


### function Printfln

```cpp
void Printfln(
    const char * cstr,
    ... 
)
```


### function Draw

```cpp
virtual void Draw(
    IMGUISystem * 
) override
```


**Reimplements**: [tec::AbstractWindow::Draw](/engine/Classes/classtec_1_1_abstract_window/#function-draw)


### function isCollapsed

```cpp
inline virtual bool isCollapsed() const override
```


**Reimplements**: [tec::AbstractWindow::isCollapsed](/engine/Classes/classtec_1_1_abstract_window/#function-iscollapsed)


### function AddConsoleCommand

```cpp
void AddConsoleCommand(
    std::string name,
    std::string help,
    std::function< void(const std::string &)> && func
)
```


**Parameters**: 

  * **name** Name of the command (case sensitive!) 
  * **help** String with help text 
  * **func** Function to be executed when the command is called 


Register a new command 


### function AddSlashHandler

```cpp
void AddSlashHandler(
    std::function< void(const std::string &)> && func
)
```


**Parameters**: 

  * **func** Function to be executed when the command is called 


Register a slash handler that handles all '/' commands 


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000