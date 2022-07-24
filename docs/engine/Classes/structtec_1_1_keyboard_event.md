---
title: tec::KeyboardEvent

---

# tec::KeyboardEvent






`#include <events.hpp>`

## Public Types

|                | Name           |
| -------------- | -------------- |
| enum| **[KEY_ACTION](/engine/Classes/structtec_1_1_keyboard_event/#enum-key-action)** { KEY_DOWN, KEY_UP, KEY_REPEAT, KEY_CHAR} |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| int | **[key](/engine/Classes/structtec_1_1_keyboard_event/#variable-key)**  |
| int | **[scancode](/engine/Classes/structtec_1_1_keyboard_event/#variable-scancode)**  |
| [KEY_ACTION](/engine/Classes/structtec_1_1_keyboard_event/#enum-key-action) | **[action](/engine/Classes/structtec_1_1_keyboard_event/#variable-action)**  |
| int | **[mods](/engine/Classes/structtec_1_1_keyboard_event/#variable-mods)**  |

## Public Types Documentation

### enum KEY_ACTION

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| KEY_DOWN | |   |
| KEY_UP | |   |
| KEY_REPEAT | |   |
| KEY_CHAR | |   |




## Public Attributes Documentation

### variable key

```cpp
int key {0};
```


### variable scancode

```cpp
int scancode {0};
```


### variable action

```cpp
KEY_ACTION action;
```


### variable mods

```cpp
int mods {0};
```


-------------------------------

Updated on 2022-07-24 at 19:27:35 +0000