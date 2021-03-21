---
title: tec::networking::MessagePool

---

# tec::networking::MessagePool



## Public Types

|                | Name           |
| -------------- | -------------- |
| typedef std::shared_ptr< [Message](/engine/Classes/classtec_1_1networking_1_1_message/) > | **[ptr_type](/engine/Classes/classtec_1_1networking_1_1_message_pool/#typedef-ptr_type)**  |
| typedef std::list< ptr_type > | **[list_type](/engine/Classes/classtec_1_1networking_1_1_message_pool/#typedef-list_type)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| ptr_type | **[get](/engine/Classes/classtec_1_1networking_1_1_message_pool/#function-get)**() |

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

Updated on 21 March 2021 at 16:51:04 UTC