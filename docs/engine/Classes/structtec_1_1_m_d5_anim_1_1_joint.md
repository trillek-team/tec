---
title: tec::MD5Anim::Joint

---

# tec::MD5Anim::Joint



## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[ComputeW](/engine/Classes/structtec_1_1_m_d5_anim_1_1_joint/#function-computew)**() |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::string | **[name](/engine/Classes/structtec_1_1_m_d5_anim_1_1_joint/#variable-name)**  |
| int | **[parent](/engine/Classes/structtec_1_1_m_d5_anim_1_1_joint/#variable-parent)**  |
| int | **[flags](/engine/Classes/structtec_1_1_m_d5_anim_1_1_joint/#variable-flags)**  |
| int | **[start_index](/engine/Classes/structtec_1_1_m_d5_anim_1_1_joint/#variable-start_index)**  |
| glm::vec3 | **[base_position](/engine/Classes/structtec_1_1_m_d5_anim_1_1_joint/#variable-base_position)**  |
| glm::quat | **[base_orientation](/engine/Classes/structtec_1_1_m_d5_anim_1_1_joint/#variable-base_orientation)**  |
| glm::mat4 | **[bind_pose_inverse](/engine/Classes/structtec_1_1_m_d5_anim_1_1_joint/#variable-bind_pose_inverse)**  |

## Public Functions Documentation

### function ComputeW

```cpp
void ComputeW()
```


## Public Attributes Documentation

### variable name

```cpp
std::string name;
```


### variable parent

```cpp
int parent {0};
```


### variable flags

```cpp
int flags {0};
```


### variable start_index

```cpp
int start_index {0};
```


### variable base_position

```cpp
glm::vec3 base_position {0.f, 0.f, 0.f};
```


### variable base_orientation

```cpp
glm::quat base_orientation {0.f, 0.f, 0.f, 1.f};
```


### variable bind_pose_inverse

```cpp
glm::mat4 bind_pose_inverse {0.f};
```


-------------------------------

Updated on 21 March 2021 at 16:48:25 UTC