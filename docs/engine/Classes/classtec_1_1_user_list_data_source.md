---
title: tec::UserListDataSource

---

# tec::UserListDataSource





Inherits from [tec::util::DataSource< User >](/engine/Classes/classtec_1_1util_1_1_data_source/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[UserListDataSource](/engine/Classes/classtec_1_1_user_list_data_source/#function-userlistdatasource)**([UserList](/engine/Classes/classtec_1_1_user_list/) & _user_list) |
| virtual void | **[Add](/engine/Classes/classtec_1_1_user_list_data_source/#function-add)**([User](/engine/Classes/classtec_1_1user_1_1_user/) user) override |
| virtual void | **[RemoveWhere](/engine/Classes/classtec_1_1_user_list_data_source/#function-removewhere)**(std::function< bool(const [User](/engine/Classes/classtec_1_1user_1_1_user/) &)> predicate) override |
| virtual [User](/engine/Classes/classtec_1_1user_1_1_user/) * | **[FindWhere](/engine/Classes/classtec_1_1_user_list_data_source/#function-findwhere)**(std::function< bool(const [User](/engine/Classes/classtec_1_1user_1_1_user/) &)> predicate) override |

## Public Functions Documentation

### function UserListDataSource

```cpp
inline UserListDataSource(
    UserList & _user_list
)
```


### function Add

```cpp
inline virtual void Add(
    User user
) override
```


**Reimplements**: [tec::util::DataSource::Add](/engine/Classes/classtec_1_1util_1_1_data_source/#function-add)


### function RemoveWhere

```cpp
inline virtual void RemoveWhere(
    std::function< bool(const User &)> predicate
) override
```


**Reimplements**: [tec::util::DataSource::RemoveWhere](/engine/Classes/classtec_1_1util_1_1_data_source/#function-removewhere)


### function FindWhere

```cpp
inline virtual User * FindWhere(
    std::function< bool(const User &)> predicate
) override
```


**Reimplements**: [tec::util::DataSource::FindWhere](/engine/Classes/classtec_1_1util_1_1_data_source/#function-findwhere)


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000