---
title: tec::MD5Mesh::Joint

---

# tec::MD5Mesh::Joint



## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[ComputeW](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_joint/#function-computew)**()<br>Compute the joint's quaternion W component.  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::string | **[name](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_joint/#variable-name)**  |
| int | **[parent](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_joint/#variable-parent)**  |
| glm::vec3 | **[position](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_joint/#variable-position)**  |
| glm::quat | **[orientation](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_joint/#variable-orientation)**  |
| glm::mat4 | **[bind_pose](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_joint/#variable-bind_pose)**  |
| glm::mat4 | **[bind_pose_inverse](/engine/Classes/structtec_1_1_m_d5_mesh_1_1_joint/#variable-bind_pose_inverse)**  |

## Public Functions Documentation

### function ComputeW

```cpp
void ComputeW()
```

Compute the joint's quaternion W component. 

**Return**: void 

## Public Attributes Documentation

### variable name

```cpp
std::string name {""};
```


### variable parent

```cpp
int parent {-1};
```


### variable position

```cpp
glm::vec3 position {0.f, 0.f, 0.f};
```


### variable orientation

```cpp
glm::quat orientation {0.f, 0.f, 0.f, 1.f};
```


### variable bind_pose

```cpp
glm::mat4 bind_pose {0.f};
```


### variable bind_pose_inverse

```cpp
glm::mat4 bind_pose_inverse {0.f};
```


-------------------------------

Updated on 21 March 2021 at 16:29:13 UTC