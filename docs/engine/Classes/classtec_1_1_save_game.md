---
title: tec::SaveGame

---

# tec::SaveGame





## Public Functions

|                | Name           |
| -------------- | -------------- |
| bool | **[Load](/engine/Classes/classtec_1_1_save_game/#function-load)**(const Path) |
| bool | **[Load](/engine/Classes/classtec_1_1_save_game/#function-load)**(std::string _filename) |
| bool | **[Reload](/engine/Classes/classtec_1_1_save_game/#function-reload)**() |
| bool | **[Reload](/engine/Classes/classtec_1_1_save_game/#function-reload)**(const Path) |
| bool | **[Save](/engine/Classes/classtec_1_1_save_game/#function-save)**() |
| bool | **[Save](/engine/Classes/classtec_1_1_save_game/#function-save)**(const Path) |
| [UserList](/engine/Classes/classtec_1_1_user_list/) * | **[GetUserList](/engine/Classes/classtec_1_1_save_game/#function-getuserlist)**() |
| void | **[RegisterLuaType](/engine/Classes/classtec_1_1_save_game/#function-registerluatype)**(sol::state & ) |

## Public Functions Documentation

### function Load

```cpp
bool Load(
    const Path
)
```


### function Load

```cpp
bool Load(
    std::string _filename
)
```


### function Reload

```cpp
bool Reload()
```


### function Reload

```cpp
bool Reload(
    const Path
)
```


### function Save

```cpp
bool Save()
```


### function Save

```cpp
bool Save(
    const Path
)
```


### function GetUserList

```cpp
UserList * GetUserList()
```


### function RegisterLuaType

```cpp
static void RegisterLuaType(
    sol::state & 
)
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000