---
title: tec::ComputerScreen

---

# tec::ComputerScreen





Inherits from [tec::DeviceBase](/engine/Classes/structtec_1_1_device_base/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ComputerScreen](/engine/Classes/structtec_1_1_computer_screen/#function-computerscreen)**() |
| virtual | **[~ComputerScreen](/engine/Classes/structtec_1_1_computer_screen/#function-~computerscreen)**() =default |
| virtual void | **[In](/engine/Classes/structtec_1_1_computer_screen/#function-in)**(const proto::Computer::Device & source) |
| virtual void | **[Out](/engine/Classes/structtec_1_1_computer_screen/#function-out)**(proto::Computer::Device * target) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< [TextureObject](/engine/Classes/classtec_1_1_texture_object/) > | **[texture](/engine/Classes/structtec_1_1_computer_screen/#variable-texture)**  |

## Additional inherited members

**Public Attributes inherited from [tec::DeviceBase](/engine/Classes/structtec_1_1_device_base/)**

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< Device > | **[device](/engine/Classes/structtec_1_1_device_base/#variable-device)**  |


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


**Reimplements**: [tec::DeviceBase::In](/engine/Classes/structtec_1_1_device_base/#function-in)


### function Out

```cpp
virtual void Out(
    proto::Computer::Device * target
)
```


**Reimplements**: [tec::DeviceBase::Out](/engine/Classes/structtec_1_1_device_base/#function-out)


## Public Attributes Documentation

### variable texture

```cpp
std::shared_ptr< TextureObject > texture;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000