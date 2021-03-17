---
title: tec::KeyboardEvent

---

# tec::KeyboardEvent



## Public Types

|                | Name           |
| -------------- | -------------- |
| enum| **[KEY_ACTION](Classes/structtec_1_1_keyboard_event.md#enum-key_action)** { KEY_DOWN, KEY_UP, KEY_REPEAT, KEY_CHAR} |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| int | **[key](Classes/structtec_1_1_keyboard_event.md#variable-key)**  |
| int | **[scancode](Classes/structtec_1_1_keyboard_event.md#variable-scancode)**  |
| KEY_ACTION | **[action](Classes/structtec_1_1_keyboard_event.md#variable-action)**  |
| int | **[mods](Classes/structtec_1_1_keyboard_event.md#variable-mods)**  |

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

Updated on 17 March 2021 at 23:08:11 UTC