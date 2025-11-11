#include "imgui-system.hpp"

#include <spdlog/spdlog.h>

#ifdef _MSC_VER
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif

#include "events.hpp"
#include "filesystem.hpp"
#include "os.hpp"

namespace tec {
GLFWwindow* IMGUISystem::window = nullptr;
int IMGUISystem::shader_program = 0, IMGUISystem::vertex_shader = 0, IMGUISystem::fragment_shader = 0;
int IMGUISystem::texture_attribute_location = 0, IMGUISystem::projmtx_attribute_location = 0;
int IMGUISystem::position_attribute_location = 0, IMGUISystem::uv_attribute_location = 0,
	IMGUISystem::color_attribute_location = 0;
std::size_t IMGUISystem::vbo_size = 0, IMGUISystem::ibo_size = 0;
unsigned int IMGUISystem::vbo = 0, IMGUISystem::ibo = 0, IMGUISystem::vao = 0;
GLuint IMGUISystem::font_texture = 0;

IMGUISystem::IMGUISystem() { ImGui::CreateContext(); }

void IMGUISystem::Initialize(GLFWwindow* _window) {
	IMGUISystem::window = _window;
	auto& io = ImGui::GetIO();
	const std::string ini_filename = (Path::GetUserSettingsPath() / "imgui.ini").toString();
	const std::string log_filename = (Path::GetUserCachePath() / "imgui_log.txt").toString();
	io.IniFilename = ini_filename.c_str();
#if defined(DEBUG) || defined(_DEBUG)
	io.LogFilename = log_filename.c_str();
#endif

	io.SetClipboardTextFn = SetClipboardText;
	io.GetClipboardTextFn = GetClipboardText;

	this->UpdateDisplaySize();

	if (!IMGUISystem::font_texture) {
		CreateDeviceObjects();
	}
}

IMGUISystem::~IMGUISystem() {
	if (vao) {
		glDeleteVertexArrays(1, &vao);
	}
	if (vbo) {
		glDeleteBuffers(1, &vbo);
	}
	vao = 0;
	vbo = 0;

	glDetachShader(shader_program, vertex_shader);
	glDeleteShader(vertex_shader);
	vertex_shader = 0;

	glDetachShader(shader_program, fragment_shader);
	glDeleteShader(fragment_shader);
	fragment_shader = 0;

	glDeleteProgram(shader_program);
	shader_program = 0;

	if (font_texture) {
		glDeleteTextures(1, &font_texture);
		ImGui::GetIO().Fonts->TexID = 0;
		font_texture = 0;
	}
	ImGui::DestroyContext();
}

void IMGUISystem::CreateGUI() {
	this->AddWindowDrawFunction("main_menu", [this]() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Connect")) {
				bool visible = this->IsWindowVisible("connect_window");
				if (ImGui::MenuItem("Connect to server...", "", visible)) {
					if (visible) {
						this->HideWindow("connect_window");
					}
					else {
						this->ShowWindow("connect_window");
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(("Debug"))) {
				bool visible = this->IsWindowVisible("debug_info");
				if (ImGui::MenuItem("Debug info")) {
					if (visible)
						this->HideWindow("debug_info");
					else
						this->ShowWindow("debug_info");
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	});

	this->ShowWindow("main_menu");
}

void IMGUISystem::Update(double delta) {
	ProcessCommandQueue();
	auto& io = ImGui::GetIO();
	io.DeltaTime = static_cast<float>(delta);
	EventQueue<WindowResizedEvent>::ProcessEventQueue();
	EventQueue<MouseMoveEvent>::ProcessEventQueue();
	EventQueue<MouseScrollEvent>::ProcessEventQueue();
	EventQueue<MouseBtnEvent>::ProcessEventQueue();
	EventQueue<KeyboardEvent>::ProcessEventQueue();

	// Setup inputs
	// (we already got mouse wheel, keyboard keys & characters from event system
	if (IMGUISystem::window == OS::GetFocusedWindow()) {
		// Convert mouse coordinates to pixels
		this->mouse_pos.x *= (float)this->framebuffer_width / (float)this->window_width;
		this->mouse_pos.y *= (float)this->framebuffer_height / (float)this->window_height;

		// Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
		io.MousePos = ImVec2(mouse_pos.x, mouse_pos.y);

		io.MouseWheel += this->mouse_wheel.y; // ImGUI not support x axis scroll :(
		this->mouse_wheel.y = 0;
		this->mouse_wheel.x = 0;
	}
	else {
		io.MousePos = ImVec2(-1, -1);
	}

	for (int i = 0; i < 3; i++) {
		// If a mouse press event came, always pass it as "mouse held this frame", so
		// we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[i] = mouse_pressed[i];
	}

	// Start the frame
	ImGui::NewFrame();

	for (auto window_name : this->visible_windows) {
		if (this->window_draw_funcs.find(window_name) != this->window_draw_funcs.end()) {
			this->window_draw_funcs.at(window_name)();
		}
	}
	ImGui::Render();

	for (int i = 0; i < 3; i++) {
		if (mouse_released[i]) {
			mouse_pressed[i] = false;
		}
		mouse_released[i] = false;
	}
	this->RenderDrawLists(ImGui::GetDrawData());
}

void IMGUISystem::CreateDeviceObjects() {
	// clang-format off
	const GLchar* _vertex_shader =
		"#version 330\n"
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 UV;\n"
		"in vec4 Color;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"	Frag_UV = UV;\n"
		"	Frag_Color = Color;\n"
		"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const GLchar* _fragment_shader =
		"#version 330\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
		"}\n";
	// clang-format on
	IMGUISystem::shader_program = glCreateProgram();
	IMGUISystem::vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	IMGUISystem::fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(IMGUISystem::vertex_shader, 1, &_vertex_shader, nullptr);
	glShaderSource(IMGUISystem::fragment_shader, 1, &_fragment_shader, nullptr);
	GLint success = 0;
	glCompileShader(IMGUISystem::vertex_shader);
	glGetShaderiv(IMGUISystem::vertex_shader, GL_COMPILE_STATUS, &success);
	glCompileShader(IMGUISystem::fragment_shader);
	glGetShaderiv(IMGUISystem::fragment_shader, GL_COMPILE_STATUS, &success);
	glAttachShader(IMGUISystem::shader_program, IMGUISystem::vertex_shader);
	glAttachShader(IMGUISystem::shader_program, IMGUISystem::fragment_shader);
	glLinkProgram(IMGUISystem::shader_program);
	GLint isLinked = 0;
	glGetProgramiv(IMGUISystem::shader_program, GL_LINK_STATUS, &isLinked);

	IMGUISystem::texture_attribute_location = glGetUniformLocation(IMGUISystem::shader_program, "Texture");
	IMGUISystem::projmtx_attribute_location = glGetUniformLocation(IMGUISystem::shader_program, "ProjMtx");
	IMGUISystem::position_attribute_location = glGetAttribLocation(IMGUISystem::shader_program, "Position");
	IMGUISystem::uv_attribute_location = glGetAttribLocation(IMGUISystem::shader_program, "UV");
	IMGUISystem::color_attribute_location = glGetAttribLocation(IMGUISystem::shader_program, "Color");

	glGenBuffers(1, &IMGUISystem::vbo);
	glGenBuffers(1, &IMGUISystem::ibo);

	glGenVertexArrays(1, &IMGUISystem::vao);
	glBindVertexArray(IMGUISystem::vao);
	glBindBuffer(GL_ARRAY_BUFFER, IMGUISystem::vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IMGUISystem::ibo);
	glEnableVertexAttribArray(IMGUISystem::position_attribute_location);
	glEnableVertexAttribArray(IMGUISystem::uv_attribute_location);
	glEnableVertexAttribArray(IMGUISystem::color_attribute_location);

#define OFFSETOF(TYPE, ELEMENT) ((std::size_t) & (((TYPE*)0)->ELEMENT))
	glVertexAttribPointer(
			IMGUISystem::position_attribute_location,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(ImDrawVert),
			(GLvoid*)OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(
			IMGUISystem::uv_attribute_location,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(ImDrawVert),
			(GLvoid*)OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(
			IMGUISystem::color_attribute_location,
			4,
			GL_UNSIGNED_BYTE,
			GL_TRUE,
			sizeof(ImDrawVert),
			(GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Create the font texture.
	unsigned char* pixels;
	int width, height;
	const auto& io = ImGui::GetIO();
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height); // Load as RGBA 32-bits for OpenGL3 demo because it is more
			// likely to be compatible with user's existing shader.

	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

	glGenTextures(1, &font_texture);
	glBindTexture(GL_TEXTURE_2D, font_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->SetTexID((intptr_t)font_texture);

	// Cleanup (don't clear the input data if you want to append new fonts later)
	io.Fonts->ClearInputData();
	io.Fonts->ClearTexData();

	glBindTexture(GL_TEXTURE_2D, last_texture);
}

const char* IMGUISystem::GetClipboardText(void*) { return glfwGetClipboardString(IMGUISystem::window); }

void IMGUISystem::SetClipboardText(void*, const char* text) { glfwSetClipboardString(IMGUISystem::window, text); }

void IMGUISystem::RenderDrawLists(ImDrawData* draw_data) {
	if (draw_data->CmdListsCount == 0) {
		return;
	}

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	// Setup orthographic projection matrix
	const auto display_size{ImGui::GetIO().DisplaySize};
	const float width = display_size.x;
	const float height = ImGui::GetIO().DisplaySize.y;
	const float ortho_projection[4][4] = {
			{2.0f / width, 0.0f, 0.0f, 0.0f},
			{0.0f, 2.0f / -height, 0.0f, 0.0f},
			{0.0f, 0.0f, -1.0f, 0.0f},
			{-1.0f, 1.0f, 0.0f, 1.0f},
	};
	glUseProgram(shader_program);
	glUniform1i(texture_attribute_location, 0);
	glUniformMatrix4fv(projmtx_attribute_location, 1, GL_FALSE, &ortho_projection[0][0]);
	glBindVertexArray(vao);
	for (int n = 0; n < draw_data->CmdListsCount; n++) {
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		std::size_t needed_vtx_size = cmd_list->VtxBuffer.size() * sizeof(ImDrawVert);
		if (vbo_size < needed_vtx_size) {
			// Grow our buffer if needed
			vbo_size = needed_vtx_size + 2000 * sizeof(ImDrawVert);
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vbo_size, NULL, GL_STREAM_DRAW);
		}

		unsigned char* vtx_data = (unsigned char*)glMapBufferRange(
				GL_ARRAY_BUFFER, 0, needed_vtx_size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		if (!vtx_data) {
			continue;
		}
		memcpy(vtx_data, &cmd_list->VtxBuffer[0], cmd_list->VtxBuffer.size() * sizeof(ImDrawVert));
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		std::size_t needed_idx_size = cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx);
		if (ibo_size < needed_idx_size) {
			// Grow our buffer if needed
			ibo_size = needed_idx_size + 500 * sizeof(ImDrawIdx);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)ibo_size, NULL, GL_STREAM_DRAW);
		}

		unsigned char* idx_data = (unsigned char*)glMapBufferRange(
				GL_ELEMENT_ARRAY_BUFFER, 0, needed_idx_size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		if (!idx_data) {
			continue;
		}
		memcpy(idx_data, &cmd_list->IdxBuffer[0], cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx));
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++) {
			if (pcmd->UserCallback) {
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor(
						(int)pcmd->ClipRect.x,
						(int)(height - pcmd->ClipRect.w),
						(int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
						(int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	// Restore modified state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void IMGUISystem::AddWindowDrawFunction(std::string name, std::function<void()>&& func) {
	this->window_draw_funcs[name] = std::move(func);
}

void IMGUISystem::UpdateDisplaySize() {
	glfwGetWindowSize(IMGUISystem::window, &this->window_width, &this->window_height);
	glfwGetFramebufferSize(IMGUISystem::window, &this->framebuffer_width, &this->framebuffer_height);
	ImGui::GetIO().DisplaySize = ImVec2((float)this->framebuffer_width, (float)this->framebuffer_height);
}

void IMGUISystem::On(eid, std::shared_ptr<WindowResizedEvent>) { this->UpdateDisplaySize(); }

void IMGUISystem::On(eid, std::shared_ptr<MouseMoveEvent> data) {
	this->mouse_pos.x = static_cast<float>(data->new_x);
	this->mouse_pos.y = static_cast<float>(data->new_y);
}

void IMGUISystem::On(eid, std::shared_ptr<MouseScrollEvent> data) {
	this->mouse_wheel.x = static_cast<float>(data->x_offset);
	this->mouse_wheel.y = static_cast<float>(data->y_offset);
}

void IMGUISystem::On(eid, std::shared_ptr<MouseBtnEvent> data) {
	if (data->action == MouseBtnEvent::DOWN) {
		this->mouse_pressed[data->button] = true;
	}
	// Imgui processes a single mouse event (up or down) per Imgui frame, the minimum for a
	// click is two frames so here we track if the mouse was released, and clear the button
	// state at the end of the Imgui frame this handles shorter-than-one-frame clicks, but could
	// still miss double clicks!
	if (data->action == MouseBtnEvent::UP) {
		this->mouse_released[data->button] = true;
	}
}

static ImGuiKey GlfwKeyToImGuiKey(int keycode) {
	switch (keycode) {
	case GLFW_KEY_TAB: return ImGuiKey_Tab;
	case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
	case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
	case GLFW_KEY_UP: return ImGuiKey_UpArrow;
	case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
	case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
	case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
	case GLFW_KEY_HOME: return ImGuiKey_Home;
	case GLFW_KEY_END: return ImGuiKey_End;
	case GLFW_KEY_INSERT: return ImGuiKey_Insert;
	case GLFW_KEY_DELETE: return ImGuiKey_Delete;
	case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
	case GLFW_KEY_SPACE: return ImGuiKey_Space;
	case GLFW_KEY_ENTER: return ImGuiKey_Enter;
	case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
	case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
	case GLFW_KEY_COMMA: return ImGuiKey_Comma;
	case GLFW_KEY_MINUS: return ImGuiKey_Minus;
	case GLFW_KEY_PERIOD: return ImGuiKey_Period;
	case GLFW_KEY_SLASH: return ImGuiKey_Slash;
	case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
	case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
	case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
	case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
	case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
	case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
	case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
	case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
	case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
	case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
	case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
	case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
	case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
	case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
	case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
	case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
	case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
	case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
	case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
	case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
	case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
	case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
	case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
	case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
	case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
	case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
	case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
	case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
	case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
	case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
	case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
	case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
	case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
	case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
	case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
	case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
	case GLFW_KEY_MENU: return ImGuiKey_Menu;
	case GLFW_KEY_0: return ImGuiKey_0;
	case GLFW_KEY_1: return ImGuiKey_1;
	case GLFW_KEY_2: return ImGuiKey_2;
	case GLFW_KEY_3: return ImGuiKey_3;
	case GLFW_KEY_4: return ImGuiKey_4;
	case GLFW_KEY_5: return ImGuiKey_5;
	case GLFW_KEY_6: return ImGuiKey_6;
	case GLFW_KEY_7: return ImGuiKey_7;
	case GLFW_KEY_8: return ImGuiKey_8;
	case GLFW_KEY_9: return ImGuiKey_9;
	case GLFW_KEY_A: return ImGuiKey_A;
	case GLFW_KEY_B: return ImGuiKey_B;
	case GLFW_KEY_C: return ImGuiKey_C;
	case GLFW_KEY_D: return ImGuiKey_D;
	case GLFW_KEY_E: return ImGuiKey_E;
	case GLFW_KEY_F: return ImGuiKey_F;
	case GLFW_KEY_G: return ImGuiKey_G;
	case GLFW_KEY_H: return ImGuiKey_H;
	case GLFW_KEY_I: return ImGuiKey_I;
	case GLFW_KEY_J: return ImGuiKey_J;
	case GLFW_KEY_K: return ImGuiKey_K;
	case GLFW_KEY_L: return ImGuiKey_L;
	case GLFW_KEY_M: return ImGuiKey_M;
	case GLFW_KEY_N: return ImGuiKey_N;
	case GLFW_KEY_O: return ImGuiKey_O;
	case GLFW_KEY_P: return ImGuiKey_P;
	case GLFW_KEY_Q: return ImGuiKey_Q;
	case GLFW_KEY_R: return ImGuiKey_R;
	case GLFW_KEY_S: return ImGuiKey_S;
	case GLFW_KEY_T: return ImGuiKey_T;
	case GLFW_KEY_U: return ImGuiKey_U;
	case GLFW_KEY_V: return ImGuiKey_V;
	case GLFW_KEY_W: return ImGuiKey_W;
	case GLFW_KEY_X: return ImGuiKey_X;
	case GLFW_KEY_Y: return ImGuiKey_Y;
	case GLFW_KEY_Z: return ImGuiKey_Z;
	case GLFW_KEY_F1: return ImGuiKey_F1;
	case GLFW_KEY_F2: return ImGuiKey_F2;
	case GLFW_KEY_F3: return ImGuiKey_F3;
	case GLFW_KEY_F4: return ImGuiKey_F4;
	case GLFW_KEY_F5: return ImGuiKey_F5;
	case GLFW_KEY_F6: return ImGuiKey_F6;
	case GLFW_KEY_F7: return ImGuiKey_F7;
	case GLFW_KEY_F8: return ImGuiKey_F8;
	case GLFW_KEY_F9: return ImGuiKey_F9;
	case GLFW_KEY_F10: return ImGuiKey_F10;
	case GLFW_KEY_F11: return ImGuiKey_F11;
	case GLFW_KEY_F12: return ImGuiKey_F12;
	case GLFW_KEY_F13: return ImGuiKey_F13;
	case GLFW_KEY_F14: return ImGuiKey_F14;
	case GLFW_KEY_F15: return ImGuiKey_F15;
	case GLFW_KEY_F16: return ImGuiKey_F16;
	case GLFW_KEY_F17: return ImGuiKey_F17;
	case GLFW_KEY_F18: return ImGuiKey_F18;
	case GLFW_KEY_F19: return ImGuiKey_F19;
	case GLFW_KEY_F20: return ImGuiKey_F20;
	case GLFW_KEY_F21: return ImGuiKey_F21;
	case GLFW_KEY_F22: return ImGuiKey_F22;
	case GLFW_KEY_F23: return ImGuiKey_F23;
	case GLFW_KEY_F24: return ImGuiKey_F24;
	default: return ImGuiKey_None;
	}
}

void IMGUISystem::On(eid, const std::shared_ptr<KeyboardEvent> data) {
	auto& io = ImGui::GetIO();
	const auto imgui_key = GlfwKeyToImGuiKey(data->key);
	switch (data->action) {
	case KeyboardEvent::KEY_DOWN: io.AddKeyEvent(imgui_key, true); break;
	case KeyboardEvent::KEY_UP: io.AddKeyEvent(imgui_key, false); break;
	case KeyboardEvent::KEY_CHAR:
		if (data->key > 0 && data->key < 0x10000) {
			io.AddInputCharacter(static_cast<unsigned short>(data->key));
		}
		break;
	case KeyboardEvent::KEY_REPEAT: break;
	}

	io.AddKeyEvent(
			ImGuiMod_Ctrl,
			(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
					|| (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS));
	io.AddKeyEvent(
			ImGuiMod_Shift,
			(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
					|| (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS));
	io.AddKeyEvent(
			ImGuiMod_Alt,
			(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
					|| (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS));
	io.AddKeyEvent(
			ImGuiMod_Super,
			(glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS)
					|| (glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS));
}
} // end namespace tec
