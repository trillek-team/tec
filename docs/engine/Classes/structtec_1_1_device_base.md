---
title: tec::DeviceBase

---

# tec::DeviceBase



Inherited by [tec::ComputerKeyboard](Classes/structtec_1_1_computer_keyboard.md), [tec::ComputerScreen](Classes/structtec_1_1_computer_screen.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| virtual void | **[In](Classes/structtec_1_1_device_base.md#function-in)**(const proto::Computer::Device & source) =0 |
| virtual void | **[Out](Classes/structtec_1_1_device_base.md#function-out)**(proto::Computer::Device * target) =0 |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< Device > | **[device](Classes/structtec_1_1_device_base.md#variable-device)**  |

## Public Functions Documentation

### function In

```cpp
virtual void In(
    const proto::Computer::Device & source
) =0
```


**Reimplemented by**: [tec::ComputerScreen::In](Classes/structtec_1_1_computer_screen.md#function-in), [tec::ComputerKeyboard::In](Classes/structtec_1_1_computer_keyboard.md#function-in)


### function Out

```cpp
virtual void Out(
    proto::Computer::Device * target
) =0
```


**Reimplemented by**: [tec::ComputerScreen::Out](Classes/structtec_1_1_computer_screen.md#function-out), [tec::ComputerKeyboard::Out](Classes/structtec_1_1_computer_keyboard.md#function-out)


## Public Attributes Documentation

### variable device

```cpp
std::shared_ptr< Device > device;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC