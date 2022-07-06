---
title: tec::GameState

---

# tec::GameState





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[GameState](/engine/Classes/structtec_1_1_game_state/#function-gamestate)**() =default |
| | **[GameState](/engine/Classes/structtec_1_1_game_state/#function-gamestate)**(const [GameState](/engine/Classes/structtec_1_1_game_state/) & other) |
| | **[GameState](/engine/Classes/structtec_1_1_game_state/#function-gamestate)**([GameState](/engine/Classes/structtec_1_1_game_state/) && other) |
| [GameState](/engine/Classes/structtec_1_1_game_state/) & | **[operator=](/engine/Classes/structtec_1_1_game_state/#function-operator=)**(const [GameState](/engine/Classes/structtec_1_1_game_state/) & other) |
| [GameState](/engine/Classes/structtec_1_1_game_state/) & | **[operator=](/engine/Classes/structtec_1_1_game_state/#function-operator=)**([GameState](/engine/Classes/structtec_1_1_game_state/) && other) |
| void | **[In](/engine/Classes/structtec_1_1_game_state/#function-in)**(const proto::GameStateUpdate & gsu) |
| void | **[Out](/engine/Classes/structtec_1_1_game_state/#function-out)**(proto::GameStateUpdate * gsu) const |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::unordered_map< [eid](/engine/Namespaces/namespacetec/#typedef-eid), [Position](/engine/Classes/structtec_1_1_position/) > | **[positions](/engine/Classes/structtec_1_1_game_state/#variable-positions)**  |
| std::unordered_map< [eid](/engine/Namespaces/namespacetec/#typedef-eid), [Orientation](/engine/Classes/structtec_1_1_orientation/) > | **[orientations](/engine/Classes/structtec_1_1_game_state/#variable-orientations)**  |
| std::unordered_map< [eid](/engine/Namespaces/namespacetec/#typedef-eid), [Velocity](/engine/Classes/structtec_1_1_velocity/) > | **[velocities](/engine/Classes/structtec_1_1_game_state/#variable-velocities)**  |
| state_id_t | **[state_id](/engine/Classes/structtec_1_1_game_state/#variable-state-id)**  |
| state_id_t | **[command_id](/engine/Classes/structtec_1_1_game_state/#variable-command-id)**  |
| uint64_t | **[timestamp](/engine/Classes/structtec_1_1_game_state/#variable-timestamp)**  |

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

Updated on 2022-07-06 at 04:15:07 +0000