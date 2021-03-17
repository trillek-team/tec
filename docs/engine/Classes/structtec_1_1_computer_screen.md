---
title: tec::ComputerScreen

---

# tec::ComputerScreen



Inherits from [tec::DeviceBase](Classes/structtec_1_1_device_base.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ComputerScreen](Classes/structtec_1_1_computer_screen.md#function-computerscreen)**() |
| virtual | **[~ComputerScreen](Classes/structtec_1_1_computer_screen.md#function-~computerscreen)**() =default |
| virtual void | **[In](Classes/structtec_1_1_computer_screen.md#function-in)**(const proto::Computer::Device & source) |
| virtual void | **[Out](Classes/structtec_1_1_computer_screen.md#function-out)**(proto::Computer::Device * target) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [TextureObject](Classes/classtec_1_1_texture_object.md) > | **[texture](Classes/structtec_1_1_computer_screen.md#variable-texture)**  |

## Additional inherited members

**Public Attributes inherited from [tec::DeviceBase](Classes/structtec_1_1_device_base.md)**

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< Device > | **[device](Classes/structtec_1_1_device_base.md#variable-device)**  |


## Public Functions Documentation

### function ComputerScreen

```cpp
ComputerScreen()
```


### function ~ComputerScreen

```cpp
virtual ~ComputerScreen() =default
```


### function In

```cpp
virtual void In(
    const proto::Computer::Device & source
)
```


**Reimplements**: [tec::DeviceBase::In](Classes/structtec_1_1_device_base.md#function-in)


### function Out

```cpp
virtual void Out(
    proto::Computer::Device * target
)
```


**Reimplements**: [tec::DeviceBase::Out](Classes/structtec_1_1_device_base.md#function-out)


## Public Attributes Documentation

### variable texture

```cpp
std::shared_ptr< TextureObject > texture;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC