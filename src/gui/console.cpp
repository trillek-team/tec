#include "gui/console.hpp"

#include <string>
#include <cstdio>
#include <GLFW/glfw3.h>

namespace tec {
	Console::Console() {
		inputBuf[0] = '\0';
	}

	void Console::Update(double delta) {
		EventQueue<WindowResizedEvent>::ProcessEventQueue();
		EventQueue<KeyboardEvent>::ProcessEventQueue();
	}

	void Console::Clear() {
		buf.clear();
	}

	void Console::Print(const std::string& str)
	{
		buf.push_front(str);
		
		scrollToBottom = true;
	}

	void Console::Println(const std::string& str)
	{
		std::string tmp( str);
		tmp += "\n";
		buf.push_front(tmp);
		
		scrollToBottom = true;
	}

	void Console::Draw()
	{
		if (show) {
			const auto root = ImGui::GetIO().DisplaySize;
			float height = root.y * 0.25f;
			float hpos = root.y - height;

			if (resize) {
				ImGui::SetNextWindowPos(ImVec2(0, hpos));
				ImGui::SetNextWindowSize(ImVec2(root.x, height));
				resize = false;
			}
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

			ImGui::Begin("Console", nullptr, window_flags);
			ImGui::BeginChild("ScrollingRegion",
				ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()),
				false,
				ImGuiWindowFlags_NoScrollbar);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
			
			for (auto it = buf.crbegin(); it != buf.crend(); it++) {
				ImGui::TextUnformatted((*it).c_str());
			}
			if (scrollToBottom) {
				ImGui::SetScrollHere(1.0f);
				scrollToBottom = false;
			}
			
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();

			// Command-line
			if (ImGui::InputText("Input", inputBuf, 
				(int)(sizeof(inputBuf) / sizeof(*inputBuf)),
				ImGuiInputTextFlags_EnterReturnsTrue
				// | ImGuiInputTextFlags_CallbackCompletion
				// | ImGuiInputTextFlags_CallbackHistory, 
				//nullptr, //&TextEditCallbackStub, 
				//(void*)this
				))
			{
				char* input_end = inputBuf + strlen(inputBuf);
				// Trim string
				while (input_end > inputBuf && input_end[-1] == ' ') {
					input_end--;
				}
				*input_end = 0;
				if (inputBuf[0]) {
					// TODO Process here input
				}
				inputBuf[0] = '\0';
			}
			// Demonstrate keeping auto focus on the input box
			if (ImGui::IsItemHovered()
				|| (ImGui::IsRootWindowOrAnyChildFocused()
					&& !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))) {
				ImGui::SetKeyboardFocusHere(-1); // Auto focus
			}

			ImGui::End();
			ImGui::PopStyleVar();
		}
	}

	void Console::On(std::shared_ptr<WindowResizedEvent> data) {
		resize = true;
	}

	void Console::On(std::shared_ptr<KeyboardEvent> data) {
		if (data->action == KeyboardEvent::KEY_DOWN 
			&& data->key == GLFW_KEY_ESCAPE) { // Togles console
			show = !show;
		}
	}

	void ConsoleSink::log(const spdlog::details::log_msg& msg) {
		switch (msg.level) {
			case spdlog::level::emerg :
			case spdlog::level::alert :
			case spdlog::level::critical :
			case spdlog::level::err :
			case spdlog::level::warn :
			case spdlog::level::notice :
			case spdlog::level::info :
			case spdlog::level::debug :
			case spdlog::level::trace :
				defaut:
				;
		}
		console.Println(msg.raw.str()); 
	}
	
}
