---
title: tec::util::DataSource

---

# tec::util::DataSource



 [More...](#detailed-description)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| virtual void | **[Add](/engine/Classes/classtec_1_1util_1_1_data_source/#function-add)**(T value) =0 |
| virtual void | **[RemoveWhere](/engine/Classes/classtec_1_1util_1_1_data_source/#function-removewhere)**(std::function< bool(const T &)> predicate) =0 |
| virtual T * | **[FindWhere](/engine/Classes/classtec_1_1util_1_1_data_source/#function-findwhere)**(std::function< bool(const T &)> predicate) =0 |

## Detailed Description

```cpp
template <typename T >
class tec::util::DataSource;
```

## Public Functions Documentation

### function Add

```cpp
virtual void Add(
    T value
) =0
```


**Reimplemented by**: [tec::UserListDataSource::Add](/engine/Classes/classtec_1_1_user_list_data_source/#function-add)


### function RemoveWhere

```cpp
virtual void RemoveWhere(
    std::function< bool(const T &)> predicate
) =0
```


**Reimplemented by**: [tec::UserListDataSource::RemoveWhere](/engine/Classes/classtec_1_1_user_list_data_source/#function-removewhere)


### function FindWhere

```cpp
virtual T * FindWhere(
    std::function< bool(const T &)> predicate
) =0
```


**Reimplemented by**: [tec::UserListDataSource::FindWhere](/engine/Classes/classtec_1_1_user_list_data_source/#function-findwhere)


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000