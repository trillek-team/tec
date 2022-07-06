---
title: tec::UserLoginEvent

---

# tec::UserLoginEvent





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[UserLoginEvent](/engine/Classes/structtec_1_1_user_login_event/#function-userloginevent)**() =default |
| | **[UserLoginEvent](/engine/Classes/structtec_1_1_user_login_event/#function-userloginevent)**(proto::UserLogin user_login) |
| void | **[In](/engine/Classes/structtec_1_1_user_login_event/#function-in)**(proto::UserLogin user_login) |
| void | **[Out](/engine/Classes/structtec_1_1_user_login_event/#function-out)**(proto::UserLogin & user_login) |
| proto::UserLogin | **[Out](/engine/Classes/structtec_1_1_user_login_event/#function-out)**() |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::string | **[username](/engine/Classes/structtec_1_1_user_login_event/#variable-username)**  |
| std::string | **[password](/engine/Classes/structtec_1_1_user_login_event/#variable-password)**  |

## Public Functions Documentation

### function UserLoginEvent

```cpp
UserLoginEvent() =default
```


### function UserLoginEvent

```cpp
inline UserLoginEvent(
    proto::UserLogin user_login
)
```


### function In

```cpp
inline void In(
    proto::UserLogin user_login
)
```


### function Out

```cpp
inline void Out(
    proto::UserLogin & user_login
)
```


### function Out

```cpp
inline proto::UserLogin Out()
```


## Public Attributes Documentation

### variable username

```cpp
std::string username;
```


### variable password

```cpp
std::string password;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000