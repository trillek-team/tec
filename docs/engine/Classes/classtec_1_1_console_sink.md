---
title: tec::ConsoleSink

---

# tec::ConsoleSink



Inherits from spdlog::sinks::sink

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ConsoleSink](/engine/Classes/classtec_1_1_console_sink/#function-consolesink)**([Console](/engine/Classes/classtec_1_1_console/) & c) |
| void | **[log](/engine/Classes/classtec_1_1_console_sink/#function-log)**(const spdlog::details::log_msg & msg) override |
| void | **[flush](/engine/Classes/classtec_1_1_console_sink/#function-flush)**() override |
| void | **[set_pattern](/engine/Classes/classtec_1_1_console_sink/#function-set_pattern)**(const std::string & pattern) |
| void | **[set_formatter](/engine/Classes/classtec_1_1_console_sink/#function-set_formatter)**(std::unique_ptr< spdlog::formatter > sink_formatter) |

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

Updated on 21 March 2021 at 16:46:06 UTC