---
title: tec::system::UserAuthenticator

---

# tec::system::UserAuthenticator






`#include <user-authenticator.hpp>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[SetDataSource](/engine/Classes/classtec_1_1system_1_1_user_authenticator/#function-setdatasource)**([util::DataSource](/engine/Classes/classtec_1_1util_1_1_data_source/)< [User](/engine/Classes/classtec_1_1user_1_1_user/) > * data_source) |
| void | **[AddUser](/engine/Classes/classtec_1_1system_1_1_user_authenticator/#function-adduser)**(std::string user_id, std::string username, std::string password ="") |
| [User](/engine/Classes/classtec_1_1user_1_1_user/) * | **[Authenticate](/engine/Classes/classtec_1_1system_1_1_user_authenticator/#function-authenticate)**(std::string username, std::string password ="") |

## Public Functions Documentation

### function SetDataSource

```cpp
inline void SetDataSource(
    util::DataSource< User > * data_source
)
```


### function AddUser

```cpp
inline void AddUser(
    std::string user_id,
    std::string username,
    std::string password =""
)
```


### function Authenticate

```cpp
inline User * Authenticate(
    std::string username,
    std::string password =""
)
```


-------------------------------

Updated on 2022-07-24 at 19:27:35 +0000