---
title: tec::Animation

---

# tec::Animation





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Animation](/engine/Classes/classtec_1_1_animation/#function-animation)**() =default |
| | **[Animation](/engine/Classes/classtec_1_1_animation/#function-animation)**(std::shared_ptr< [MD5Anim](/engine/Classes/classtec_1_1_m_d5_anim/) > animation) |
| void | **[UpdateAnimation](/engine/Classes/classtec_1_1_animation/#function-updateanimation)**(const double delta)<br>Updates the current animation based on a change in time.  |
| void | **[SetAnimationFile](/engine/Classes/classtec_1_1_animation/#function-setanimationfile)**(std::shared_ptr< [MD5Anim](/engine/Classes/classtec_1_1_m_d5_anim/) > file)<br>Sets the animation file for this animation.  |
| void | **[Play](/engine/Classes/classtec_1_1_animation/#function-play)**(bool from_start) |
| void | **[Stop](/engine/Classes/classtec_1_1_animation/#function-stop)**() |
| void | **[SetLoop](/engine/Classes/classtec_1_1_animation/#function-setloop)**(bool loop) |
| void | **[Out](/engine/Classes/classtec_1_1_animation/#function-out)**(proto::Component * target) |
| void | **[In](/engine/Classes/classtec_1_1_animation/#function-in)**(const proto::Component & source) |

## Friends

|                | Name           |
| -------------- | -------------- |
| class | **[RenderSystem](/engine/Classes/classtec_1_1_animation/#friend-rendersystem)**  |

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


### function Play

```cpp
inline void Play(
    bool from_start
)
```


### function Stop

```cpp
inline void Stop()
```


### function SetLoop

```cpp
inline void SetLoop(
    bool loop
)
```


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

Updated on 2022-07-06 at 04:15:07 +0000