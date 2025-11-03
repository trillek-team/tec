#pragma once

#include <memory>
#include <string>

#include <default-config.hpp>
#include <spdlog/spdlog.h>

#include "game.hpp"
#include "gui/active-entity-tooltip.hpp"
#include "gui/console.hpp"
#include "gui/debug-info.hpp"
#include "gui/server-connect.hpp"
#include "imgui-system.hpp"
#include "os.hpp"

namespace tec {

/**
 * @brief Main client application class that manages initialization and lifecycle
 */
class ClientApp {
public:
	ClientApp(int argc, char* argv[]);
	~ClientApp() = default;

	// Run the application main loop
	int Run();

private:
	// Initialization methods
	void InitializeLogger(spdlog::level::level_enum log_level);
	void InitializeWindow();
	void InitializeGame();
	void InitializeGUI();
	void RegisterConsoleCommands();
	void SetupAutoConnect();
	void RegisterGUIWindows();

	// Configuration
	spdlog::level::level_enum ParseLogLevel(int argc, char* argv[]);
	void LoadConfiguration();

	// Member variables
	Console console_;
	OS os_;
	std::shared_ptr<spdlog::logger> log_;
	std::unique_ptr<Game> game_;
	std::unique_ptr<IMGUISystem> gui_;

	// GUI windows
	std::unique_ptr<ActiveEntityTooltip> active_entity_tooltip_;
	std::unique_ptr<ServerConnectWindow> server_connect_window_;
	std::unique_ptr<PingTimesWindow> ping_times_window_;
	std::unique_ptr<DebugInfo> debug_info_window_;

	// Configuration values
	std::string default_username_;
	bool auto_connect_ = false;
	unsigned int window_width_ = WINDOW_WIDTH;
	unsigned int window_height_ = WINDOW_HEIGHT;
};

} // namespace tec
