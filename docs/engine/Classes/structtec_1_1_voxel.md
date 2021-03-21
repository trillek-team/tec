---
title: tec::Voxel

---

# tec::Voxel



## Public Types

|                | Name           |
| -------------- | -------------- |
| enum| **[NEIGHBORS](/engine/Classes/structtec_1_1_voxel/#enum-neighbors)** { UP = 0, DOWN, LEFT, RIGHT, FRONT, BACK} |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| float | **[color](/engine/Classes/structtec_1_1_voxel/#variable-color)**  |
| std::weak_ptr< [Voxel](/engine/Classes/structtec_1_1_voxel/) > | **[neighbors](/engine/Classes/structtec_1_1_voxel/#variable-neighbors)**  |

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

Updated on 21 March 2021 at 16:43:56 UTC