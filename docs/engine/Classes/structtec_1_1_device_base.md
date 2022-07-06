---
title: tec::DeviceBase

---

# tec::DeviceBase





Inherited by [tec::ComputerKeyboard](/engine/Classes/structtec_1_1_computer_keyboard/), [tec::ComputerScreen](/engine/Classes/structtec_1_1_computer_screen/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| virtual void | **[In](/engine/Classes/structtec_1_1_device_base/#function-in)**(const proto::Computer::Device & source) =0 |
| virtual void | **[Out](/engine/Classes/structtec_1_1_device_base/#function-out)**(proto::Computer::Device * target) =0 |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< Device > | **[device](/engine/Classes/structtec_1_1_device_base/#variable-device)**  |

## Public Functions Documentation

### function In

```cpp
virtual void In(
    const proto::Computer::Device & source
) =0
```


**Reimplemented by**: [tec::ComputerScreen::In](/engine/Classes/structtec_1_1_computer_screen/#function-in), [tec::ComputerKeyboard::In](/engine/Classes/structtec_1_1_computer_keyboard/#function-in)


### function Out

```cpp
virtual void Out(
    proto::Computer::Device * target
) =0
```


**Reimplemented by**: [tec::ComputerScreen::Out](/engine/Classes/structtec_1_1_computer_screen/#function-out), [tec::ComputerKeyboard::Out](/engine/Classes/structtec_1_1_computer_keyboard/#function-out)


## Public Attributes Documentation

### variable device

```cpp
std::shared_ptr< Device > device;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000