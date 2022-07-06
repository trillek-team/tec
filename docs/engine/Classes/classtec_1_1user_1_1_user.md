---
title: tec::user::User
summary: Container used to store information about a logged in user. 

---

# tec::user::User



Container used to store information about a logged in user.  [More...](#detailed-description)


`#include <user.hpp>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[User](/engine/Classes/classtec_1_1user_1_1_user/#function-user)**() =default |
| | **[~User](/engine/Classes/classtec_1_1user_1_1_user/#function-~user)**() |
| void | **[AddEntityToWorld](/engine/Classes/classtec_1_1user_1_1_user/#function-addentitytoworld)**()<br>Add the user's entity to the world by emitting [EntityCreated](/engine/Classes/structtec_1_1_entity_created/) and [ControllerAddedEvent](/engine/Classes/structtec_1_1_controller_added_event/) events.  |
| void | **[RemoveEntityFromWorld](/engine/Classes/classtec_1_1user_1_1_user/#function-removeentityfromworld)**()<br>Add the user's entity to the world by emitting EntityDestroy and [ControllerRemovedEvent](/engine/Classes/structtec_1_1_controller_removed_event/) events.  |
| [uid](/engine/Namespaces/namespacetec/#typedef-uid) | **[GetUserId](/engine/Classes/classtec_1_1user_1_1_user/#function-getuserid)**() const<br>Get the user's ID.  |
| void | **[SetUserId](/engine/Classes/classtec_1_1user_1_1_user/#function-setuserid)**([uid](/engine/Namespaces/namespacetec/#typedef-uid) user_id) |
| [eid](/engine/Namespaces/namespacetec/#typedef-eid) | **[GetEntityId](/engine/Classes/classtec_1_1user_1_1_user/#function-getentityid)**() const<br>Get the underlying entity's ID.  |
| std::string | **[GetUsername](/engine/Classes/classtec_1_1user_1_1_user/#function-getusername)**() const<br>Get the username associated with this user.  |
| void | **[SetUsername](/engine/Classes/classtec_1_1user_1_1_user/#function-setusername)**(std::string username) |
| void | **[Out](/engine/Classes/classtec_1_1user_1_1_user/#function-out)**(proto::User * target)<br>Serialize to a proto::User.  |
| void | **[In](/engine/Classes/classtec_1_1user_1_1_user/#function-in)**(const proto::User & source)<br>Deserialize from a proto::User.  |
| void | **[RegisterLuaType](/engine/Classes/classtec_1_1user_1_1_user/#function-registerluatype)**(sol::state & ) |

## Detailed Description

```cpp
class tec::user::User;
```

Container used to store information about a logged in user. 

Information such as the user's id, username, inventory, etc will be connect here.

Additionally users contain an [Entity](/engine/Classes/classtec_1_1_entity/) that can be used to access the user's state in the world for purposes of retrieving position, health, etc components. 

## Public Functions Documentation

### function User

```cpp
User() =default
```


### function ~User

```cpp
~User()
```


### function AddEntityToWorld

```cpp
void AddEntityToWorld()
```

Add the user's entity to the world by emitting [EntityCreated](/engine/Classes/structtec_1_1_entity_created/) and [ControllerAddedEvent](/engine/Classes/structtec_1_1_controller_added_event/) events. 

Call this after all user entity state has been set. 


### function RemoveEntityFromWorld

```cpp
void RemoveEntityFromWorld()
```

Add the user's entity to the world by emitting EntityDestroy and [ControllerRemovedEvent](/engine/Classes/structtec_1_1_controller_removed_event/) events. 

Call this when the user is no longer in the world e.g. disconnect. 


### function GetUserId

```cpp
inline uid GetUserId() const
```

Get the user's ID. 

**Return**: uid Users' ID. 

This ID is different than an entity ID in that it is assign during registration and is not meant to vary whereas an entity is assigned at random when the user's entity is added to the world. 


### function SetUserId

```cpp
inline void SetUserId(
    uid user_id
)
```


### function GetEntityId

```cpp
inline eid GetEntityId() const
```

Get the underlying entity's ID. 

**Return**: eid The underlying entity's ID. 

### function GetUsername

```cpp
inline std::string GetUsername() const
```

Get the username associated with this user. 

**Return**: string [User](/engine/Classes/classtec_1_1user_1_1_user/)'s username. 

### function SetUsername

```cpp
inline void SetUsername(
    std::string username
)
```


### function Out

```cpp
void Out(
    proto::User * target
)
```

Serialize to a proto::User. 

**Parameters**: 

  * **proto::User*** target The proto::User to serialize to. 


### function In

```cpp
void In(
    const proto::User & source
)
```

Deserialize from a proto::User. 

**Parameters**: 

  * **const** proto::User& source The proto::User to Deserialize from. 


### function RegisterLuaType

```cpp
static void RegisterLuaType(
    sol::state & 
)
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000