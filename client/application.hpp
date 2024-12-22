#pragma once

#include <memory>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include "game.hpp"
#include "gui/active-entity-tooltip.hpp"
#include "gui/console.hpp"
#include "gui/debug-info.hpp"
#include "gui/server-connect.hpp"
#include "imgui-system.hpp"
#include "os.hpp"

namespace tec {
class Application {
public:
	Application();
	~Application();
	void Run();

private:
	void InitializeWindow();
	void InitializeGUI();
	void InitializeConsoleCommands();

	OS os;
	Game game;
	Console console;
	static std::shared_ptr<spdlog::logger> log;
	IMGUISystem gui;
	ActiveEntityTooltip active_entity_tooltip;
	ServerConnectWindow server_connect_window;
	PingTimesWindow ping_times_window;
	DebugInfo debug_info_window;
};
} // namespace tec
