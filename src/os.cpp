#include "os.hpp"

#include <iostream>
#include "event-system.hpp"

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

namespace vv {

	// Error helper function used by GLFW for error messaging.
	// Currently outputs to std::cout.
	static void ErrorCallback(int error_no, const char* description) {
		std::cout << "Error " << error_no << ": " << description << std::endl;
	}

	bool OS::InitializeWindow(const int width, const int height, const std::string title,
		const unsigned int glMajor /*= 3*/, const unsigned int glMinor /*= 2*/) {
		glfwSetErrorCallback(ErrorCallback);

		// Initialize the library.
		if (glfwInit() != GL_TRUE) {
			return false;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);

#ifndef __APPLE__
#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#else
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
#else
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

		// Create a windowed mode window and its OpenGL context.
		this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (!this->window) {
			glfwTerminate();
			return false;
		}

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

		// Associate a pointer for this instance with this window.
		glfwSetWindowUserPointer(this->window, this);

		// Set up some callbacks.
		glfwSetWindowSizeCallback(this->window, &OS::WindowResized);
		glfwSetKeyCallback(this->window, &OS::KeyboardEventCallback);
		glfwSetCursorPosCallback(this->window, &OS::MouseMoveEventCallback);
		glfwSetCharCallback(this->window, &OS::CharacterEventCallback);
		glfwSetMouseButtonCallback(this->window, &OS::MouseButtonEventCallback);
		glfwSetWindowFocusCallback(this->window, &OS::WindowFocusChangeCallback);

		glfwGetCursorPos(this->window, &this->old_mouse_x, &this->old_mouse_y);

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

	void OS::MouseButtonEventCallback(GLFWwindow* window, int button, int action, int mods) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->DispatchMouseButtonEvent(button, action, mods);
		}
	}

	void OS::WindowFocusChangeCallback(GLFWwindow* window, int focused) {
		if (focused == GL_FALSE) {
			// Get the user pointer and cast it.
			OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

			if (os) {
				// TODO: Implement a DispatchWindowFocusEvent() method in OS
				// TODO: Dispatch a focus changed event.
			}
		}
	}

	void OS::UpdateWindowSize(const int width, const int height) {
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
		glfwSetCursorPos(this->window, x, y);
	}

}
