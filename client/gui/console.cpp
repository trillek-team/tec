#include "console.hpp"

#include <string>
#include <cstdio>
#include <GLFW/glfw3.h>

namespace tec {
	Console::Console() {
		inputBuf[0] = '\0';
		
		// Default embed commands
		this->AddConsoleCommand( "cmdlist",
			"cmdlist : List all commands",
		[this ] (const char* args) {
			for (auto command : this->commands) {
				this->Println(command.first);
			}
		});
		
		this->AddConsoleCommand( "help",
			"help [command] : Prints a short help about an command",
		[this ] (const char* args) {
			const char* end_arg = args;
			while (*end_arg != '\0' && *end_arg != ' ') {
				end_arg++;
			}
			// Args now points were the arguments begins
			std::string command(args, end_arg - args);
			auto search = commands.find(command);
			if(search != commands.end()) {
				this->Println(std::get<1>(search->second));
			} else {
				this->Println("Unknown command. Please use cmdlist to list all commands.");
			}
		});
		
		this->AddConsoleCommand( "clear",
			"clear : Clear console output",
		[this ] (const char* args) {
			this->Clear();
		});
		
		this->AddConsoleCommand( "echo",
			"echo [message] : Prints a message to the console",
		[this ] (const char* args) {
			this->Println(args);
		});
		
	}

	void Console::Update(double delta) {
		EventQueue<WindowResizedEvent>::ProcessEventQueue();
		EventQueue<KeyboardEvent>::ProcessEventQueue();
	}

	void Console::Clear() {
		buf.clear();
	}

	void Console::Println(const std::string& str, ImVec4 color) {
		{
			std::lock_guard<std::mutex> lock(input_mutex);
			if (buf.full()) {
				buf.pop_back();
			}
			buf.push_front(std::make_tuple(color, str));
		}
		scrollToBottom = true;
	}

	void Console::Println(const char* cstr, ImVec4 color) {
		{
			std::lock_guard<std::mutex> lock(input_mutex);
			if (buf.full()) {
				buf.pop_back();
			}
			buf.push_front(std::make_tuple(color, std::string(cstr)));
		}
		scrollToBottom = true;
	}

	void Console::Printfln(const char* fmt, ... ) {
		char tmp[1024];
		va_list args;
		va_start(args, fmt);
		std::vsnprintf(tmp, 1024, fmt, args);
		tmp[1023] = 0;
		va_end(args);

		{
			std::lock_guard<std::mutex> lock(input_mutex);
			if (buf.full()) {
				buf.pop_back();
			}
			buf.push_front(std::make_tuple(ImVec4(255, 255, 255, 255), std::string(tmp)));
		}
		scrollToBottom = true;
	}
	
	void Console::Draw() {
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

			for (std::size_t i = 0; i < buf.size(); i++) {
				ImGui::PushStyleColor(ImGuiCol_Text, std::get<0>(buf[i]) );
				ImGui::TextWrapped("%s", std::get<1>(buf[i]).c_str() );
				ImGui::PopStyleColor();
				/*
				ImVec2 pos = ImGui::GetCursorScreenPos();
				ImVec2 maxrect = ImGui::GetWindowContentRegionMax();
				ImVec2 textrect = ImGui::CalcTextSize(it->c_str());
				ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowFont(), 
					ImGui::GetWindowFontSize(),
					pos, 
					ImColor(ImGui::GetStyle().Colors[ImGuiCol_Text]), 
					it->c_str(), 0,
					maxrect.x, 
					0);
				pos.y += textrect.y;
				ImGui::SetCursorScreenPos(pos);
				*/
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
				if (inputBuf[0] != '\0') { // Processing input
					const char* args = inputBuf;
					std::size_t cmd_len = 0;
					while (*args != '\0' && *args != ' ') {
						args++;
						cmd_len++;
					}
					if (*args != '\0') {
						args++;
					}
					// Args now points were the arguments begins
					std::string command(inputBuf, cmd_len);
					auto search = commands.find(command);
					if(search != commands.end()) {
						Printfln("]%s", command.c_str());
						std::get<0>(search->second)(args);
					} else {
						this->Println("Unknown command");
					}
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
			&& data->key == GLFW_KEY_ESCAPE) { // Toggles console
			show = !show;
		}
	}

	void Console::AddConsoleCommand(std::string name, std::string help, std::function<void(const char*)> && func) {
		auto tmp = std::make_tuple(std::move(func), help);
		this->commands[name] = tmp;
	}


	void ConsoleSink::log(const spdlog::details::log_msg& msg) {
		std::string str(msg.raw.str());
		ImVec4 color(255, 255, 255, 255);
		switch (msg.level) {
			case spdlog::level::trace :
				str = "trace " + str;
				break;
			case spdlog::level::debug :
				str = "debug " + str;
				break;
			case spdlog::level::warn :
				str = "WARNING : " + str;
				color = ImVec4(255, 48, 0, 255);
				break;
			case spdlog::level::err :
				str = "ERROR! " + str;
				color = ImVec4(255, 0, 0, 255);
				break;
			case spdlog::level::critical :
				str = "CRITICAL ERROR! " + str;
				color = ImVec4(255, 0, 0, 255);
				break;
			case spdlog::level::alert :
				str = "ALERT! " + str;
				color = ImVec4(255, 0, 0, 255);
				break;
			case spdlog::level::emerg :
				str = "BLUE SCREEN OF DEATH! " + str;
				color = ImVec4(255, 0, 0, 255);
				break;
			case spdlog::level::info :
			case spdlog::level::notice :
			default:
				;
		}
		console.Println(str, color);
	}

}
