---
title: tec::PhysicsDebugDrawer

---

# tec::PhysicsDebugDrawer





Inherits from btIDebugDraw

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[PhysicsDebugDrawer](/engine/Classes/classtec_1_1_physics_debug_drawer/#function-physicsdebugdrawer)**() |
| virtual void | **[drawLine](/engine/Classes/classtec_1_1_physics_debug_drawer/#function-drawline)**(const btVector3 & from, const btVector3 & to, const btVector3 & color) |
| virtual void | **[drawAABB](/engine/Classes/classtec_1_1_physics_debug_drawer/#function-drawaabb)**(const btVector3 & from, const btVector3 & to, const btVector3 & color) |
| virtual void | **[drawContactPoint](/engine/Classes/classtec_1_1_physics_debug_drawer/#function-drawcontactpoint)**(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color) |
| virtual void | **[reportErrorWarning](/engine/Classes/classtec_1_1_physics_debug_drawer/#function-reporterrorwarning)**(const char * warningString) |
| virtual void | **[draw3dText](/engine/Classes/classtec_1_1_physics_debug_drawer/#function-draw3dtext)**(const btVector3 & location, const char * textString) |
| virtual void | **[setDebugMode](/engine/Classes/classtec_1_1_physics_debug_drawer/#function-setdebugmode)**(int debugMode) |
| virtual int | **[getDebugMode](/engine/Classes/classtec_1_1_physics_debug_drawer/#function-getdebugmode)**() const |
| void | **[UpdateVertexBuffer](/engine/Classes/classtec_1_1_physics_debug_drawer/#function-updatevertexbuffer)**() |

## Public Functions Documentation

### function PhysicsDebugDrawer

```cpp
PhysicsDebugDrawer()
```


### function drawLine

```cpp
virtual void drawLine(
    const btVector3 & from,
    const btVector3 & to,
    const btVector3 & color
)
```


### function drawAABB

```cpp
virtual void drawAABB(
    const btVector3 & from,
    const btVector3 & to,
    const btVector3 & color
)
```


### function drawContactPoint

```cpp
virtual void drawContactPoint(
    const btVector3 & PointOnB,
    const btVector3 & normalOnB,
    btScalar distance,
    int lifeTime,
    const btVector3 & color
)
```


### function reportErrorWarning

```cpp
virtual void reportErrorWarning(
    const char * warningString
)
```


### function draw3dText

```cpp
virtual void draw3dText(
    const btVector3 & location,
    const char * textString
)
```


### function setDebugMode

```cpp
virtual void setDebugMode(
    int debugMode
)
```


### function getDebugMode

```cpp
inline virtual int getDebugMode() const
```


### function UpdateVertexBuffer

```cpp
void UpdateVertexBuffer()
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000