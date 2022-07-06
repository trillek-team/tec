---
title: tec::Computer

---

# tec::Computer





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Computer](/engine/Classes/structtec_1_1_computer/#function-computer)**() |
| | **[~Computer](/engine/Classes/structtec_1_1_computer/#function-~computer)**() |
| void | **[In](/engine/Classes/structtec_1_1_computer/#function-in)**(const proto::Component & source) |
| void | **[Out](/engine/Classes/structtec_1_1_computer/#function-out)**(proto::Component * target) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::uint8_t * | **[rom](/engine/Classes/structtec_1_1_computer/#variable-rom)**  |
| std::string | **[rom_name](/engine/Classes/structtec_1_1_computer/#variable-rom-name)**  |
| std::size_t | **[rom_size](/engine/Classes/structtec_1_1_computer/#variable-rom-size)**  |
| VComputer | **[vc](/engine/Classes/structtec_1_1_computer/#variable-vc)**  |
| std::map< int, std::shared_ptr< [DeviceBase](/engine/Classes/structtec_1_1_device_base/) > > | **[devices](/engine/Classes/structtec_1_1_computer/#variable-devices)**  |

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

Updated on 2022-07-06 at 04:15:07 +0000