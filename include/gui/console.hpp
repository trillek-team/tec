#pragma once

#include <deque>
#include <map>
#include <tuple>
#include <mutex>

#include <imgui.h>
#include <spdlog/sinks/sink.h>

#include "ring_buffer.hpp"
#include "os.hpp"
#include "events.hpp"
#include "event-system.hpp"
#include "gui/abs_window.hpp"

namespace tec {
	class Console :
		public AbstractWindow,
		public EventQueue <KeyboardEvent>,
		public EventQueue <WindowResizedEvent> {
	public:
		Console();

		void Update(double delta);

		/**
		 * Clears console output
		 */
		void Clear();

		void Println(const std::string& str, ImVec4 color = ImVec4(255, 255, 255, 255) );
		void Println(const char* cstr, ImVec4 color = ImVec4(255, 255, 255, 255) );
		
		void Printfln(const char* cstr, ...) IM_PRINTFARGS(2);

		void Draw();
		
		bool isCollapsed() const {
			return false;
		}
		
		/**
		 * Register a new command
		 * @param name Name of the commad (case sensitive!)
		 * @param help String with help text
		 * @param func Function to be executed when the command is called
		 */
		void AddConsoleCommand(std::string name, std::string help, std::function<void(const char*)> && func);
		

	private:
		// TODO Store a RingBuffer<tuple<color, text> instead of raw text ?
		tec::RingBuffer< std::tuple< ImVec4, std::string >, 4096> buf;
		std::mutex input_mutex; /// Mutex to serialize write to Console buffer
		bool scrollToBottom = false;
		char inputBuf[256];

		bool resize = true;
		static const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar 
			| ImGuiWindowFlags_NoResize 
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoCollapse;

		std::map<std::string, std::tuple<std::function<void(const char*)>, std::string>> commands; /// Storage of commands and help info
			
		void On(std::shared_ptr<WindowResizedEvent> data);

		void On(std::shared_ptr<KeyboardEvent> data);
	};

	class ConsoleSink : 
		public spdlog::sinks::sink {
		public:
			ConsoleSink(Console& c) : console(c) {};
			
			// SPDLog sink interface
			void log(const spdlog::details::log_msg& msg) override;
			
			void flush() {};
		private:
			Console& console;
	};
	
}
