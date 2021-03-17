---
title: tec::ComputerKeyboard

---

# tec::ComputerKeyboard



Inherits from [tec::DeviceBase](Classes/structtec_1_1_device_base.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ComputerKeyboard](Classes/structtec_1_1_computer_keyboard.md#function-computerkeyboard)**() |
| virtual | **[~ComputerKeyboard](Classes/structtec_1_1_computer_keyboard.md#function-~computerkeyboard)**() =default |
| virtual void | **[In](Classes/structtec_1_1_computer_keyboard.md#function-in)**(const proto::Computer::Device & source) |
| virtual void | **[Out](Classes/structtec_1_1_computer_keyboard.md#function-out)**(proto::Computer::Device * target) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[has_focus](Classes/structtec_1_1_computer_keyboard.md#variable-has_focus)**  |
| int | **[last_keycode](Classes/structtec_1_1_computer_keyboard.md#variable-last_keycode)**  |
| int | **[modifiers_state](Classes/structtec_1_1_computer_keyboard.md#variable-modifiers_state)**  |

## Additional inherited members

**Public Attributes inherited from [tec::DeviceBase](Classes/structtec_1_1_device_base.md)**

|                | Name           |
| -------------- | -------------- |
| std::shared_ptr< Device > | **[device](Classes/structtec_1_1_device_base.md#variable-device)**  |


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


**Reimplements**: [tec::DeviceBase::In](Classes/structtec_1_1_device_base.md#function-in)


### function Out

```cpp
virtual void Out(
    proto::Computer::Device * target
)
```


**Reimplements**: [tec::DeviceBase::Out](Classes/structtec_1_1_device_base.md#function-out)


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

Updated on 17 March 2021 at 23:08:11 UTC