---
title: tec::UserList

---

# tec::UserList



## Public Functions

|                | Name           |
| -------------- | -------------- |
| template <typename T \> <br>void | **[SetUsers](Classes/classtec_1_1_user_list.md#function-setusers)**(T begin, T end) |
| void | **[AddUser](Classes/classtec_1_1_user_list.md#function-adduser)**(proto::User user) |
| const std::list< proto::User > * | **[GetUsers](Classes/classtec_1_1_user_list.md#function-getusers)**() |
| proto::User * | **[GetUser](Classes/classtec_1_1_user_list.md#function-getuser)**(uid id) |
| bool | **[RemoveUser](Classes/classtec_1_1_user_list.md#function-removeuser)**(uid id) |
| bool | **[UserExists](Classes/classtec_1_1_user_list.md#function-userexists)**(uid id) |

## Public Functions Documentation

### function SetUsers

```cpp
template <typename T >
void SetUsers(
    T begin,
    T end
)
```


### function AddUser

```cpp
void AddUser(
    proto::User user
)
```


### function GetUsers

```cpp
const std::list< proto::User > * GetUsers()
```


### function GetUser

```cpp
proto::User * GetUser(
    uid id
)
```


### function RemoveUser

```cpp
bool RemoveUser(
    uid id
)
```


### function UserExists

```cpp
bool UserExists(
    uid id
)
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC