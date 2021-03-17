---
title: tec::ConsoleSink

---

# tec::ConsoleSink



Inherits from spdlog::sinks::sink

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ConsoleSink](Classes/classtec_1_1_console_sink.md#function-consolesink)**([Console](Classes/classtec_1_1_console.md) & c) |
| void | **[log](Classes/classtec_1_1_console_sink.md#function-log)**(const spdlog::details::log_msg & msg) override |
| void | **[flush](Classes/classtec_1_1_console_sink.md#function-flush)**() override |
| void | **[set_pattern](Classes/classtec_1_1_console_sink.md#function-set_pattern)**(const std::string & pattern) |
| void | **[set_formatter](Classes/classtec_1_1_console_sink.md#function-set_formatter)**(std::unique_ptr< spdlog::formatter > sink_formatter) |

## Public Functions Documentation

### function ConsoleSink

```cpp
inline ConsoleSink(
    Console & c
)
```


### function log

```cpp
void log(
    const spdlog::details::log_msg & msg
) override
```


### function flush

```cpp
inline void flush() override
```


### function set_pattern

```cpp
inline void set_pattern(
    const std::string & pattern
)
```


### function set_formatter

```cpp
inline void set_formatter(
    std::unique_ptr< spdlog::formatter > sink_formatter
)
```


-------------------------------

Updated on 17 March 2021 at 23:08:11 UTC