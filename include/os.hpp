#pragma once

#ifndef __APPLE__
#include <GL/glew.h>
#ifndef __unix
#include <GL/wglew.h>
#endif
#endif

#include <GLFW/glfw3.h>
#include <string>
#include <chrono>

namespace vv {
	struct KeyboardEvent {
		enum KEY_ACTION { KEY_DOWN, KEY_UP, KEY_REPEAT, KEY_CHAR };
		int key;
		int scancode;
		KEY_ACTION action;
		int mods;
	};

	struct MouseBtnEvent {
		enum MOUSE_BTN_ACTION { DOWN, UP };
		enum MOUSE_BTN { LEFT, RIGHT, MIDDLE };
		MOUSE_BTN_ACTION action;
		MOUSE_BTN button;
	};

	struct MouseMoveEvent {
		double norm_x, norm_y; // Resolution independent new x, y (0-1) from upper-left to lower-right.
		int old_x, old_y; // Client space old x, y.
		int new_x, new_y; // Client space new x, y.
	};

	class OS {
	public:
		OS() : mouse_lock(false) { }
		~OS() { }

		/**
		* \brief Initialize the rendering window and makes the window's context the current one.
		*
		* \param[in] const unsigned int width, height The window's client width and height
		* \param[in] const std::string title The title to show in the title bar and task manager.
		* \return bool If creation was successful or not.
		*/
		bool InitializeWindow(const int width, const int height, const std::string title,
			const unsigned int glMajor = 3, const unsigned int glMinor = 2);

		/** \brief Make the context of the window current for the calling thread
		*
		*/
		void MakeCurrent();

		/** \brief Detach the context of the window from the calling thread
		*
		*/
		void DetachContext();

		/**
		* \brief Calls GLFWTerminate to close the window.
		*
		* This is a static method so it can be called from anywhere to terminate the current window.
		* \return void
		*/
		static void Terminate();

		/**
		* \brief Checks if the window is closing.
		*
		* \return bool True if the window is closing.
		*/
		bool Closing();

		/**
		* \brief Swap the front and back buffers. Call after rendering.
		*
		* \return void
		*/
		void SwapBuffers();

		/**
		* \brief Processes events in the OS message event loop.
		*
		*/
		void OSMessageLoop();

		/**
		* \brief Gets the cached window width.
		*
		* \return int The window width.
		*/
		int GetWindowWidth();

		/**
		* \brief Gets the cached window height.
		*
		* \return int The window height.
		*/
		int GetWindowHeight();

		/**
		* \brief Returns the time since the start of the program.
		*
		* \return double The number of nanoseconds that have pass since the last call.
		* that have passed.
		*/
		double GetDeltaTime();

		/**
		* \brief Callback for when the window is resized.
		*
		* \param[in] GLFWwindow* window
		* \param[in] int width, height The new client width and height of the window.
		* \return void
		*/
		static void WindowResized(GLFWwindow* window, int width, int height);

		/**
		* \brief Callback for keyboard events.
		*
		* \param[in] GLFWwindow* window
		* \param[in] int key ASCII key number.
		* \param[in] int scancode The converted key value
		* \param[in] int action The event type.
		* \param[in] int mods Modifier keys.
		* \return void
		*/
		static void KeyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		/**
		* \brief Callback for Unicode character event.
		*
		* This is different from just a normal keyboard event as it has been translated and modified by
		* the OS and is just like typing into a text editor.
		* \param[in] GLFWwindow* window
		* \param[in] unsigned int uchar The Unicode character key code.
		* \return void
		*/
		static void CharacterEventCallback(GLFWwindow* window, unsigned int uchar);

		/**
		* \brief Callback for mouse move events.
		*
		* \param[in] GLFWwindow* window
		* \param[in] double x, y The new x and y coordinate of the mouse in screen coordinates.
		* \return void
		*/
		static void MouseMoveEventCallback(GLFWwindow* window, double x, double y);

		/**
		* \brief Callback for mouse button events.
		*
		* \param[in] GLFWwindow* window
		* \param[in] int button The button that the action occurred on.
		* \param[in] int action The action that occurred.
		* \param[in] int mods Modifier keys.
		* \return void
		*/
		static void MouseButtonEventCallback(GLFWwindow* window, int button, int action, int mods);

		/**
		* \brief Callback for window focus change events.
		*
		* \param[in] GLFWwindow* window
		* \param[in] int focused GL_TRUE if focused, GL_FALSE if unfocused.
		* \return void
		*/
		static void WindowFocusChangeCallback(GLFWwindow* window, int focused);

		/**
		* \brief Toggles whether the mouse cursor should be locked to the current window.
		*
		* \return void
		*/
		void ToggleMouseLock();

		/**
		* \brief Sets the mouse cursor position relative to the upper-left corner of the window.
		*
		* \param[in] double x, y The new x and y coordinate of the mouse in screen coordinates.
		* \return void
		*/
		void SetMousePosition(const double x, const double y);
	private:
		/**
		* \brief Updates the internal size variables from the windowResized callback.
		*
		* \param[in] const int width, height The new client width and height of the window
		* \return void
		*/
		void UpdateWindowSize(const int width, const int height);

		/**
		* \brief Dispatches keyboard events from the callback.
		*
		* \param[in] const int key ASCII key number.
		* \param[in] const int scancode The converted key value
		* \param[in] const int action The event type.
		* \param[in] const int mods Modifier keys.
		* \return void
		*/
		void DispatchKeyboardEvent(const int key, const int scancode, const int action, const int mods);

		/**
		* \brief Dispatches a character event.
		*
		* \param[in] const unsigned int uchar The Unicode character key code.
		* \return void
		*/
		void DispatchCharacterEvent(const unsigned int uchar);

		/**
		* \brief Dispatches mouse movement events.
		*
		* It determines the changes in mouse position and stores the new position for later.
		* \param[in] const double x, y The new x and y coordinate of the mouse in screen coordinates.
		* \return void
		*/
		void DispatchMouseMoveEvent(const double x, const double y);

		/**
		* \brief Dispatches mouse button events from the callback.
		*
		* \param[in] const int button The button that the action occurred on.
		* \param[in] const int action The action that occurred.
		* \param[in] const int mods Modifier keys.
		* \return void
		*/
		void DispatchMouseButtonEvent(const int button, const int action, const int mods);

		GLFWwindow* window;
		int client_width, client_height; // Current window's client width and height.
		double old_mouse_x, old_mouse_y;
		double last_time; // The time at the last call to GetDeltaTime().
		bool mouse_lock; // If mouse lock is enabled causing the cursor to snap to mid-window each movement event.
	};
}
