---
title: tec::LuaScript

---

# tec::LuaScript





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[LuaScript](/engine/Classes/structtec_1_1_lua_script/#function-luascript)**(std::shared_ptr< [ScriptFile](/engine/Classes/classtec_1_1_script_file/) > scriptfile) |
| | **[LuaScript](/engine/Classes/structtec_1_1_lua_script/#function-luascript)**() |
| void | **[Out](/engine/Classes/structtec_1_1_lua_script/#function-out)**(proto::Component * target) |
| void | **[In](/engine/Classes/structtec_1_1_lua_script/#function-in)**(const proto::Component & source) |
| void | **[SetupEnvironment](/engine/Classes/structtec_1_1_lua_script/#function-setupenvironment)**(sol::state * global_state) |
| void | **[ReloadScript](/engine/Classes/structtec_1_1_lua_script/#function-reloadscript)**() |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::string | **[script_name](/engine/Classes/structtec_1_1_lua_script/#variable-script-name)**  |
| std::shared_ptr< [ScriptFile](/engine/Classes/classtec_1_1_script_file/) > | **[script](/engine/Classes/structtec_1_1_lua_script/#variable-script)**  |
| sol::environment | **[environment](/engine/Classes/structtec_1_1_lua_script/#variable-environment)**  |
| sol::state * | **[global_state](/engine/Classes/structtec_1_1_lua_script/#variable-global-state)**  |

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

Updated on 2022-07-06 at 04:15:07 +0000