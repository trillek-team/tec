---
title: tec::Voxel

---

# tec::Voxel



## Public Types

|                | Name           |
| -------------- | -------------- |
| enum| **[NEIGHBORS](Classes/structtec_1_1_voxel.md#enum-neighbors)** { UP = 0, DOWN, LEFT, RIGHT, FRONT, BACK} |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| float | **[color](Classes/structtec_1_1_voxel.md#variable-color)**  |
| std::weak_ptr< [Voxel](Classes/structtec_1_1_voxel.md) > | **[neighbors](Classes/structtec_1_1_voxel.md#variable-neighbors)**  |

## Public Types Documentation

### enum NEIGHBORS

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| UP | 0|   |
| DOWN | |   |
| LEFT | |   |
| RIGHT | |   |
| FRONT | |   |
| BACK | |   |




## Public Attributes Documentation

### variable color

```cpp
float color {0.f, 0.f, 0.f};
```


### variable neighbors

```cpp
std::weak_ptr< Voxel > neighbors;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC