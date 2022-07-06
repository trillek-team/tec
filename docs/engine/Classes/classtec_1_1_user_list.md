---
title: tec::UserList

---

# tec::UserList





## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[AddUser](/engine/Classes/classtec_1_1_user_list/#function-adduser)**([User](/engine/Classes/classtec_1_1user_1_1_user/) user) |
| [User](/engine/Classes/classtec_1_1user_1_1_user/) * | **[CreateUser](/engine/Classes/classtec_1_1_user_list/#function-createuser)**([uid](/engine/Namespaces/namespacetec/#typedef-uid) user_id) |
| [User](/engine/Classes/classtec_1_1user_1_1_user/) * | **[GetUser](/engine/Classes/classtec_1_1_user_list/#function-getuser)**([uid](/engine/Namespaces/namespacetec/#typedef-uid) id) |
| bool | **[RemoveUser](/engine/Classes/classtec_1_1_user_list/#function-removeuser)**([uid](/engine/Namespaces/namespacetec/#typedef-uid) id) |
| bool | **[HasUser](/engine/Classes/classtec_1_1_user_list/#function-hasuser)**([uid](/engine/Namespaces/namespacetec/#typedef-uid) id) |
| std::list< [User](/engine/Classes/classtec_1_1user_1_1_user/) > * | **[GetUsers](/engine/Classes/classtec_1_1_user_list/#function-getusers)**() |
| void | **[RegisterLuaType](/engine/Classes/classtec_1_1_user_list/#function-registerluatype)**(sol::state & ) |

## Public Functions Documentation

### function AddUser

```cpp
void AddUser(
    User user
)
```


### function CreateUser

```cpp
User * CreateUser(
    uid user_id
)
```


### function GetUser

```cpp
User * GetUser(
    uid id
)
```


### function RemoveUser

```cpp
bool RemoveUser(
    uid id
)
```


### function HasUser

```cpp
bool HasUser(
    uid id
)
```


### function GetUsers

```cpp
inline std::list< User > * GetUsers()
```


### function RegisterLuaType

```cpp
static void RegisterLuaType(
    sol::state & 
)
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000