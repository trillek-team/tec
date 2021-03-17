---
title: tec::Animation

---

# tec::Animation



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Animation](Classes/classtec_1_1_animation.md#function-animation)**() =default |
| | **[Animation](Classes/classtec_1_1_animation.md#function-animation)**(std::shared_ptr< [MD5Anim](Classes/classtec_1_1_m_d5_anim.md) > animation) |
| void | **[UpdateAnimation](Classes/classtec_1_1_animation.md#function-updateanimation)**(const double delta)<br>Updates the current animation based on a change in time.  |
| void | **[SetAnimationFile](Classes/classtec_1_1_animation.md#function-setanimationfile)**(std::shared_ptr< [MD5Anim](Classes/classtec_1_1_m_d5_anim.md) > file)<br>Sets the animation file for this animation.  |
| void | **[Out](Classes/classtec_1_1_animation.md#function-out)**(proto::Component * target) |
| void | **[In](Classes/classtec_1_1_animation.md#function-in)**(const proto::Component & source) |

## Friends

|                | Name           |
| -------------- | -------------- |
| class | **[RenderSystem](Classes/classtec_1_1_animation.md#friend-rendersystem)**  |

## Public Functions Documentation

### function Animation

```cpp
Animation() =default
```


### function Animation

```cpp
Animation(
    std::shared_ptr< MD5Anim > animation
)
```


### function UpdateAnimation

```cpp
void UpdateAnimation(
    const double delta
)
```

Updates the current animation based on a change in time. 

**Parameters**: 

  * **float** delta The change in time 


**Return**: void 

### function SetAnimationFile

```cpp
void SetAnimationFile(
    std::shared_ptr< MD5Anim > file
)
```

Sets the animation file for this animation. 

**Parameters**: 

  * **std::shared_ptr<resource::MD5Anim>** file The animation file. 


**Return**: void 

This does a run for the first set of animation from frame 0 to 1 with time 0. 


### function Out

```cpp
void Out(
    proto::Component * target
)
```


### function In

```cpp
void In(
    const proto::Component & source
)
```


## Friends

### friend RenderSystem

```cpp
friend class RenderSystem(
    RenderSystem 
);
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC