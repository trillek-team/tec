#pragma once

#include <imgui.h>

#include "os.hpp"
#include "event-system.hpp"
#include "spdlog/sinks/sink.h"

namespace tec {
	class Console :
		public spdlog::sinks::sink,
		public EventQueue <KeyboardEvent>,
		public EventQueue <WindowResizedEvent> {
	public:
		Console();

		void Update(double delta);

		void Clear();

		void Printf(const char* fmt, ...) IM_PRINTFARGS(2);
		void Printfln(const char* fmt, ...) IM_PRINTFARGS(2);

		void Draw();

		// SPDLog sink interface
		void log(const spdlog::details::log_msg& msg) override;
		void flush() {};

	private:
		// TODO Store a ImVector<tuple<level, text> instead of raw text ?
		// So we can put color
		// or (perhaps better), preparse before call Text() searching [RRGGBB] to change of color of the text
		ImGuiTextBuffer buf;
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
}
