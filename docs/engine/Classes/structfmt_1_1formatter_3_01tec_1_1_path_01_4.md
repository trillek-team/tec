---
title: fmt::formatter< tec::Path >
summary: fmtcore formatter for Paths for use with spdlog (or similar) Examples 

---

# fmt::formatter< tec::Path >



fmtcore formatter for Paths for use with spdlog (or similar) Examples  [More...](#detailed-description)


`#include <filesystem.hpp>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| constexpr auto | **[parse](/engine/Classes/structfmt_1_1formatter_3_01tec_1_1_path_01_4/#function-parse)**(format_parse_context & ctx) |
| template <typename FormatContext \> <br>auto | **[format](/engine/Classes/structfmt_1_1formatter_3_01tec_1_1_path_01_4/#function-format)**(const [tec::Path](/engine/Classes/classtec_1_1_path/) & p, FormatContext & ctx) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[filename](/engine/Classes/structfmt_1_1formatter_3_01tec_1_1_path_01_4/#variable-filename)**  |
| bool | **[fileex](/engine/Classes/structfmt_1_1formatter_3_01tec_1_1_path_01_4/#variable-fileex)**  |
| bool | **[relative](/engine/Classes/structfmt_1_1formatter_3_01tec_1_1_path_01_4/#variable-relative)**  |
| bool | **[quote](/engine/Classes/structfmt_1_1formatter_3_01tec_1_1_path_01_4/#variable-quote)**  |

## Detailed Description

```cpp
struct fmt::formatter< tec::Path >;
```

fmtcore formatter for Paths for use with spdlog (or similar) Examples 



```cpp
Path p{"/root/foo.bar"};
log->info("-> {}", p);    // -> /root/foo.bar
log->info("-> {:f}", p);  // -> foo.bar
log->info("-> {:fx}", p); // -> foo
log->info("-> {:x}", p);  // -> bar
log->info("-> {:r}", p);  // -> root/foo.bar
log->info("-> {:qr}", p); // -> "root/foo.bar"
```

## Public Functions Documentation

### function parse

```cpp
inline constexpr auto parse(
    format_parse_context & ctx
)
```


### function format

```cpp
template <typename FormatContext >
inline auto format(
    const tec::Path & p,
    FormatContext & ctx
)
```


## Public Attributes Documentation

### variable filename

```cpp
bool filename = false;
```


### variable fileex

```cpp
bool fileex = false;
```


### variable relative

```cpp
bool relative = false;
```


### variable quote

```cpp
bool quote = false;
```


-------------------------------

Updated on 2022-07-06 at 04:15:07 +0000