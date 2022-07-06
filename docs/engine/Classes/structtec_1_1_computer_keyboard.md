---
title: tec::ComputerKeyboard

---

# tec::ComputerKeyboard





Inherits from [tec::DeviceBase](/engine/Classes/structtec_1_1_device_base/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ComputerKeyboard](/engine/Classes/structtec_1_1_computer_keyboard/#function-computerkeyboard)**() |
| virtual | **[~ComputerKeyboard](/engine/Classes/structtec_1_1_computer_keyboard/#function-~computerkeyboard)**() =default |
| virtual void | **[In](/engine/Classes/structtec_1_1_computer_keyboard/#function-in)**(const proto::Computer::Device & source) |
| virtual void | **[Out](/engine/Classes/structtec_1_1_computer_keyboard/#function-out)**(proto::Computer::Device * target) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[has_focus](/engine/Classes/structtec_1_1_computer_keyboard/#variable-has-focus)**  |
| int | **[last_keycode](/engine/Classes/structtec_1_1_computer_keyboard/#variable-last-keycode)**  |
| int | **[modifiers_state](/engine/Classes/structtec_1_1_computer_keyboard/#variable-modifiers-state)**  |

## Additional inherited members

**Public Attributes inherited from [tec::DeviceBase](/engine/Classes/structtec_1_1_device_base/)**

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< Device > | **[device](/engine/Classes/structtec_1_1_device_base/#variable-device)**  |


## Public Functions Documentation

### function ComputerKeyboard

```cpp
ComputerKeyboard()
```


### function ~ComputerKeyboard

```cpp
virtual ~ComputerKeyboard() =default
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

### variable has_focus

```cpp
bool has_focus {false};
```


### variable last_keycode

```cpp
int last_keycode {0};
```


### variable modifiers_state

```cpp
int modifiers_state {0};
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000