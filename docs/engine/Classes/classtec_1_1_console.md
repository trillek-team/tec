---
title: tec::Console

---

# tec::Console



Inherits from [tec::AbstractWindow](Classes/classtec_1_1_abstract_window.md), [tec::EventQueue< KeyboardEvent >](Classes/classtec_1_1_event_queue.md), [tec::EventQueue< WindowResizedEvent >](Classes/classtec_1_1_event_queue.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Console](Classes/classtec_1_1_console.md#function-console)**() |
| virtual void | **[Update](Classes/classtec_1_1_console.md#function-update)**(double delta) override |
| void | **[Clear](Classes/classtec_1_1_console.md#function-clear)**() |
| void | **[Println](Classes/classtec_1_1_console.md#function-println)**(const std::string & str, ImVec4 color =ImVec4(255, 255, 255, 255)) |
| void | **[Println](Classes/classtec_1_1_console.md#function-println)**(const char * cstr, ImVec4 color =ImVec4(255, 255, 255, 255)) |
| void | **[Printfln](Classes/classtec_1_1_console.md#function-printfln)**(const char * cstr, ... ) |
| virtual void | **[Draw](Classes/classtec_1_1_console.md#function-draw)**() override |
| virtual bool | **[isCollapsed](Classes/classtec_1_1_console.md#function-iscollapsed)**() const override |
| void | **[AddConsoleCommand](Classes/classtec_1_1_console.md#function-addconsolecommand)**(std::string name, std::string help, std::function< void(const char *)> && func) |
| void | **[AddSlashHandler](Classes/classtec_1_1_console.md#function-addslashhandler)**(std::function< void(const char *)> && func) |

## Additional inherited members

**Public Functions inherited from [tec::AbstractWindow](Classes/classtec_1_1_abstract_window.md)**

|                | Name           |
| -------------- | -------------- |
| virtual | **[~AbstractWindow](Classes/classtec_1_1_abstract_window.md#function-~abstractwindow)**() =default |
| void | **[SetAlpha](Classes/classtec_1_1_abstract_window.md#function-setalpha)**(float a) |
| float | **[GetAlpha](Classes/classtec_1_1_abstract_window.md#function-getalpha)**() const |
| bool | **[isVisible](Classes/classtec_1_1_abstract_window.md#function-isvisible)**() const |
| void | **[Hide](Classes/classtec_1_1_abstract_window.md#function-hide)**() |
| void | **[Show](Classes/classtec_1_1_abstract_window.md#function-show)**() |

**Protected Attributes inherited from [tec::AbstractWindow](Classes/classtec_1_1_abstract_window.md)**

|                | Name           |
| -------------- | -------------- |
| float | **[alpha](Classes/classtec_1_1_abstract_window.md#variable-alpha)**  |
| bool | **[collapsed](Classes/classtec_1_1_abstract_window.md#variable-collapsed)**  |
| bool | **[show](Classes/classtec_1_1_abstract_window.md#variable-show)** <br>Collapsed ?  |

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


**Reimplements**: [tec::AbstractWindow::Update](Classes/classtec_1_1_abstract_window.md#function-update)


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
virtual void Draw() override
```


**Reimplements**: [tec::AbstractWindow::Draw](Classes/classtec_1_1_abstract_window.md#function-draw)


### function isCollapsed

```cpp
inline virtual bool isCollapsed() const override
```


**Reimplements**: [tec::AbstractWindow::isCollapsed](Classes/classtec_1_1_abstract_window.md#function-iscollapsed)


### function AddConsoleCommand

```cpp
void AddConsoleCommand(
    std::string name,
    std::string help,
    std::function< void(const char *)> && func
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
    std::function< void(const char *)> && func
)
```


**Parameters**: 

  * **func** Function to be executed when the command is called 


Register a slash handler that handles all '/' commands 


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC