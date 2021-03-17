---
title: tec::Computer

---

# tec::Computer



## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Computer](Classes/structtec_1_1_computer.md#function-computer)**() |
| | **[~Computer](Classes/structtec_1_1_computer.md#function-~computer)**() |
| void | **[In](Classes/structtec_1_1_computer.md#function-in)**(const proto::Component & source) |
| void | **[Out](Classes/structtec_1_1_computer.md#function-out)**(proto::Component * target) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::uint8_t * | **[rom](Classes/structtec_1_1_computer.md#variable-rom)**  |
| std::string | **[rom_name](Classes/structtec_1_1_computer.md#variable-rom_name)**  |
| std::size_t | **[rom_size](Classes/structtec_1_1_computer.md#variable-rom_size)**  |
| VComputer | **[vc](Classes/structtec_1_1_computer.md#variable-vc)**  |
| std::map< int, std::shared_ptr< [DeviceBase](Classes/structtec_1_1_device_base.md) > > | **[devices](Classes/structtec_1_1_computer.md#variable-devices)**  |

## Public Functions Documentation

### function Computer

```cpp
Computer()
```


### function ~Computer

```cpp
inline ~Computer()
```


### function In

```cpp
void In(
    const proto::Component & source
)
```


### function Out

```cpp
void Out(
    proto::Component * target
)
```


## Public Attributes Documentation

### variable rom

```cpp
std::uint8_t * rom;
```


### variable rom_name

```cpp
std::string rom_name;
```


### variable rom_size

```cpp
std::size_t rom_size;
```


### variable vc

```cpp
VComputer vc;
```


### variable devices

```cpp
std::map< int, std::shared_ptr< DeviceBase > > devices;
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC