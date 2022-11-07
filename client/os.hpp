#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <vector>

#include "tec-types.hpp"

#include "event-system.hpp"

namespace tec {
struct KeyboardEvent;

class OS : public EventQueue<KeyboardEvent> {
public:
	/**
	* \brief Initialize the rendering window and makes the window's context the current one.
	*
	* \param[in] const unsigned int width, height The window's client width and height
	* \param[in] const std::string title The title to show in the title bar and task manager.
	* \param[in] const glMajor OpenGL major version number. Must be >= 3
	* \param[in] const glMinor OpenGL minor version. If major = 3, must be 3 (OpenGL 3.3)
	* \param[in] bool fullscreen Whether the window should be started in fullscreen mode.
	* \return bool If creation was successful or not.
	*/
	bool InitializeWindow(
			const int width,
			const int height,
			const std::string title,
			const int glMajor = 3,
			const int glMinor = 3,
			bool fullscreen = false);

	/** \brief Make the context of the window current for the calling thread
	*
	*/
	void MakeCurrent();

	/** \brief Sets the desired window aspect ratio numerator:denominator e.g. 16:9, 4:3
	*
	*/
	void SetWindowAspectRatio(const unsigned int numerator = 16, const unsigned int denominator = 9);

	/** \brief Toggles between fullscreen and windowed mode based.
	*
	*/
	void ToggleFullScreen();

	/** \brief Detach the context of the window from the calling thread
	*
	*/
	void DetachContext();

	/**
	* \brief Calls GLFWTerminate to close the window.
	*
	* This is a static method so it can be called from anywhere to terminate the current
	* window.
	* \return void
	*/
	static void Terminate();

	/**
	* \brief Tells the OS that the active window should close.
	*
	* Since the main loop is based on that close status of that active window
	* this effectively causes Closing() to return true during an upcoming message loop.
	* \return void
	*/
	void Quit();

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
	int GetWindowWidth() const;

	/**
	* \brief Gets the cached window height.
	*
	* \return int The window height.
	*/
	int GetWindowHeight() const;

	/**
	* \brief Returns the time since the start of the program.
	*
	* \return double The amount of time that have passed since the last call in seconds.
	*/
	double GetDeltaTime();

	/**
	* \brief Returns the time since initialization from glfw.
	*
	* \return double The amount of time since glfw was initialized in seconds.
	*/
	double GetTime();

	/**
	* \brief Returns the current active window.
	*
	* \return GLFWwindow* The current active window.
	*/
	GLFWwindow* GetWindow();

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
	* This is different from just a normal keyboard event as it has been translated and
	* modified by the OS and is just like typing into a text editor.
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
	* \brief Callback for mouse scroll events.
	*
	* \param[in] GLFWwindow* window
	* \param[in] double x, y The delta x and y of the mouse wheel.
	* \return void
	*/
	static void MouseScrollEventCallback(GLFWwindow* window, double x, double y);

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
	* \brief Callback for window focus change events.
	*
	* \param[in] GLFWwindow* window
	* \param[in] int count The number of files dropped.
	* \param[in] const char** paths Array of filenames.
	* \return void
	*/
	static void FileDropCallback(GLFWwindow* window, int count, const char** paths);

	void EnableMouseLock();

	void DisableMouseLock();

	/**
	* \brief Sets the mouse cursor position relative to the upper-left corner of the window.
	*
	* \param[in] double x, y The new x and y coordinate of the mouse in screen coordinates.
	* \return void
	*/
	static void SetMousePosition(const double x, const double y);

	/**
	* \brief Gets the mouse cursor position relative to the upper-left corner of the window.
	*
	* \param[out] double* x, y The current x and y coordinate of the mouse in screen coordinates.
	* \return void
	*/
	static void GetMousePosition(double* x, double* y);

	static const GLFWwindow* GetFocusedWindow() { return OS::focused_window; }

	static void RegisterLuaType(sol::state&);

private:
	/**
	* \brief Keyboard event listener, useful for events such as alt-enter to toggle fullscreen.
	*
	* \param[in] std::shared_ptr<KeyboardEvent> data Keyboard event data
	* \return void
	*/
	using EventQueue<KeyboardEvent>::On;
	void On(std::shared_ptr<KeyboardEvent> data);

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
	* \brief Dispatches mouse scroll events.
	*
	* It determines the changes in mouse position and stores the new position for later.
	* \param[in] const double xoffset, yoffset The delta x and y coordinate of the mouse wheel.
	* \return void
	*/
	void DispatchMouseScrollEvent(const double xoffset, const double yoffset);

	/**
	* \brief Dispatches mouse button events from the callback.
	*
	* \param[in] const int button The button that the action occurred on.
	* \param[in] const int action The action that occurred.
	* \param[in] const int mods Modifier keys.
	* \return void
	*/
	void DispatchMouseButtonEvent(const int button, const int action, const int mods);

	/**
	* \brief Dispatches a character event.
	*
	* \param[in] const int count The number of files dropped.
	* \param[in] const char** paths Array of filenames.
	* \return void
	*/
	void DispatchFileDropEvent(const int count, const char** paths);

	GLFWwindow* window{nullptr};
	static GLFWwindow* focused_window; // The window that currently has focus.
	int client_width{0}, client_height{0}; // Current window's client width and height.
	double old_mouse_x{0}, old_mouse_y{0};
	double last_time{0}; // The time at the last call to GetDeltaTime().
	bool mouse_lock{false}; // If mouse lock is requested.
	static bool mouse_locked; // If mouse lock is enabled causing the cursor to snap to
			// mid-window each movement event.
	bool fullscreen{false}; // False if the game is in windowed mode
};
} // namespace tec
