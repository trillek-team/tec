---
title: tec::UserList

---

# tec::UserList



## Public Functions

|                | Name           |
| -------------- | -------------- |
| template <typename T \> <br>void | **[SetUsers](/engine/Classes/classtec_1_1_user_list/#function-setusers)**(T begin, T end) |
| void | **[AddUser](/engine/Classes/classtec_1_1_user_list/#function-adduser)**(proto::User user) |
| const std::list< proto::User > * | **[GetUsers](/engine/Classes/classtec_1_1_user_list/#function-getusers)**() |
| proto::User * | **[GetUser](/engine/Classes/classtec_1_1_user_list/#function-getuser)**(uid id) |
| bool | **[RemoveUser](/engine/Classes/classtec_1_1_user_list/#function-removeuser)**(uid id) |
| bool | **[UserExists](/engine/Classes/classtec_1_1_user_list/#function-userexists)**(uid id) |

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

Updated on 21 March 2021 at 16:48:25 UTC