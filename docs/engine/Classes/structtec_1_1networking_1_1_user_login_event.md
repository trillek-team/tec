---
title: tec::networking::UserLoginEvent

---

# tec::networking::UserLoginEvent





## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[RegisterLuaType](/engine/Classes/structtec_1_1networking_1_1_user_login_event/#function-registerluatype)**(sol::state & ) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::string | **[username](/engine/Classes/structtec_1_1networking_1_1_user_login_event/#variable-username)**  |
| std::string | **[user_id](/engine/Classes/structtec_1_1networking_1_1_user_login_event/#variable-user-id)**  |
| std::string | **[reason](/engine/Classes/structtec_1_1networking_1_1_user_login_event/#variable-reason)**  |
| bool | **[authenticated](/engine/Classes/structtec_1_1networking_1_1_user_login_event/#variable-authenticated)**  |
| bool | **[reject](/engine/Classes/structtec_1_1networking_1_1_user_login_event/#variable-reject)**  |

## Public Functions Documentation

### function RegisterLuaType

```cpp
static void RegisterLuaType(
    sol::state & 
)
```


## Public Attributes Documentation

### variable username

```cpp
std::string username;
```


### variable user_id

```cpp
std::string user_id;
```


### variable reason

```cpp
std::string reason;
```


### variable authenticated

```cpp
bool authenticated {false};
```


### variable reject

```cpp
bool reject {false};
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000