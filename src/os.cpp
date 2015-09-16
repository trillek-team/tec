#include "os.hpp"

#include <algorithm>
#include "spdlog/spdlog.h"
#include "event-system.hpp"
#include "events.hpp"


#ifdef __APPLE__
// Needed so we can disable retina support for our window.
#define GLFW_EXPOSE_NATIVE_COCOA 1
#define GLFW_EXPOSE_NATIVE_NSGL 1
#include <GLFW/glfw3native.h>
// We can't just include objc/runtime.h and objc/message.h because glfw is too forward thinking for its own good.
typedef void* SEL;
extern "C" id objc_msgSend(id self, SEL op, ...);
extern "C" SEL sel_getUid(const char *str);
#endif

namespace tec {
	GLFWwindow* OS::focused_window;

	// Error helper function used by GLFW for error messaging.
	static void ErrorCallback(int error_no, const char* description) {
		spdlog::get("console_log")->error() << "[OS] GLFW Error " << error_no << ": " << description;
	}

	bool OS::InitializeWindow(const int width, const int height, const std::string title,
		const unsigned int glMajor /*= 3*/, const unsigned int glMinor /*= 2*/) {
		glfwSetErrorCallback(ErrorCallback);

		auto l = spdlog::get("console_log");
		// Initialize the library.
		if (glfwInit() != GL_TRUE) {
			return false;
		}

		// don't hint much (request default context version)
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

		// Create a windowed mode window and its OpenGL context.
		this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (!this->window) {
			// creating a window with default hints failed
			// try again with specific hints
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

			if (!this->window) {
				// still not right, give up
				glfwTerminate();
				return false;
			}
		}

		// attach the context
		glfwMakeContextCurrent(this->window);

		// check the context version
		std::string glcx_version((char*)glGetString(GL_VERSION));
		std::string glcx_major = glcx_version.substr(0, glcx_version.find('.', 0));
		if (glcx_major == "1" || glcx_major == "2") {
			// we got a version 1 context, that will not work
			// so try again.
			glfwMakeContextCurrent(nullptr);
			glfwDestroyWindow(this->window);
			// be more specific this time
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

			if (!this->window) {
				glfwTerminate();
				return false;
			}
			// attach the context
			glfwMakeContextCurrent(this->window);

			// check the context version again
			glcx_version = (char*)glGetString(GL_VERSION);
			glcx_major = glcx_version.substr(0, glcx_version.find('.', 0));
			if (glcx_major == "1") {
				// still 1, higher versions probably not supported
				glfwTerminate();
				l->critical() << "[OS] Initializing OpenGL failed, unsupported version: " << glcx_version << '\n' 
					<< "Press \"Enter\" to exit\n";
				std::cin.get();
				return false;
			}
		}

		const char* glcx_glslver = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		l->info() << "GL version : " << glcx_version << " GLSL version : " << glcx_glslver; 
		const char* glcx_vendor = (char*)glGetString(GL_VENDOR);
		const char* glcx_renderer = (char*)glGetString(GL_RENDERER);
		l->info() << glcx_vendor << " " << glcx_renderer;
		
		this->client_width = width;
		this->client_height = height;

#ifdef __APPLE__
		// Force retina displays to create a 1x framebuffer so we don't choke our fill rate.
		id cocoaWindow = glfwGetCocoaWindow(this->window);
		id cocoaGLView = ((id(*)(id, SEL)) objc_msgSend)(cocoaWindow, sel_getUid("contentView"));
		((void(*)(id, SEL, bool)) objc_msgSend)(cocoaGLView, sel_getUid("setWantsBestResolutionOpenGLSurface:"), false);
#endif

		// attach the context
		glfwMakeContextCurrent(this->window);

#ifndef __APPLE__
		// setting glewExperimental fixes a glfw context problem
		// (tested on Ubuntu 13.04)
		glewExperimental = GL_TRUE;

		// Init GLEW.
		GLuint error = glewInit();
		if (error != GLEW_OK) {
			return false;
		}
#endif

		// Getting a list of the avail extensions
		
		std::string extensions = "";
		GLint num_exts = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &num_exts);
		l->info("Extensions {} : ", num_exts);
		for (GLint e=0; e < num_exts; e++) {
			l->info() << (const char*) glGetStringi(GL_EXTENSIONS, e);
		}
		
		// Associate a pointer for this instance with this window.
		glfwSetWindowUserPointer(this->window, this);

		// Set up some callbacks.
		glfwSetWindowSizeCallback(this->window, &OS::WindowResized);
		glfwSetKeyCallback(this->window, &OS::KeyboardEventCallback);
		glfwSetCursorPosCallback(this->window, &OS::MouseMoveEventCallback);
		glfwSetCharCallback(this->window, &OS::CharacterEventCallback);
		glfwSetMouseButtonCallback(this->window, &OS::MouseButtonEventCallback);
		glfwSetScrollCallback(this->window, &OS::MouseScrollEventCallback);
		glfwSetWindowFocusCallback(this->window, &OS::WindowFocusChangeCallback);
		glfwSetDropCallback(this->window, &OS::FileDropCallback);

		glfwGetCursorPos(this->window, &this->old_mouse_x, &this->old_mouse_y);

		UpdateWindowSize(width, height);

		OS::focused_window = this->window;

		return true;
	}

	void OS::MakeCurrent() {
		glfwMakeContextCurrent(this->window);
	}

	void OS::DetachContext() {
		glfwMakeContextCurrent(NULL);
	}

	void OS::Terminate() {
		glfwTerminate();
	}
	
	void OS::Quit() {
		glfwSetWindowShouldClose(this->window, true);
	}

	bool OS::Closing() {
		return glfwWindowShouldClose(this->window) > 0;
	}

	void OS::SwapBuffers() {
		glfwSwapBuffers(this->window);
	}

	void OS::OSMessageLoop() {
		glfwPollEvents();
	}

	int OS::GetWindowWidth() {
		return this->client_width;
	}

	int OS::GetWindowHeight() {
		return this->client_height;
	}


	GLFWwindow* OS::GetWindow() {
		return this->window;
	}

	double OS::GetDeltaTime() {
		double new_time = glfwGetTime();
		double delta_time = new_time - this->last_time;
		this->last_time = new_time;
		return delta_time;
	}

	void OS::WindowResized(GLFWwindow* window, int width, int height) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->UpdateWindowSize(width, height);
		}
	}

	void OS::KeyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->DispatchKeyboardEvent(key, scancode, action, mods);
		}
	}

	void OS::CharacterEventCallback(GLFWwindow* window, unsigned int uchar) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->DispatchCharacterEvent(uchar);
		}
	}

	void OS::MouseMoveEventCallback(GLFWwindow* window, double x, double y) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->DispatchMouseMoveEvent(x, y);
		}
	}

	void OS::MouseScrollEventCallback(GLFWwindow* window, double x, double y) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->DispatchMouseScrollEvent(x, y);
		}
	}

	void OS::MouseButtonEventCallback(GLFWwindow* window, int button, int action, int mods) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->DispatchMouseButtonEvent(button, action, mods);
		}
	}

	void OS::WindowFocusChangeCallback(GLFWwindow* window, int focused) {
		if (focused == GL_FALSE) {
			OS::focused_window = nullptr;
			// Get the user pointer and cast it.
			OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

			if (os) {

			}
		}
		else if (focused == GL_TRUE) {
			OS::focused_window = window;
		}
	}

	void OS::FileDropCallback(GLFWwindow* window, int count, const char** paths) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));
		os->DispatchFileDropEvent(count, paths);
	}

	void OS::UpdateWindowSize(const int width, const int height) {
		std::shared_ptr<WindowResizedEvent> resize_event = std::make_shared<WindowResizedEvent>(
			WindowResizedEvent {this->client_width, this->client_height, width, height});
		EventSystem<WindowResizedEvent>::Get()->Emit(resize_event);

		this->client_width = width;
		this->client_height = height;
	}

	void OS::DispatchKeyboardEvent(const int key, const int scancode, const int action, const int mods) {
		std::shared_ptr<KeyboardEvent> key_event = std::make_shared<KeyboardEvent>(
			KeyboardEvent {key, scancode, KeyboardEvent::KEY_DOWN, mods});
		// Default is KEY_DOWN, check if it is REPEAT or UP instead.
		if (action == GLFW_REPEAT) {
			key_event->action = KeyboardEvent::KEY_REPEAT;
		}
		else if (action == GLFW_RELEASE) {
			key_event->action = KeyboardEvent::KEY_UP;
		}

		EventSystem<KeyboardEvent>::Get()->Emit(key_event);
	}

	void OS::DispatchCharacterEvent(const unsigned int uchar) {
		std::shared_ptr<KeyboardEvent> key_event = std::make_shared<KeyboardEvent>(
			KeyboardEvent {(const int)uchar, 0, KeyboardEvent::KEY_CHAR, 0});
		EventSystem<KeyboardEvent>::Get()->Emit(key_event);
	}

	void OS::DispatchMouseMoveEvent(const double x, const double y) {
		std::shared_ptr<MouseMoveEvent> mmov_event = std::make_shared<MouseMoveEvent>(
			MouseMoveEvent {
			static_cast<double>(x) / this->client_width,
			static_cast<double>(y) / this->client_height,
			static_cast<int>(this->old_mouse_x),
			static_cast<int>(this->old_mouse_y),
			static_cast<int>(x),
			static_cast<int>(y)
		});
		EventSystem<MouseMoveEvent>::Get()->Emit(mmov_event);

		// If we are in mouse lock we will snap the mouse to the middle of the screen.
		if (this->mouse_lock) {
			this->old_mouse_x = this->client_width / 2;
			this->old_mouse_y = this->client_height / 2;
			glfwSetCursorPos(this->window, this->old_mouse_x, this->old_mouse_y);
		}
		else {
			this->old_mouse_x = x;
			this->old_mouse_y = y;
		}
	}
	
	void OS::DispatchMouseScrollEvent(const double xoffset, const double yoffset) {
		std::shared_ptr<MouseScrollEvent> mscroll_event = std::make_shared<MouseScrollEvent>(
			MouseScrollEvent {
			static_cast<double>(xoffset),
			static_cast<double>(yoffset),
		});
		EventSystem<MouseScrollEvent>::Get()->Emit(mscroll_event);

	}

	void OS::DispatchMouseButtonEvent(const int button, const int action, const int mods) {
		std::shared_ptr<MouseBtnEvent> mbtn_event = std::make_shared<MouseBtnEvent>();
		if (action == GLFW_PRESS) {
			mbtn_event->action = MouseBtnEvent::DOWN;
		}
		else {
			mbtn_event->action = MouseBtnEvent::UP;
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			mbtn_event->button = MouseBtnEvent::LEFT;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			mbtn_event->button = MouseBtnEvent::RIGHT;
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			mbtn_event->button = MouseBtnEvent::MIDDLE;
		}
		EventSystem<MouseBtnEvent>::Get()->Emit(mbtn_event);
	}

	void OS::DispatchFileDropEvent(const int count, const char** paths) {
		std::shared_ptr<FileDropEvent> fd_event = std::make_shared<FileDropEvent>();
		for (int i = 0; i < count; ++i) {
			fd_event->filenames.push_back(paths[i]);
			while (fd_event->filenames[i].find("\\") != std::string::npos) {
				fd_event->filenames[i].replace(fd_event->filenames[i].find("\\"), 1, "/");
			}
		}
		EventSystem<FileDropEvent>::Get()->Emit(fd_event);
	}

	void OS::ToggleMouseLock() {
		this->mouse_lock = !this->mouse_lock;
		if (this->mouse_lock) {
			glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	void OS::SetMousePosition(const double x, const double y) {
		glfwSetCursorPos(OS::focused_window, x, y);
	}

	void OS::GetMousePosition(double* x, double* y) {
		if (focused_window) {
			glfwGetCursorPos(OS::focused_window, x, y);
		}
	}

}
