// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include <iostream>
#include <string>
#include <thread>

#include "game.hpp"

#include "server-connection.hpp"
#include "server-message.hpp"
#include "filesystem.hpp"
#include "gui/console.hpp"
#include "gui/server-connect.hpp"
#include "gui/active-entity-tooltip.hpp"
#include "imgui-system.hpp"
#include "os.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

namespace tec {
	extern void InitializeFileFactories();
	extern void BuildTestVoxelVolume();
	extern void ProtoLoad(std::string filename);
}

auto InitializeLogger(spdlog::level::level_enum log_level, tec::Console& console) {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
	sinks.push_back(std::make_shared<tec::ConsoleSink>(console));
	auto log = std::make_shared<spdlog::logger>("console_log", begin(sinks), end(sinks));
	log->set_level(log_level);
	log->set_pattern("%v"); // [%l] [thread %t] %v"); // Format on stdout
	spdlog::register_logger(log);
	return log;
}

// TODO write a proper arguments parser
// Now only search for -v or -vv to set log level
auto ParseLogLevel(int argc, char* argv[]) {
	auto loglevel = spdlog::level::info;
	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "-v") {
			loglevel = spdlog::level::debug;
		}
		else if (std::string(argv[i]) == "-vv") {
			loglevel = spdlog::level::trace;
		}
	}
	return loglevel;
}

const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 768;
const std::string ASPECT_RATIO = "16:9";

int main(int argc, char* argv[]) {
	tec::Console console;

	auto log = InitializeLogger(ParseLogLevel(argc, argv), console);

	log->info(std::string("Asset path: ") + tec::FilePath::GetAssetsBasePath().toString());

	tec::Game game;

	const unsigned int window_width = game.config_script->environment.get_or("window_width", WINDOW_WIDTH);
	const unsigned int window_height = game.config_script->environment.get_or("window_height", WINDOW_HEIGHT);

	log->info("Initializing OpenGL...");
	tec::OS os;
	if (!os.InitializeWindow(window_width, window_height, "TEC 0.1", 4, 0)) {
		log->info("Exiting. The context wasn't created properly please update drivers and try again.");
		exit(1);
	}

	const std::string aspect_ratio = game.config_script->environment.get_or("aspect_ratio", ASPECT_RATIO);
	auto numer = stoi(aspect_ratio.substr(0, aspect_ratio.find(':')));
	auto denom = stoi(aspect_ratio.substr(aspect_ratio.find(':') + 1));
	os.SetWindowAspectRatio(numer, denom);
	console.AddConsoleCommand(
		"exit",
		"exit : Exit from TEC",
		[&os] (const char*) {
			os.Quit();
		});
	game.Startup();

	tec::ActiveEntityTooltip active_entity_tooltip(game);
	tec::networking::ServerConnection& connection = game.GetServerConnection();
	tec::ServerConnectWindow server_connect_window(connection);
	tec::PingTimesWindow ping_times_window(connection);

	console.AddConsoleCommand(
		"msg",
		"msg : Send a message to all clients.",
		[&connection] (const char* args) {
			const char* end_arg = args;
			while (*end_arg != '\0') {
				end_arg++;
			}
			// Args now points were the arguments begins
			std::string message(args, end_arg - args);
			connection.SendChatMessage(message);
		});

	log->info("Initializing GUI system...");
	tec::IMGUISystem gui(os.GetWindow());
	gui.CreateGUI();
	gui.AddWindowDrawFunction("connect_window", [&server_connect_window] () {server_connect_window.Draw(); });
	gui.AddWindowDrawFunction("ping_times", [&ping_times_window] () {ping_times_window.Draw(); });
	gui.AddWindowDrawFunction("console", [&console] () { console.Draw(); });
	gui.ShowWindow("console");
	gui.AddWindowDrawFunction("active_entity", [&active_entity_tooltip] () { active_entity_tooltip.Draw(); });
	gui.ShowWindow("active_entity");

	connection.RegisterMessageHandler(
		tec::networking::MessageType::CLIENT_ID,
		[&gui, &log] (const tec::networking::ServerMessage& message) {
			std::string client_id(message.GetBodyPTR(), message.GetBodyLength());
			log->info("You are connected as client ID " + client_id);
			gui.HideWindow("connect_window");
			gui.ShowWindow("ping_times");
		});

	tec::LuaSystem* lua_sys = game.GetLuaSystem();
	os.LuaStateRegistration(lua_sys->GetGlobalState());

	tec::InitializeFileFactories();
	tec::BuildTestVoxelVolume();
	tec::ProtoLoad("json/test.json");

	console.AddConsoleCommand(
		"lua",
		"lua : Execute a string in lua",
		[&lua_sys] (const char* args) {
			const char* end_arg = args;
			while (*end_arg != '\0') {
				end_arg++;
			}
			// Args now points were the arguments begins
			std::string message(args, end_arg - args);
			lua_sys->ExecuteString(message);
		});
	console.AddSlashHandler(
		[&lua_sys] (const char* args) {
			const char* end_arg = args;
			while (*end_arg != '\0') {
				end_arg++;
			}

			// TODO: Add processor for commands with arguments
			// TODO: Add check if command exists and report if it doesn't

			// Args now points were the arguments begins
			std::string message(args, end_arg - args);
			message = "OS:" + message + "()";
			lua_sys->ExecuteString(message);
		});

	os.DetachContext();

	std::thread gameThread(
		[&] () {
			os.MakeCurrent();

			double mouse_x, mouse_y;
			double delta;

			while (!os.Closing()) {
				delta = os.GetDeltaTime();

				tec::OS::GetMousePosition(&mouse_x, &mouse_y);

				game.Update(delta, mouse_x, mouse_y, os.GetWindowWidth(), os.GetWindowHeight());

				gui.Update(delta);
				console.Update(delta);
				os.SwapBuffers();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		});

	while (!os.Closing()) {
		os.OSMessageLoop();
	}

	gameThread.join();
	tec::OS::Terminate();

	return 0;
}
