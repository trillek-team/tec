---
title: tec::Game

---

# tec::Game



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Game](Classes/classtec_1_1_game.md#function-game)**([OS](Classes/classtec_1_1_o_s.md) & _os, std::string config_file_name ="scripts/config.lua") |
| | **[~Game](Classes/classtec_1_1_game.md#function-~game)**() |
| void | **[Startup](Classes/classtec_1_1_game.md#function-startup)**() |
| void | **[Update](Classes/classtec_1_1_game.md#function-update)**(double delta, double mouse_x, double mouse_y, int window_width, int window_height) |
| [ServerConnection](Classes/classtec_1_1networking_1_1_server_connection.md) & | **[GetServerConnection](Classes/classtec_1_1_game.md#function-getserverconnection)**() |
| const [eid](Namespaces/namespacetec.md#typedef-eid) | **[GetActiveEntity](Classes/classtec_1_1_game.md#function-getactiveentity)**() const |
| [LuaSystem](Classes/classtec_1_1_lua_system.md) * | **[GetLuaSystem](Classes/classtec_1_1_game.md#function-getluasystem)**() |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [LuaScript](Classes/structtec_1_1_lua_script.md) > | **[config_script](Classes/classtec_1_1_game.md#variable-config_script)**  |
| unsigned int | **[fps](Classes/classtec_1_1_game.md#variable-fps)**  |
| float | **[avg_frame_time](Classes/classtec_1_1_game.md#variable-avg_frame_time)**  |
| [TimeFrameMetrics](Classes/structtec_1_1_time_frame_metrics.md) | **[tfm](Classes/classtec_1_1_game.md#variable-tfm)**  |

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

Updated on 17 March 2021 at 23:08:11 UTC