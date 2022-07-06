---
title: tec::LuaClassList

---

# tec::LuaClassList





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[LuaClassList](/engine/Classes/structtec_1_1_lua_class_list/#function-luaclasslist)**(std::function< void(sol::state &)> load_fn) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::function< void(sol::state &)> | **[load](/engine/Classes/structtec_1_1_lua_class_list/#variable-load)**  |
| [LuaClassList](/engine/Classes/structtec_1_1_lua_class_list/) * | **[next](/engine/Classes/structtec_1_1_lua_class_list/#variable-next)**  |

## Public Functions Documentation

### function LuaClassList

```cpp
inline LuaClassList(
    std::function< void(sol::state &)> load_fn
)
```


## Public Attributes Documentation

### variable load

```cpp
std::function< void(sol::state &)> load;
```


### variable next

```cpp
LuaClassList * next;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000