---
title: tec::LuaScript

---

# tec::LuaScript



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[LuaScript](Classes/structtec_1_1_lua_script.md#function-luascript)**(std::shared_ptr< [ScriptFile](Classes/classtec_1_1_script_file.md) > scriptfile) |
| | **[LuaScript](Classes/structtec_1_1_lua_script.md#function-luascript)**() |
| void | **[Out](Classes/structtec_1_1_lua_script.md#function-out)**(proto::Component * target) |
| void | **[In](Classes/structtec_1_1_lua_script.md#function-in)**(const proto::Component & source) |
| void | **[SetupEnvironment](Classes/structtec_1_1_lua_script.md#function-setupenvironment)**(sol::state * global_state) |
| void | **[ReloadScript](Classes/structtec_1_1_lua_script.md#function-reloadscript)**() |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::string | **[script_name](Classes/structtec_1_1_lua_script.md#variable-script_name)**  |
| std::shared_ptr< [ScriptFile](Classes/classtec_1_1_script_file.md) > | **[script](Classes/structtec_1_1_lua_script.md#variable-script)**  |
| sol::environment | **[environment](Classes/structtec_1_1_lua_script.md#variable-environment)**  |
| sol::state * | **[global_state](Classes/structtec_1_1_lua_script.md#variable-global_state)**  |

## Public Functions Documentation

### function LuaScript

```cpp
LuaScript(
    std::shared_ptr< ScriptFile > scriptfile
)
```


### function LuaScript

```cpp
LuaScript()
```


### function Out

```cpp
void Out(
    proto::Component * target
)
```


### function In

```cpp
void In(
    const proto::Component & source
)
```


### function SetupEnvironment

```cpp
void SetupEnvironment(
    sol::state * global_state
)
```


### function ReloadScript

```cpp
void ReloadScript()
```


## Public Attributes Documentation

### variable script_name

```cpp
std::string script_name;
```


### variable script

```cpp
std::shared_ptr< ScriptFile > script;
```


### variable environment

```cpp
sol::environment environment;
```


### variable global_state

```cpp
sol::state * global_state = nullptr;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC