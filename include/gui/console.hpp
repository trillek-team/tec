#pragma once

#include <imgui.h>

#include "os.hpp"
#include "event-system.hpp"

namespace tec {
	class Console :
		public EventQueue <KeyboardEvent>,
		public EventQueue <WindowResizedEvent> {
	public:
		Console();

		void Update(double delta);

		void Clear();

		void AddLog(const char* fmt, ...) IM_PRINTFARGS(2);

		void Draw();

	private:
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
