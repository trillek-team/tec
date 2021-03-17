---
title: tec::Game

---

# tec::Game



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Game](/engine/Classes/classtec_1_1_game/#function-game)**([OS](/engine/Classes/classtec_1_1_o_s/) & _os, std::string config_file_name ="scripts/config.lua") |
| | **[~Game](/engine/Classes/classtec_1_1_game/#function-~game)**() |
| void | **[Startup](/engine/Classes/classtec_1_1_game/#function-startup)**() |
| void | **[Update](/engine/Classes/classtec_1_1_game/#function-update)**(double delta, double mouse_x, double mouse_y, int window_width, int window_height) |
| [ServerConnection](/engine/Classes/classtec_1_1networking_1_1_server_connection/) & | **[GetServerConnection](/engine/Classes/classtec_1_1_game/#function-getserverconnection)**() |
| const [eid](/engine/Namespaces/namespacetec/#typedef-eid) | **[GetActiveEntity](/engine/Classes/classtec_1_1_game/#function-getactiveentity)**() const |
| [LuaSystem](/engine/Classes/classtec_1_1_lua_system/) * | **[GetLuaSystem](/engine/Classes/classtec_1_1_game/#function-getluasystem)**() |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [LuaScript](/engine/Classes/structtec_1_1_lua_script/) > | **[config_script](/engine/Classes/classtec_1_1_game/#variable-config_script)**  |
| unsigned int | **[fps](/engine/Classes/classtec_1_1_game/#variable-fps)**  |
| float | **[avg_frame_time](/engine/Classes/classtec_1_1_game/#variable-avg_frame_time)**  |
| [TimeFrameMetrics](/engine/Classes/structtec_1_1_time_frame_metrics/) | **[tfm](/engine/Classes/classtec_1_1_game/#variable-tfm)**  |

## Public Functions Documentation

### function Game

```cpp
Game(
    OS & _os,
    std::string config_file_name ="scripts/config.lua"
)
```


### function ~Game

```cpp
~Game()
```


### function Startup

```cpp
void Startup()
```


### function Update

```cpp
void Update(
    double delta,
    double mouse_x,
    double mouse_y,
    int window_width,
    int window_height
)
```


### function GetServerConnection

```cpp
inline ServerConnection & GetServerConnection()
```


### function GetActiveEntity

```cpp
inline const eid GetActiveEntity() const
```


### function GetLuaSystem

```cpp
inline LuaSystem * GetLuaSystem()
```


## Public Attributes Documentation

### variable config_script

```cpp
std::shared_ptr< LuaScript > config_script;
```


### variable fps

```cpp
unsigned int fps;
```


### variable avg_frame_time

```cpp
float avg_frame_time;
```


### variable tfm

```cpp
TimeFrameMetrics tfm;
```


-------------------------------

Updated on 17 March 2021 at 22:09:28 UTC