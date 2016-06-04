// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <map>
#include <string>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#ifndef __unix
#include <GL/wglew.h>
#endif
#endif

#include <GLFW/glfw3.h>
#include <imgui.h>

#include "multiton.hpp"
#include "types.hpp"
#include "events.hpp"
#include "event-system.hpp"
#include "command-queue.hpp"

namespace tec {
	class IMGUISystem;
	typedef Command<IMGUISystem> GUICommand;
	struct KeyboardEvent;
	struct WindowResizedEvent;

	class IMGUISystem :
		public CommandQueue < IMGUISystem >,
		public EventQueue < KeyboardEvent >,
		public EventQueue < MouseMoveEvent >,
		public EventQueue < MouseScrollEvent >,
		public EventQueue < WindowResizedEvent > {
	public:
		IMGUISystem(GLFWwindow* window);
		~IMGUISystem();

		void Update(double delta);

		void CreateDeviceObjects();

		void AddWindowDrawFunction(std::string name, std::function<void()>&& func);

		void ShowWindow(std::string name) {
			GUICommand show_window(
				[=] (IMGUISystem* gui_system) {
				this->visible_windows.insert(name);
			});
			IMGUISystem::QueueCommand(std::move(show_window));
		}

		void HideWindow(std::string name) {
			GUICommand hide_window(
				[=] (IMGUISystem* gui_system) {
				this->visible_windows.erase(name);
			});
			IMGUISystem::QueueCommand(std::move(hide_window));
		}

		bool IsWindowVisible(std::string name) {
			return this->visible_windows.find(name) != this->visible_windows.end();
		}

		static const char* GetClipboardText();
		static void SetClipboardText(const char* text);
		static void RenderDrawLists(ImDrawData* draw_data);
	private:
		void On(std::shared_ptr<WindowResizedEvent> data);
		void On(std::shared_ptr<MouseMoveEvent > data);
		void On(std::shared_ptr<MouseScrollEvent > data);
		void On(std::shared_ptr<KeyboardEvent> data);

		ImGuiIO& io;
		int framebuffer_width, framebuffer_height;
		int window_width, window_height;
		bool mouse_pressed[3];
		ImVec2 mouse_pos;
		ImVec2 mouse_wheel;

		static GLuint font_texture;
		static GLFWwindow* window;
		static int shader_program, vertex_shader, fragment_shader;
		static int texture_attribute_location, projmtx_attribute_location;
		static int position_attribute_location, uv_attribute_location, color_attribute_location;
		static std::size_t vbo_size;
		static unsigned int vbo, vao;

		std::set<std::string> visible_windows;

		std::map<std::string, std::function<void()>> window_draw_funcs;
	};
}
