---
title: tec::GameState

---

# tec::GameState



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[GameState](Classes/structtec_1_1_game_state.md#function-gamestate)**() =default |
| | **[GameState](Classes/structtec_1_1_game_state.md#function-gamestate)**(const [GameState](Classes/structtec_1_1_game_state.md) & other) |
| | **[GameState](Classes/structtec_1_1_game_state.md#function-gamestate)**([GameState](Classes/structtec_1_1_game_state.md) && other) |
| [GameState](Classes/structtec_1_1_game_state.md) & | **[operator=](Classes/structtec_1_1_game_state.md#function-operator=)**(const [GameState](Classes/structtec_1_1_game_state.md) & other) |
| [GameState](Classes/structtec_1_1_game_state.md) & | **[operator=](Classes/structtec_1_1_game_state.md#function-operator=)**([GameState](Classes/structtec_1_1_game_state.md) && other) |
| void | **[In](Classes/structtec_1_1_game_state.md#function-in)**(const proto::GameStateUpdate & gsu) |
| void | **[Out](Classes/structtec_1_1_game_state.md#function-out)**(proto::GameStateUpdate * gsu) const |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::unordered_map< [eid](Namespaces/namespacetec.md#typedef-eid), [Position](Classes/structtec_1_1_position.md) > | **[positions](Classes/structtec_1_1_game_state.md#variable-positions)**  |
| std::unordered_map< [eid](Namespaces/namespacetec.md#typedef-eid), [Orientation](Classes/structtec_1_1_orientation.md) > | **[orientations](Classes/structtec_1_1_game_state.md#variable-orientations)**  |
| std::unordered_map< [eid](Namespaces/namespacetec.md#typedef-eid), [Velocity](Classes/structtec_1_1_velocity.md) > | **[velocities](Classes/structtec_1_1_game_state.md#variable-velocities)**  |
| state_id_t | **[state_id](Classes/structtec_1_1_game_state.md#variable-state_id)**  |
| state_id_t | **[command_id](Classes/structtec_1_1_game_state.md#variable-command_id)**  |
| uint64_t | **[timestamp](Classes/structtec_1_1_game_state.md#variable-timestamp)**  |

## Public Functions Documentation

### function GameState

```cpp
GameState() =default
```


### function GameState

```cpp
inline GameState(
    const GameState & other
)
```


### function GameState

```cpp
inline GameState(
    GameState && other
)
```


### function operator=

```cpp
inline GameState & operator=(
    const GameState & other
)
```


### function operator=

```cpp
inline GameState & operator=(
    GameState && other
)
```


### function In

```cpp
inline void In(
    const proto::GameStateUpdate & gsu
)
```


### function Out

```cpp
inline void Out(
    proto::GameStateUpdate * gsu
) const
```


## Public Attributes Documentation

### variable positions

```cpp
std::unordered_map< eid, Position > positions;
```


### variable orientations

```cpp
std::unordered_map< eid, Orientation > orientations;
```


### variable velocities

```cpp
std::unordered_map< eid, Velocity > velocities;
```


### variable state_id

```cpp
state_id_t state_id = 0;
```


### variable command_id

```cpp
state_id_t command_id = 0;
```


### variable timestamp

```cpp
uint64_t timestamp = 0;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC