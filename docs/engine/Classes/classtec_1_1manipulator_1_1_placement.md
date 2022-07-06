---
title: tec::manipulator::Placement
summary: Manipulator used to place objects in the world. Can have a mesh assigned to provide visual feedback. 

---

# tec::manipulator::Placement



Manipulator used to place objects in the world. Can have a mesh assigned to provide visual feedback. 


`#include <placement.hpp>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[SetMaxDistance](/engine/Classes/classtec_1_1manipulator_1_1_placement/#function-setmaxdistance)**(float _max_distance =DEFAULT_MAX_PLACEMENT_DISTANCE) |
| void | **[SetMesh](/engine/Classes/classtec_1_1manipulator_1_1_placement/#function-setmesh)**(const std::shared_ptr< [MeshFile](/engine/Classes/classtec_1_1_mesh_file/) > _mesh)<br>Set the mesh used to represent the placement in the world.  |
| void | **[SetMesh](/engine/Classes/classtec_1_1manipulator_1_1_placement/#function-setmesh)**(const std::string mesh_name)<br>Set the mesh used to represent the placement in the world.  |
| void | **[ClearMesh](/engine/Classes/classtec_1_1manipulator_1_1_placement/#function-clearmesh)**()<br>Clear the mesh, effectively not longer rendering the placement.  |
| void | **[SetRayIntersectionPoint](/engine/Classes/classtec_1_1manipulator_1_1_placement/#function-setrayintersectionpoint)**(const glm::vec3 start, const glm::vec3 intersection)<br>Sets the ray used to render the placement.  |
| void | **[PlaceEntityInWorld](/engine/Classes/classtec_1_1manipulator_1_1_placement/#function-placeentityinworld)**(glm::vec3 _position) |
| void | **[RegisterLuaType](/engine/Classes/classtec_1_1manipulator_1_1_placement/#function-registerluatype)**(sol::state & ) |

## Public Functions Documentation

### function SetMaxDistance

```cpp
void SetMaxDistance(
    float _max_distance =DEFAULT_MAX_PLACEMENT_DISTANCE
)
```


### function SetMesh

```cpp
void SetMesh(
    const std::shared_ptr< MeshFile > _mesh
)
```

Set the mesh used to represent the placement in the world. 

**Parameters**: 

  * **const** std::shared_ptr<MeshFile> mesh [MeshFile](/engine/Classes/classtec_1_1_mesh_file/) to render in the world. 


### function SetMesh

```cpp
void SetMesh(
    const std::string mesh_name
)
```

Set the mesh used to represent the placement in the world. 

**Parameters**: 

  * **const** std::string mesh_name Name of the mesh file to load. 


This version takes the mesh_name and locators the corresponding mesh_file and then calls the appropriate SetMesh;


### function ClearMesh

```cpp
void ClearMesh()
```

Clear the mesh, effectively not longer rendering the placement. 

### function SetRayIntersectionPoint

```cpp
void SetRayIntersectionPoint(
    const glm::vec3 start,
    const glm::vec3 intersection
)
```

Sets the ray used to render the placement. 

**Parameters**: 

  * **const** glm::vec3 start Starting point for the ray, usually the center of the screen. 
  * **const** glm::vec3 intersection Intersection point of the ray with the physics world. 


Sets the position in the world of the placement mesh based on the ray's start point and going to either the intersection point or the point along the ray that is MAX_DISTANCE towards the intersection point.


### function PlaceEntityInWorld

```cpp
void PlaceEntityInWorld(
    glm::vec3 _position
)
```


### function RegisterLuaType

```cpp
static void RegisterLuaType(
    sol::state & 
)
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000