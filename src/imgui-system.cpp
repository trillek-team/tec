#include "imgui-system.hpp"
#include "os.hpp"

#ifdef _MSC_VER
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif
#include "events.hpp"

namespace tec {
	GLFWwindow* IMGUISystem::window = nullptr;
	int IMGUISystem::shader_program = 0, IMGUISystem::vertex_shader = 0, IMGUISystem::fragment_shader = 0;
	int IMGUISystem::texture_attribute_location = 0, IMGUISystem::projmtx_attribute_location = 0;
	int IMGUISystem::position_attribute_location = 0, IMGUISystem::uv_attribute_location = 0, IMGUISystem::color_attribute_location = 0;
	size_t IMGUISystem::vbo_size = 0;
	unsigned int IMGUISystem::vbo = 0, IMGUISystem::vao = 0;
	GLuint IMGUISystem::font_texture = 0;

	IMGUISystem::IMGUISystem(GLFWwindow* window) : io(ImGui::GetIO()) {
		IMGUISystem::window = window;
		this->io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB; // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
		this->io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		this->io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		this->io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		this->io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		this->io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		this->io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		this->io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		this->io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		this->io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		this->io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		this->io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		this->io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		this->io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		this->io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		this->io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		this->io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		this->io.RenderDrawListsFn = RenderDrawLists;
		this->io.SetClipboardTextFn = SetClipboardText;
		this->io.GetClipboardTextFn = GetClipboardText;
#ifdef _MSC_VER
		this->io.ImeWindowHandle = glfwGetWin32Window(IMGUISystem::window);
#endif

		glfwGetWindowSize(IMGUISystem::window, &this->window_width, &this->window_height);
		glfwGetFramebufferSize(IMGUISystem::window, &this->framebuffer_width, &this->framebuffer_height);
		io.DisplaySize = ImVec2((float)this->framebuffer_width, (float)this->framebuffer_height);
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
		ImGui::Shutdown();
	}

	void IMGUISystem::Update(double delta) {
		this->io.DeltaTime = static_cast<float>(delta);
		EventQueue<WindowResizedEvent>::ProcessEventQueue();
		EventQueue<KeyboardEvent>::ProcessEventQueue();

		if (!IMGUISystem::font_texture) {
			CreateDeviceObjects();
		}

		// Setup inputs
		// (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
		if (glfwGetWindowAttrib(IMGUISystem::window, GLFW_FOCUSED)) {
			double mouse_x, mouse_y;
			glfwGetCursorPos(IMGUISystem::window, &mouse_x, &mouse_y);
			mouse_x *= (float)this->framebuffer_width / this->window_width;                        // Convert mouse coordinates to pixels
			mouse_y *= (float)this->framebuffer_height / this->window_height;
			this->io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);   // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
		}
		else {
			this->io.MousePos = ImVec2(-1, -1);
		}

		for (int i = 0; i < 3; i++) {
			this->io.MouseDown[i] = mouse_pressed[i] || glfwGetMouseButton(IMGUISystem::window, i) != 0;    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
			mouse_pressed[i] = false;
		}

		this->io.MouseWheel = g_MouseWheel;
		g_MouseWheel = 0.0f;

		// Hide OS mouse cursor if ImGui is drawing it
		glfwSetInputMode(IMGUISystem::window, GLFW_CURSOR, this->io.MouseDrawCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);

		// Start the frame
		ImGui::NewFrame();

		for (auto draw_func : this->window_draw_funcs) {
			draw_func.second();
		}
		ImGui::Render();
	}

	void IMGUISystem::CreateDeviceObjects() {
		const GLchar *vertex_shader =
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

		const GLchar* fragment_shader =
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
		glShaderSource(IMGUISystem::vertex_shader, 1, &vertex_shader, 0);
		glShaderSource(IMGUISystem::fragment_shader, 1, &fragment_shader, 0);
		glCompileShader(IMGUISystem::vertex_shader);
		glCompileShader(IMGUISystem::fragment_shader);
		glAttachShader(IMGUISystem::shader_program, IMGUISystem::vertex_shader);
		glAttachShader(IMGUISystem::shader_program, IMGUISystem::fragment_shader);
		glLinkProgram(IMGUISystem::shader_program);

		IMGUISystem::texture_attribute_location = glGetUniformLocation(IMGUISystem::shader_program, "Texture");
		IMGUISystem::projmtx_attribute_location = glGetUniformLocation(IMGUISystem::shader_program, "ProjMtx");
		IMGUISystem::position_attribute_location = glGetAttribLocation(IMGUISystem::shader_program, "Position");
		IMGUISystem::uv_attribute_location = glGetAttribLocation(IMGUISystem::shader_program, "UV");
		IMGUISystem::color_attribute_location = glGetAttribLocation(IMGUISystem::shader_program, "Color");

		glGenBuffers(1, &IMGUISystem::vbo);

		glGenVertexArrays(1, &IMGUISystem::vao);
		glBindVertexArray(IMGUISystem::vao);
		glBindBuffer(GL_ARRAY_BUFFER, IMGUISystem::vbo);
		glEnableVertexAttribArray(IMGUISystem::position_attribute_location);
		glEnableVertexAttribArray(IMGUISystem::uv_attribute_location);
		glEnableVertexAttribArray(IMGUISystem::color_attribute_location);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
		glVertexAttribPointer(IMGUISystem::position_attribute_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
		glVertexAttribPointer(IMGUISystem::uv_attribute_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
		glVertexAttribPointer(IMGUISystem::color_attribute_location, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Create the font texture.
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.

		glGenTextures(1, &font_texture);
		glBindTexture(GL_TEXTURE_2D, font_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// Store our identifier
		io.Fonts->TexID = (void *)(intptr_t)font_texture;

		// Cleanup (don't clear the input data if you want to append new fonts later)
		io.Fonts->ClearInputData();
		io.Fonts->ClearTexData();
	}

	const char* IMGUISystem::GetClipboardText() {
		return glfwGetClipboardString(IMGUISystem::window);
	}

	void IMGUISystem::SetClipboardText(const char* text) {
		glfwSetClipboardString(IMGUISystem::window, text);
	}

	void IMGUISystem::RenderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count) {
		if (cmd_lists_count == 0) {
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
		const float width = ImGui::GetIO().DisplaySize.x;
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

		// Grow our buffer according to what we need
		size_t total_vtx_count = 0;
		for (int n = 0; n < cmd_lists_count; n++)
			total_vtx_count += cmd_lists[n]->vtx_buffer.size();
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		size_t needed_vtx_size = total_vtx_count * sizeof(ImDrawVert);
		if (vbo_size < needed_vtx_size) {
			vbo_size = needed_vtx_size + 5000 * sizeof(ImDrawVert);  // Grow buffer
			glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL, GL_STREAM_DRAW);
		}

		// Copy and convert all vertices into a single contiguous buffer
		unsigned char* buffer_data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (!buffer_data)
			return;
		for (int n = 0; n < cmd_lists_count; n++) {
			const ImDrawList* cmd_list = cmd_lists[n];
			memcpy(buffer_data, &cmd_list->vtx_buffer[0], cmd_list->vtx_buffer.size() * sizeof(ImDrawVert));
			buffer_data += cmd_list->vtx_buffer.size() * sizeof(ImDrawVert);
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(vao);

		int cmd_offset = 0;
		for (int n = 0; n < cmd_lists_count; n++) {
			const ImDrawList* cmd_list = cmd_lists[n];
			int vtx_offset = cmd_offset;
			const ImDrawCmd* pcmd_end = cmd_list->commands.end();
			for (const ImDrawCmd* pcmd = cmd_list->commands.begin(); pcmd != pcmd_end; pcmd++) {
				if (pcmd->user_callback) {
					pcmd->user_callback(cmd_list, pcmd);
				}
				else {
					glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->texture_id);
					glScissor((int)pcmd->clip_rect.x, (int)(height - pcmd->clip_rect.w), (int)(pcmd->clip_rect.z - pcmd->clip_rect.x), (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
					glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
				}
				vtx_offset += pcmd->vtx_count;
			}
			cmd_offset = vtx_offset;
		}

		// Restore modified state
		glBindVertexArray(0);
		glUseProgram(0);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void IMGUISystem::AddWindowDrawFunction(std::string name, std::function<void()>&& func) {
		this->window_draw_funcs[name] = std::move(func);
	}


	void IMGUISystem::On(std::shared_ptr<WindowResizedEvent> data) {
		glfwGetWindowSize(IMGUISystem::window, &this->window_width, &this->window_height);
		glfwGetFramebufferSize(IMGUISystem::window, &this->framebuffer_width, &this->framebuffer_height);
		io.DisplaySize = ImVec2((float)this->framebuffer_width, (float)this->framebuffer_height);
	}

	void IMGUISystem::On(std::shared_ptr<KeyboardEvent> data) {
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
