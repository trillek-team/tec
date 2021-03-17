---
title: tec::ServerStats

---

# tec::ServerStats



## Public Attributes

|                | Name           |
| -------------- | -------------- |
| uint64_t | **[estimated_server_time](Classes/classtec_1_1_server_stats.md#variable-estimated_server_time)**  |
| uint64_t | **[last_state_time](Classes/classtec_1_1_server_stats.md#variable-last_state_time)**  |
| size_t | **[server_state_count](Classes/classtec_1_1_server_stats.md#variable-server_state_count)**  |
| state_id_t | **[current_command_id](Classes/classtec_1_1_server_stats.md#variable-current_command_id)**  |
| state_id_t | **[current_acked_id](Classes/classtec_1_1_server_stats.md#variable-current_acked_id)**  |
| glm::vec3 | **[server_position](Classes/classtec_1_1_server_stats.md#variable-server_position)**  |
| glm::vec3 | **[server_position_next](Classes/classtec_1_1_server_stats.md#variable-server_position_next)**  |
| glm::vec3 | **[client_position](Classes/classtec_1_1_server_stats.md#variable-client_position)**  |
| glm::vec3 | **[client_velocity](Classes/classtec_1_1_server_stats.md#variable-client_velocity)**  |
| uint64_t | **[estimated_delay](Classes/classtec_1_1_server_stats.md#variable-estimated_delay)**  |
| uint64_t | **[estimated_delay_accumulator](Classes/classtec_1_1_server_stats.md#variable-estimated_delay_accumulator)**  |
| size_t | **[estimated_delay_count](Classes/classtec_1_1_server_stats.md#variable-estimated_delay_count)**  |

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

Updated on 17 March 2021 at 23:08:11 UTC