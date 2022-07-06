---
title: tec::OS

---

# tec::OS





Inherits from [tec::EventQueue< KeyboardEvent >](/engine/Classes/classtec_1_1_event_queue/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| bool | **[InitializeWindow](/engine/Classes/classtec_1_1_o_s/#function-initializewindow)**(const int width, const int height, const std::string title, const int glMajor =3, const int glMinor =3, bool fullscreen =false)<br>Initialize the rendering window and makes the window's context the current one.  |
| void | **[MakeCurrent](/engine/Classes/classtec_1_1_o_s/#function-makecurrent)**()<br>Make the context of the window current for the calling thread.  |
| void | **[SetWindowAspectRatio](/engine/Classes/classtec_1_1_o_s/#function-setwindowaspectratio)**(const unsigned int numerator =16, const unsigned int denominator =9)<br>Sets the desired window aspect ratio numerator:denominator e.g. 16:9, 4:3.  |
| void | **[ToggleFullScreen](/engine/Classes/classtec_1_1_o_s/#function-togglefullscreen)**()<br>Toggles between fullscreen and windowed mode based.  |
| void | **[DetachContext](/engine/Classes/classtec_1_1_o_s/#function-detachcontext)**()<br>Detach the context of the window from the calling thread.  |
| void | **[Quit](/engine/Classes/classtec_1_1_o_s/#function-quit)**()<br>Tells the [OS](/engine/Classes/classtec_1_1_o_s/) that the active window should close.  |
| bool | **[Closing](/engine/Classes/classtec_1_1_o_s/#function-closing)**()<br>Checks if the window is closing.  |
| void | **[SwapBuffers](/engine/Classes/classtec_1_1_o_s/#function-swapbuffers)**()<br>Swap the front and back buffers. Call after rendering.  |
| void | **[OSMessageLoop](/engine/Classes/classtec_1_1_o_s/#function-osmessageloop)**()<br>Processes events in the [OS](/engine/Classes/classtec_1_1_o_s/) message event loop.  |
| int | **[GetWindowWidth](/engine/Classes/classtec_1_1_o_s/#function-getwindowwidth)**() const<br>Gets the cached window width.  |
| int | **[GetWindowHeight](/engine/Classes/classtec_1_1_o_s/#function-getwindowheight)**() const<br>Gets the cached window height.  |
| double | **[GetDeltaTime](/engine/Classes/classtec_1_1_o_s/#function-getdeltatime)**()<br>Returns the time since the start of the program.  |
| double | **[GetTime](/engine/Classes/classtec_1_1_o_s/#function-gettime)**()<br>Returns the time since initialization from glfw.  |
| GLFWwindow * | **[GetWindow](/engine/Classes/classtec_1_1_o_s/#function-getwindow)**()<br>Returns the current active window.  |
| void | **[EnableMouseLock](/engine/Classes/classtec_1_1_o_s/#function-enablemouselock)**() |
| void | **[DisableMouseLock](/engine/Classes/classtec_1_1_o_s/#function-disablemouselock)**() |
| void | **[Terminate](/engine/Classes/classtec_1_1_o_s/#function-terminate)**()<br>Calls GLFWTerminate to close the window.  |
| void | **[WindowResized](/engine/Classes/classtec_1_1_o_s/#function-windowresized)**(GLFWwindow * window, int width, int height)<br>Callback for when the window is resized.  |
| void | **[KeyboardEventCallback](/engine/Classes/classtec_1_1_o_s/#function-keyboardeventcallback)**(GLFWwindow * window, int key, int scancode, int action, int mods)<br>Callback for keyboard events.  |
| void | **[CharacterEventCallback](/engine/Classes/classtec_1_1_o_s/#function-charactereventcallback)**(GLFWwindow * window, unsigned int uchar)<br>Callback for Unicode character event.  |
| void | **[MouseMoveEventCallback](/engine/Classes/classtec_1_1_o_s/#function-mousemoveeventcallback)**(GLFWwindow * window, double x, double y)<br>Callback for mouse move events.  |
| void | **[MouseScrollEventCallback](/engine/Classes/classtec_1_1_o_s/#function-mousescrolleventcallback)**(GLFWwindow * window, double x, double y)<br>Callback for mouse scroll events.  |
| void | **[MouseButtonEventCallback](/engine/Classes/classtec_1_1_o_s/#function-mousebuttoneventcallback)**(GLFWwindow * window, int button, int action, int mods)<br>Callback for mouse button events.  |
| void | **[WindowFocusChangeCallback](/engine/Classes/classtec_1_1_o_s/#function-windowfocuschangecallback)**(GLFWwindow * window, int focused)<br>Callback for window focus change events.  |
| void | **[FileDropCallback](/engine/Classes/classtec_1_1_o_s/#function-filedropcallback)**(GLFWwindow * window, int count, const char ** paths)<br>Callback for window focus change events.  |
| void | **[SetMousePosition](/engine/Classes/classtec_1_1_o_s/#function-setmouseposition)**(const double x, const double y)<br>Sets the mouse cursor position relative to the upper-left corner of the window.  |
| void | **[GetMousePosition](/engine/Classes/classtec_1_1_o_s/#function-getmouseposition)**(double * x, double * y)<br>Gets the mouse cursor position relative to the upper-left corner of the window.  |
| const GLFWwindow * | **[GetFocusedWindow](/engine/Classes/classtec_1_1_o_s/#function-getfocusedwindow)**() |
| void | **[RegisterLuaType](/engine/Classes/classtec_1_1_o_s/#function-registerluatype)**(sol::state & ) |

## Additional inherited members

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


## Public Functions Documentation

### function InitializeWindow

```cpp
bool InitializeWindow(
    const int width,
    const int height,
    const std::string title,
    const int glMajor =3,
    const int glMinor =3,
    bool fullscreen =false
)
```

Initialize the rendering window and makes the window's context the current one. 

**Parameters**: 

  * **const** unsigned int width, height The window's client width and height 
  * **const** std::string title The title to show in the title bar and task manager. 
  * **const** glMajor OpenGL major version number. Must be >= 3 
  * **const** glMinor OpenGL minor version. If major = 3, must be 3 (OpenGL 3.3) 
  * **bool** fullscreen Whether the window should be started in fullscreen mode. 


**Return**: bool If creation was successful or not. 

### function MakeCurrent

```cpp
void MakeCurrent()
```

Make the context of the window current for the calling thread. 

### function SetWindowAspectRatio

```cpp
void SetWindowAspectRatio(
    const unsigned int numerator =16,
    const unsigned int denominator =9
)
```

Sets the desired window aspect ratio numerator:denominator e.g. 16:9, 4:3. 

### function ToggleFullScreen

```cpp
void ToggleFullScreen()
```

Toggles between fullscreen and windowed mode based. 

### function DetachContext

```cpp
void DetachContext()
```

Detach the context of the window from the calling thread. 

### function Quit

```cpp
void Quit()
```

Tells the [OS](/engine/Classes/classtec_1_1_o_s/) that the active window should close. 

**Return**: void 

Since the main loop is based on that close status of that active window this effectively causes [Closing()](/engine/Classes/classtec_1_1_o_s/#function-closing) to return true during an upcoming message loop. 


### function Closing

```cpp
bool Closing()
```

Checks if the window is closing. 

**Return**: bool True if the window is closing. 

### function SwapBuffers

```cpp
void SwapBuffers()
```

Swap the front and back buffers. Call after rendering. 

**Return**: void 

### function OSMessageLoop

```cpp
void OSMessageLoop()
```

Processes events in the [OS](/engine/Classes/classtec_1_1_o_s/) message event loop. 

### function GetWindowWidth

```cpp
int GetWindowWidth() const
```

Gets the cached window width. 

**Return**: int The window width. 

### function GetWindowHeight

```cpp
int GetWindowHeight() const
```

Gets the cached window height. 

**Return**: int The window height. 

### function GetDeltaTime

```cpp
double GetDeltaTime()
```

Returns the time since the start of the program. 

**Return**: double The amount of time that have passed since the last call in seconds. 

### function GetTime

```cpp
double GetTime()
```

Returns the time since initialization from glfw. 

**Return**: double The amount of time since glfw was initialized in seconds. 

### function GetWindow

```cpp
GLFWwindow * GetWindow()
```

Returns the current active window. 

**Return**: GLFWwindow* The current active window. 

### function EnableMouseLock

```cpp
void EnableMouseLock()
```


### function DisableMouseLock

```cpp
void DisableMouseLock()
```


### function Terminate

```cpp
static void Terminate()
```

Calls GLFWTerminate to close the window. 

**Return**: void 

This is a static method so it can be called from anywhere to terminate the current window. 


### function WindowResized

```cpp
static void WindowResized(
    GLFWwindow * window,
    int width,
    int height
)
```

Callback for when the window is resized. 

**Parameters**: 

  * **GLFWwindow*** window 
  * **int** width, height The new client width and height of the window. 


**Return**: void 

### function KeyboardEventCallback

```cpp
static void KeyboardEventCallback(
    GLFWwindow * window,
    int key,
    int scancode,
    int action,
    int mods
)
```

Callback for keyboard events. 

**Parameters**: 

  * **GLFWwindow*** window 
  * **int** key ASCII key number. 
  * **int** scancode The converted key value 
  * **int** action The event type. 
  * **int** mods Modifier keys. 


**Return**: void 

### function CharacterEventCallback

```cpp
static void CharacterEventCallback(
    GLFWwindow * window,
    unsigned int uchar
)
```

Callback for Unicode character event. 

**Parameters**: 

  * **GLFWwindow*** window 
  * **unsigned** int uchar The Unicode character key code. 


**Return**: void 

This is different from just a normal keyboard event as it has been translated and modified by the [OS](/engine/Classes/classtec_1_1_o_s/) and is just like typing into a text editor. 


### function MouseMoveEventCallback

```cpp
static void MouseMoveEventCallback(
    GLFWwindow * window,
    double x,
    double y
)
```

Callback for mouse move events. 

**Parameters**: 

  * **GLFWwindow*** window 
  * **double** x, y The new x and y coordinate of the mouse in screen coordinates. 


**Return**: void 

### function MouseScrollEventCallback

```cpp
static void MouseScrollEventCallback(
    GLFWwindow * window,
    double x,
    double y
)
```

Callback for mouse scroll events. 

**Parameters**: 

  * **GLFWwindow*** window 
  * **double** x, y The delta x and y of the mouse wheel. 


**Return**: void 

### function MouseButtonEventCallback

```cpp
static void MouseButtonEventCallback(
    GLFWwindow * window,
    int button,
    int action,
    int mods
)
```

Callback for mouse button events. 

**Parameters**: 

  * **GLFWwindow*** window 
  * **int** button The button that the action occurred on. 
  * **int** action The action that occurred. 
  * **int** mods Modifier keys. 


**Return**: void 

### function WindowFocusChangeCallback

```cpp
static void WindowFocusChangeCallback(
    GLFWwindow * window,
    int focused
)
```

Callback for window focus change events. 

**Parameters**: 

  * **GLFWwindow*** window 
  * **int** focused GL_TRUE if focused, GL_FALSE if unfocused. 


**Return**: void 

### function FileDropCallback

```cpp
static void FileDropCallback(
    GLFWwindow * window,
    int count,
    const char ** paths
)
```

Callback for window focus change events. 

**Parameters**: 

  * **GLFWwindow*** window 
  * **int** count The number of files dropped. 
  * **const** char** paths Array of filenames. 


**Return**: void 

### function SetMousePosition

```cpp
static void SetMousePosition(
    const double x,
    const double y
)
```

Sets the mouse cursor position relative to the upper-left corner of the window. 

**Parameters**: 

  * **double** x, y The new x and y coordinate of the mouse in screen coordinates. 


**Return**: void 

### function GetMousePosition

```cpp
static void GetMousePosition(
    double * x,
    double * y
)
```

Gets the mouse cursor position relative to the upper-left corner of the window. 

**Parameters**: 

  * **double*** x, y The current x and y coordinate of the mouse in screen coordinates. 


**Return**: void 

### function GetFocusedWindow

```cpp
static inline const GLFWwindow * GetFocusedWindow()
```


### function RegisterLuaType

```cpp
static void RegisterLuaType(
    sol::state & 
)
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000