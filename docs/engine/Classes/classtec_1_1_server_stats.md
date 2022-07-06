---
title: tec::ServerStats

---

# tec::ServerStats





## Public Attributes

|                | Name           |
| -------------- | -------------- |
| uint64_t | **[estimated_server_time](/engine/Classes/classtec_1_1_server_stats/#variable-estimated-server-time)**  |
| uint64_t | **[last_state_time](/engine/Classes/classtec_1_1_server_stats/#variable-last-state-time)**  |
| size_t | **[server_state_count](/engine/Classes/classtec_1_1_server_stats/#variable-server-state-count)**  |
| state_id_t | **[current_command_id](/engine/Classes/classtec_1_1_server_stats/#variable-current-command-id)**  |
| state_id_t | **[current_acked_id](/engine/Classes/classtec_1_1_server_stats/#variable-current-acked-id)**  |
| glm::vec3 | **[server_position](/engine/Classes/classtec_1_1_server_stats/#variable-server-position)**  |
| glm::vec3 | **[server_position_next](/engine/Classes/classtec_1_1_server_stats/#variable-server-position-next)**  |
| glm::vec3 | **[client_position](/engine/Classes/classtec_1_1_server_stats/#variable-client-position)**  |
| glm::vec3 | **[client_velocity](/engine/Classes/classtec_1_1_server_stats/#variable-client-velocity)**  |
| uint64_t | **[estimated_delay](/engine/Classes/classtec_1_1_server_stats/#variable-estimated-delay)**  |
| uint64_t | **[estimated_delay_accumulator](/engine/Classes/classtec_1_1_server_stats/#variable-estimated-delay-accumulator)**  |
| size_t | **[estimated_delay_count](/engine/Classes/classtec_1_1_server_stats/#variable-estimated-delay-count)**  |

## Public Attributes Documentation

### variable estimated_server_time

```cpp
uint64_t estimated_server_time;
```


### variable last_state_time

```cpp
uint64_t last_state_time;
```


### variable server_state_count

```cpp
size_t server_state_count;
```


### variable current_command_id

```cpp
state_id_t current_command_id;
```


### variable current_acked_id

```cpp
state_id_t current_acked_id;
```


### variable server_position

```cpp
glm::vec3 server_position;
```


### variable server_position_next

```cpp
glm::vec3 server_position_next;
```


### variable client_position

```cpp
glm::vec3 client_position;
```


### variable client_velocity

```cpp
glm::vec3 client_velocity;
```


### variable estimated_delay

```cpp
uint64_t estimated_delay;
```


### variable estimated_delay_accumulator

```cpp
uint64_t estimated_delay_accumulator;
```


### variable estimated_delay_count

```cpp
size_t estimated_delay_count;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000