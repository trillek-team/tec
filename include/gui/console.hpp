#pragma once

#include <deque>
#include <map>
#include <tuple>

#include <imgui.h>

#include "os.hpp"
#include "event-system.hpp"
#include "spdlog/sinks/sink.h"

namespace tec {
	class Console :
		public EventQueue <KeyboardEvent>,
		public EventQueue <WindowResizedEvent> {
	public:
		Console();

		void Update(double delta);

		/**
		 * Clears console output
		 */
		void Clear();

		void Println(const std::string& str);
		void Println(const char* cstr);
		
		void Printfln(const char* cstr, ...) IM_PRINTFARGS(2);

		void Draw();

		void SetAlpha(float a) {
			a = (a < 0)? 0 : a; // Clamp [0, 1.0]
			a = (a > 1.0f)? 1.0f : a;
			this->alpha = a;
		}
		
		float GetAlpha() const { return alpha;}
		
		/**
		 * Register a new command
		 * @param name Name of the commad (case sensitive!)
		 * @param help String with help text
		 * @param func Function to be executed when the command is called
		 */
		void AddConsoleCommand(std::string name, std::string help, std::function<void(const char*)> && func);
		

	private:
		// TODO Store a deque<tuple<color, text> instead of raw text ?
		std::deque<std::string> buf;
		bool scrollToBottom = false;
		char inputBuf[256];

		bool show = true;
		bool resize = true;
		float alpha = 0.85f;
		const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar 
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
