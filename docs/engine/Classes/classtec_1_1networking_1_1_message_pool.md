---
title: tec::networking::MessagePool

---

# tec::networking::MessagePool



## Public Types

|                | Name           |
| -------------- | -------------- |
| typedef std::shared_ptr< [Message](Classes/classtec_1_1networking_1_1_message.md) > | **[ptr_type](Classes/classtec_1_1networking_1_1_message_pool.md#typedef-ptr_type)**  |
| typedef std::list< ptr_type > | **[list_type](Classes/classtec_1_1networking_1_1_message_pool.md#typedef-list_type)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| ptr_type | **[get](Classes/classtec_1_1networking_1_1_message_pool.md#function-get)**() |

## Public Types Documentation

### typedef ptr_type

```cpp
typedef std::shared_ptr<Message> tec::networking::MessagePool::ptr_type;
```


### typedef list_type

```cpp
typedef std::list<ptr_type> tec::networking::MessagePool::list_type;
```


## Public Functions Documentation

### function get

```cpp
static inline ptr_type get()
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC