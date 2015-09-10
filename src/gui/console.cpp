#include "gui/console.hpp"

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

	void Console::AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
	{
		va_list args;
		va_start(args, fmt);
		buf.appendv(fmt, args);
		va_end(args);
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
			
			ImGui::TextUnformatted(buf.begin());
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

}
