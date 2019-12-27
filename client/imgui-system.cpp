// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "imgui-system.hpp"

#include <spdlog/spdlog.h>

#ifdef _MSC_VER
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif

#include "filesystem.hpp"
#include "os.hpp"
#include "events.hpp"

namespace tec {
	GLFWwindow* IMGUISystem::window = nullptr;
	int IMGUISystem::shader_program = 0, IMGUISystem::vertex_shader = 0, IMGUISystem::fragment_shader = 0;
	int IMGUISystem::texture_attribute_location = 0, IMGUISystem::projmtx_attribute_location = 0;
	int IMGUISystem::position_attribute_location = 0, IMGUISystem::uv_attribute_location = 0, IMGUISystem::color_attribute_location = 0;
	std::size_t IMGUISystem::vbo_size = 0, IMGUISystem::ibo_size = 0;
	unsigned int IMGUISystem::vbo = 0, IMGUISystem::ibo = 0, IMGUISystem::vao = 0;
	GLuint IMGUISystem::font_texture = 0;

	std::string inifilename;
	std::string logfilename;

	IMGUISystem::IMGUISystem(GLFWwindow* _window) {
		ImGui::CreateContext();
		IMGUISystem::window = _window;
		auto& io = ImGui::GetIO();
		inifilename = (FilePath::GetUserSettingsPath() / "imgui.ini").toString();
		logfilename = (FilePath::GetUserCachePath() / "imgui_log.txt").toString();
		io.IniFilename = inifilename.c_str();
#if defined(DEBUG) || defined(_DEBUG)
		io.LogFilename = logfilename.c_str();
#endif
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB; // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		io.SetClipboardTextFn = SetClipboardText;
		io.GetClipboardTextFn = GetClipboardText;
#ifdef _MSC_VER
		io.ImeWindowHandle = glfwGetWin32Window(IMGUISystem::window);
#endif
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
		this->AddWindowDrawFunction("main_menu", [this] () {
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
		EventQueue<KeyboardEvent>::ProcessEventQueue();

		// Setup inputs
		// (we already got mouse wheel, keyboard keys & characters from event system
		if (glfwGetWindowAttrib(IMGUISystem::window, GLFW_FOCUSED)) {
			this->mouse_pos.x *= (float)this->framebuffer_width / this->window_width;  // Convert mouse coordinates to pixels
			this->mouse_pos.y *= (float)this->framebuffer_height / this->window_height;
			io.MousePos = ImVec2((float)mouse_pos.x, (float)mouse_pos.y);   // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
			io.MouseWheel += this->mouse_wheel.y; // ImGUI not support x axis scroll :(
			this->mouse_wheel.y = 0;
			this->mouse_wheel.x = 0;
		}
		else {
			io.MousePos = ImVec2(-1, -1);
		}

		for (int i = 0; i < 3; i++) {
			io.MouseDown[i] = mouse_pressed[i] || glfwGetMouseButton(IMGUISystem::window, i) != 0;    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
			mouse_pressed[i] = false;
		}

		// Start the frame
		ImGui::NewFrame();

		for (auto window_name : this->visible_windows) {
			if (this->window_draw_funcs.find(window_name) != this->window_draw_funcs.end()) {
				this->window_draw_funcs.at(window_name)();
			}
		}
		ImGui::Render();
		this->RenderDrawLists(ImGui::GetDrawData());
	}

	void IMGUISystem::CreateDeviceObjects() {
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

		IMGUISystem::shader_program = glCreateProgram();
		IMGUISystem::vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		IMGUISystem::fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(IMGUISystem::vertex_shader, 1, &_vertex_shader, 0);
		glShaderSource(IMGUISystem::fragment_shader, 1, &_fragment_shader, 0);
		GLint success = 0;
		glCompileShader(IMGUISystem::vertex_shader);
		glGetShaderiv(IMGUISystem::vertex_shader, GL_COMPILE_STATUS, &success);
		glCompileShader(IMGUISystem::fragment_shader);
		glGetShaderiv(IMGUISystem::fragment_shader, GL_COMPILE_STATUS, &success);
		glAttachShader(IMGUISystem::shader_program, IMGUISystem::vertex_shader);
		glAttachShader(IMGUISystem::shader_program, IMGUISystem::fragment_shader);
		glLinkProgram(IMGUISystem::shader_program);
		GLint isLinked = 0;
		glGetProgramiv(IMGUISystem::shader_program, GL_LINK_STATUS, (int*)&isLinked);

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

#define OFFSETOF(TYPE, ELEMENT) ((std::size_t)&(((TYPE *)0)->ELEMENT))
		glVertexAttribPointer(IMGUISystem::position_attribute_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
		glVertexAttribPointer(IMGUISystem::uv_attribute_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
		glVertexAttribPointer(IMGUISystem::color_attribute_location, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Create the font texture.
		unsigned char* pixels;
		int width, height;
		auto& io = ImGui::GetIO();
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.

		glGenTextures(1, &font_texture);
		glBindTexture(GL_TEXTURE_2D, font_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// Store our identifier
		io.Fonts->TexID = (void*)(intptr_t)font_texture;

		// Cleanup (don't clear the input data if you want to append new fonts later)
		io.Fonts->ClearInputData();
		io.Fonts->ClearTexData();
	}

	const char* IMGUISystem::GetClipboardText(void*) {
		return glfwGetClipboardString(IMGUISystem::window);
	}

	void IMGUISystem::SetClipboardText(void*, const char* text) {
		glfwSetClipboardString(IMGUISystem::window, text);
	}

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
		const auto display_size{ ImGui::GetIO().DisplaySize };
		const float width = display_size.x;
		const float height = ImGui::GetIO().DisplaySize.y;
		const float ortho_projection[4][4] =
		{
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

			unsigned char* vtx_data = (unsigned char*)glMapBufferRange(GL_ARRAY_BUFFER, 0, needed_vtx_size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

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

			unsigned char* idx_data = (unsigned char*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, needed_idx_size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

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
					glScissor((int)pcmd->ClipRect.x, (int)(height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
					glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer_offset);
				}
				idx_buffer_offset += pcmd->ElemCount;
			}
		}

		/*

		// Grow our buffer according to what we need
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		size_t total_vtx_count = 0;
		for (int n = 0; n < draw_data->CmdListsCount; n++) {
			total_vtx_count += draw_data->CmdLists[n]->VtxBuffer.size();
		}
		size_t needed_vtx_size = total_vtx_count * sizeof(ImDrawVert);
		if (vbo_size < needed_vtx_size) {
			vbo_size = needed_vtx_size + 5000 * sizeof(ImDrawVert);  // Grow buffer
			glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL, GL_STREAM_DRAW);
		}

		// Copy and convert all vertices into a single contiguous buffer
		unsigned char* buffer_data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (!buffer_data)
			return;
		for (int n = 0; n < draw_data->CmdListsCount; n++) {
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			memcpy(buffer_data, &cmd_list->VtxBuffer[0], cmd_list->VtxBuffer.size() * sizeof(ImDrawVert));
			buffer_data += cmd_list->VtxBuffer.size() * sizeof(ImDrawVert);
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(vao);

		int cmd_offset = 0;
		for (int n = 0; n < draw_data->CmdListsCount; n++) {
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			int vtx_offset = cmd_offset;
			const ImDrawCmd* pcmd_end = cmd_list->CmdBuffer.end();
			for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != pcmd_end; pcmd++) {
				if (pcmd->UserCallback) {
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else {
					glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
					glScissor((int)pcmd->ClipRect.x, (int)(height - pcmd->ClipRect.w),
						(int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
					glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->ElemCount);
				}
				vtx_offset += pcmd->ElemCount;
			}
			cmd_offset = vtx_offset;
		}
		*/
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


	void IMGUISystem::On(std::shared_ptr<WindowResizedEvent> data) {
		this->UpdateDisplaySize();
	}

	void IMGUISystem::On(std::shared_ptr<MouseMoveEvent> data) {
		this->mouse_pos.x = static_cast<float>(data->new_x);
		this->mouse_pos.y = static_cast<float>(data->new_y);
	}

	void IMGUISystem::On(std::shared_ptr<MouseScrollEvent> data) {
		this->mouse_wheel.x = static_cast<float>(data->x_offset);
		this->mouse_wheel.y = static_cast<float>(data->y_offset);
	}


	void IMGUISystem::On(std::shared_ptr<KeyboardEvent> data) {
		auto& io = ImGui::GetIO();
		if (data->action == KeyboardEvent::KEY_DOWN) {
			io.KeysDown[data->key] = true;
		}
		else if (data->action == KeyboardEvent::KEY_UP) {
			io.KeysDown[data->key] = false;
		}

		if (data->action == KeyboardEvent::KEY_CHAR) {
			if (data->key > 0 && data->key < 0x10000) {
				io.AddInputCharacter((unsigned short)data->key);
			}
			return;
		}

		(void)data->mods; // Modifiers are not reliable across systems
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	}
}
