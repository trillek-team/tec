#pragma once

#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <tuple>

#include <imgui.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/sink.h>

#include "abs_window.hpp"
#include "event-system.hpp"
#include "events.hpp"
#include "os.hpp"
#include "ring-buffer.hpp"

namespace tec {
class Console : public AbstractWindow, public EventQueue<KeyboardEvent>, public EventQueue<WindowResizedEvent> {
public:
	Console();

	void Update(double delta) override;

	/**
	* Clears console output
	*/
	void Clear();

	void Println(const std::string& str, ImVec4 color = ImVec4(255, 255, 255, 255));
	void Println(const char* cstr, ImVec4 color = ImVec4(255, 255, 255, 255));

	void Printfln(const char* cstr, ...) IM_FMTARGS(2);

	void Draw() override;

	bool isCollapsed() const override { return false; }

	/**
	* Register a new command
	* @param name Name of the command (case sensitive!)
	* @param help String with help text
	* @param func Function to be executed when the command is called
	*/
	void AddConsoleCommand(std::string name, std::string help, std::function<void(const char*)>&& func);

	/**
	* Register a slash handler that handles all '/' commands
	* @param func Function to be executed when the command is called
	*/
	void AddSlashHandler(std::function<void(const char*)>&& func);

private:
	std::shared_ptr<tec::RingBuffer<std::tuple<ImVec4, std::string>, 4096>> buf;
	std::mutex input_mutex; /// Mutex to serialize write to Console buffer
	bool scrollToBottom{false};
	char inputBuf[256]{};

	bool resize = true;
	static const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
												 | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
												 | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	std::map<std::string, std::tuple<std::function<void(const char*)>, std::string>>
			commands; /// Storage of commands and help info

	using EventQueue<WindowResizedEvent>::On;
	using EventQueue<KeyboardEvent>::On;
	void On(std::shared_ptr<WindowResizedEvent> data) override;
	void On(std::shared_ptr<KeyboardEvent> data) override;

	std::function<void(const char*)> slash_handler;
};

class ConsoleSink : public spdlog::sinks::sink {
public:
	ConsoleSink(Console& c) : console(c){};

	// SPDLog sink interface
	void log(const spdlog::details::log_msg& msg) override;

	void flush() override {}

	void set_pattern(const std::string& pattern) final {
		std::lock_guard<std::mutex> lg(mutex_);
		set_pattern_(pattern);
	}

	void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) final {
		std::lock_guard<std::mutex> lg(mutex_);
		set_formatter_(std::move(sink_formatter));
	}

private:
	Console& console;
	virtual void set_pattern_(const std::string& pattern) {
		set_formatter_(std::make_unique<spdlog::pattern_formatter>(pattern));
	}

	virtual void set_formatter_(std::unique_ptr<spdlog::formatter> sink_formatter) {
		formatter_ = std::move(sink_formatter);
	}
	std::mutex mutex_;
	std::unique_ptr<spdlog::formatter> formatter_;
};

} // namespace tec
