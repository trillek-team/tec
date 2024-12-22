#pragma once

#include <functional>
#include <map>
#include <set>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "command-queue.hpp"
#include "event-system.hpp"
#include "events.hpp"

namespace tec {
class IMGUISystem;
class Console;
class OS;

namespace networking {
class ServerConnection;
}

class IMGUISystem :
		public CommandQueue<IMGUISystem>,
		public EventQueue<KeyboardEvent>,
		public EventQueue<MouseMoveEvent>,
		public EventQueue<MouseScrollEvent>,
		public EventQueue<MouseBtnEvent>,
		public EventQueue<WindowResizedEvent> {
	typedef Command<IMGUISystem> GUICommand;

public:
	IMGUISystem();
	~IMGUISystem();

	void Initialize(GLFWwindow* window); // Add initialization method

	void CreateGUI();

	void Update(double delta);

	void CreateDeviceObjects();

	void AddWindowDrawFunction(std::string name, std::function<void()>&& func);

	void ShowWindow(const std::string name) {
		GUICommand show_window([=](IMGUISystem*) { this->visible_windows.insert(name); });
		IMGUISystem::QueueCommand(std::move(show_window));
	}

	void HideWindow(const std::string name) {
		GUICommand hide_window([=](IMGUISystem*) { this->visible_windows.erase(name); });
		IMGUISystem::QueueCommand(std::move(hide_window));
	}

	bool IsWindowVisible(const std::string& name) const {
		return this->visible_windows.find(name) != this->visible_windows.end();
	}

	static const char* GetClipboardText(void* user_data);
	static void SetClipboardText(void* user_data, const char* text);
	static void RenderDrawLists(ImDrawData* draw_data);

private:
	void On(eid, std::shared_ptr<WindowResizedEvent> data) override;
	void On(eid, std::shared_ptr<MouseMoveEvent> data) override;
	void On(eid, std::shared_ptr<MouseScrollEvent> data) override;
	void On(eid, std::shared_ptr<MouseBtnEvent> data) override;
	void On(eid, std::shared_ptr<KeyboardEvent> data) override;

	void UpdateDisplaySize();

	int framebuffer_width{0}, framebuffer_height{0};
	int window_width{0}, window_height{0};
	bool mouse_pressed[3]{false, false, false};
	bool mouse_released[3]{false, false, false};
	ImVec2 mouse_pos{0, 0};
	ImVec2 mouse_wheel{0, 0};

	static GLuint font_texture;
	static GLFWwindow* window;
	static int shader_program, vertex_shader, fragment_shader;
	static int texture_attribute_location, projmtx_attribute_location;
	static int position_attribute_location, uv_attribute_location, color_attribute_location;
	static std::size_t vbo_size, ibo_size;
	static unsigned int vbo, ibo, vao;

	std::set<std::string> visible_windows;

	std::map<std::string, std::function<void()>> window_draw_funcs;
};
} // namespace tec
