#pragma once

#include <deque>

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

		void Clear();

		void Print(const std::string& str);
		void Println(const std::string& str);

		void Draw();


	private:
		// TODO Store a deque<tuple<color, text> instead of raw text ?
		std::deque<std::string> buf;
		bool scrollToBottom = false;
		bool show = true;
		bool resize = true;
		float alpha = 0.75f;
		const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar 
			| ImGuiWindowFlags_NoResize 
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoSavedSettings 
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoCollapse;

		char inputBuf[256];

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
