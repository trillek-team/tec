---
title: tec::PhysicsDebugDrawer

---

# tec::PhysicsDebugDrawer



Inherits from btIDebugDraw

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[PhysicsDebugDrawer](Classes/classtec_1_1_physics_debug_drawer.md#function-physicsdebugdrawer)**() |
| virtual void | **[drawLine](Classes/classtec_1_1_physics_debug_drawer.md#function-drawline)**(const btVector3 & from, const btVector3 & to, const btVector3 & color) |
| virtual void | **[drawAABB](Classes/classtec_1_1_physics_debug_drawer.md#function-drawaabb)**(const btVector3 & from, const btVector3 & to, const btVector3 & color) |
| virtual void | **[drawContactPoint](Classes/classtec_1_1_physics_debug_drawer.md#function-drawcontactpoint)**(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color) |
| virtual void | **[reportErrorWarning](Classes/classtec_1_1_physics_debug_drawer.md#function-reporterrorwarning)**(const char * warningString) |
| virtual void | **[draw3dText](Classes/classtec_1_1_physics_debug_drawer.md#function-draw3dtext)**(const btVector3 & location, const char * textString) |
| virtual void | **[setDebugMode](Classes/classtec_1_1_physics_debug_drawer.md#function-setdebugmode)**(int debugMode) |
| virtual int | **[getDebugMode](Classes/classtec_1_1_physics_debug_drawer.md#function-getdebugmode)**() const |
| void | **[UpdateVertexBuffer](Classes/classtec_1_1_physics_debug_drawer.md#function-updatevertexbuffer)**() |

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

Updated on 17 March 2021 at 23:08:11 UTC