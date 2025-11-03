#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <default-config.hpp>
#include <file-factories.hpp>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

#include "filesystem.hpp"
#include "game.hpp"
#include "gui/active-entity-tooltip.hpp"
#include "gui/console.hpp"
#include "gui/debug-info.hpp"
#include "gui/server-connect.hpp"
#include "imgui-system.hpp"
#include "net-message.hpp"
#include "os.hpp"
#include "resources/md5anim.hpp"
#include "resources/md5mesh.hpp"
#include "resources/obj.hpp"
#include "resources/script-file.hpp"
#include "resources/vorbis-stream.hpp"
#include "server-connection.hpp"

namespace tec {
void RegisterFileFactories() {
	AddFileFactory<MD5Mesh>();
	AddFileFactory<MD5Anim>();
	AddFileFactory<OBJ>();
	AddFileFactory<VorbisStream>();
	AddFileFactory<ScriptFile>();
}
void BuildTestVoxelVolume();
} // namespace tec

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
	auto log_level = spdlog::level::info;
	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "-v") {
			log_level = spdlog::level::debug;
		}
		else if (std::string(argv[i]) == "-vv") {
			log_level = spdlog::level::trace;
		}
	}
	return log_level;
}

/**
 * \brief Finds an approximate aspect ratio
 * Adapted from
 * https://www.geeksforgeeks.org/convert-given-decimal-number-into-an-irreducible-fraction/ Could be
 * further enhanced to compare values against a known set of ratios to find the best match \return
 * std:string The aspect ratio in the form of "A:B"
 */
std::string CalculateAspectRatioString(const unsigned int window_width, const unsigned int window_height) {
	double ratio = static_cast<double>(window_width) / static_cast<double>(window_height);
	double intVal = std::floor(ratio);
	double precision = 1000;
	double fraction = std::round((ratio - intVal) * precision);
	double gcd = std::gcd(static_cast<int>(fraction), static_cast<int>(precision));
	double numer = fraction / gcd;
	double denom = precision / gcd;

	std::ostringstream aspect_ratio;
	aspect_ratio << static_cast<int>(std::round((intVal * denom) + numer)) << ":"
				 << static_cast<int>(std::round(denom));
	return aspect_ratio.str();
}

int main(int argc, char* argv[]) {
	tec::Console console;
	tec::OS os;

	auto log = InitializeLogger(ParseLogLevel(argc, argv), console);

	log->info("Asset path: {}", tec::Path::GetAssetsBasePath());

	tec::Game game(os);

	const unsigned int window_width = game.config_script->environment.get_or("window_width", WINDOW_WIDTH);
	const unsigned int window_height = game.config_script->environment.get_or("window_height", WINDOW_HEIGHT);
	std::string window_title = "Trillek Engine 0.14";

	log->info("Initializing OpenGL...");
	if (!os.InitializeWindow(window_width, window_height, window_title, 4, 0)) {
		log->warn("The OpenGL 4.0 context wasn't created properly, attempting fallback");
		if (!os.InitializeWindow(window_width, window_height, window_title, 3, 3)) {
			log->error("Exiting. Can not create OpenGL 4.0 or 3.3 context. please update drivers and try again.");
			exit(1);
		}
	}

	const std::string default_aspect_ratio = CalculateAspectRatioString(window_width, window_height);
	const std::string aspect_ratio = game.config_script->environment.get_or("aspect_ratio", default_aspect_ratio);
	auto numer = stoi(aspect_ratio.substr(0, aspect_ratio.find(':')));
	auto denom = stoi(aspect_ratio.substr(aspect_ratio.find(':') + 1));
	os.SetWindowAspectRatio(numer, denom);
	console.AddConsoleCommand("exit", "exit : Exit from TEC", [&os](const std::string&) { os.Quit(); });
	game.Startup(console);

	tec::ActiveEntityTooltip active_entity_tooltip(game);
	tec::networking::ServerConnection& connection = game.GetServerConnection();
	tec::ServerConnectWindow server_connect_window(connection);
	tec::PingTimesWindow ping_times_window(connection);
	tec::DebugInfo debug_info_window(game);
	
	// Read configuration once for use in both GUI and auto-connect
	std::string default_username = game.config_script->environment.get_or("default_username", std::string(""));
	bool auto_connect = game.config_script->environment.get_or("auto_connect", false);
	tec::ServerConnectWindow::SetUsername(default_username);

	console.AddConsoleCommand("msg", "msg : Send a message to all clients.", [&connection](const std::string& message) {
		connection.SendChatMessage(message);
	});

	log->info("Initializing GUI system...");
	tec::IMGUISystem gui(os.GetWindow());
	gui.CreateGUI();
	gui.AddWindowDrawFunction(server_connect_window.GetWindowName(), [&server_connect_window, &gui]() {
		server_connect_window.Draw(&gui);
	});
	
	if (auto_connect && !default_username.empty()) {
		log->info("Auto-connecting to localhost as '{}'", default_username);
		
		// Register handlers before connecting to avoid race condition
		connection.RegisterConnectFunc([&connection, default_username, &log]() {
			tec::proto::UserLogin user_login;
			user_login.set_username(default_username);
			user_login.set_password("");
			tec::networking::MessageOut msg(tec::networking::LOGIN);
			user_login.SerializeToZeroCopyStream(&msg);
			connection.Send(msg);
			log->info("Auto-login sent for user '{}'", default_username);
		});
		
		// Hide the connect window on successful AUTHENTICATED message
		// Capture window_name by value to ensure it remains valid for the async handler
		std::string window_name = server_connect_window.GetWindowName();
		connection.RegisterMessageHandler(
				tec::networking::MessageType::AUTHENTICATED,
				[&gui, window_name, &log](tec::networking::MessageIn&) { 
					log->info("Auto-connect authentication successful");
					gui.HideWindow(window_name);
				});
		
		if (!connection.Connect(tec::networking::LOCAL_HOST)) {
			log->warn("Auto-connect failed, showing connect window");
			gui.ShowWindow(server_connect_window.GetWindowName());
		}
	} else {
		// Show connect window normally if auto-connect is disabled
		gui.ShowWindow(server_connect_window.GetWindowName());
	}
	
	gui.AddWindowDrawFunction(
			ping_times_window.GetWindowName(), [&ping_times_window, &gui]() { ping_times_window.Draw(&gui); });
	gui.AddWindowDrawFunction(console.GetWindowName(), [&console, &gui]() { console.Draw(&gui); });
	gui.ShowWindow(console.GetWindowName());
	gui.AddWindowDrawFunction(active_entity_tooltip.GetWindowName(), [&active_entity_tooltip, &gui]() {
		active_entity_tooltip.Draw(&gui);
	});
	gui.ShowWindow(active_entity_tooltip.GetWindowName());
	gui.AddWindowDrawFunction(
			debug_info_window.GetWindowName(), [&debug_info_window, &gui]() { debug_info_window.Draw(&gui); });

	connection.RegisterMessageHandler(
			tec::networking::MessageType::CLIENT_ID,
			[&gui, &ping_times_window, &log](tec::networking::MessageIn& message) {
				std::string client_id = message.ToString();
				log->info("You are connected as client ID {}", client_id);
				gui.ShowWindow(ping_times_window.GetWindowName());
			});

	tec::LuaSystem* lua_sys = game.GetLuaSystem();
	lua_sys->GetGlobalState().set("OS", &os); // register instance

	tec::RegisterFileFactories();
	tec::BuildTestVoxelVolume();

	console.AddConsoleCommand("lua", "lua : Execute a string in lua", [&lua_sys](const std::string& message) {
		lua_sys->ExecuteString(message);
	});
	console.AddConsoleCommand(
			"connect",
			"connect [username][ip] : Connect to a server [ip] with the provided [username]",
			[&connection](const std::string& args) {
				std::vector<std::string> splitArgs = tec::SplitString(args);
				if (splitArgs.size() >= 2) {
					connection.Connect(splitArgs[1]);
					std::string username = splitArgs[0];
					connection.RegisterConnectFunc([&connection, username]() {
						tec::proto::UserLogin user_login;
						user_login.set_username(username);
						user_login.set_password("");
						tec::networking::MessageOut msg(tec::networking::LOGIN);
						user_login.SerializeToZeroCopyStream(&msg);
						connection.Send(msg);
					});
				}
			});
	console.AddSlashHandler([&connection](const std::string& chat_command_message) {
		std::size_t argument_break_offset = chat_command_message.find_first_of(" ");
		std::shared_ptr<tec::ChatCommandEvent> data = std::make_shared<tec::ChatCommandEvent>();
		data->command = chat_command_message.substr(0, argument_break_offset);

		if (argument_break_offset < chat_command_message.size()) {
			std::string command_args = chat_command_message.substr(argument_break_offset + 1);
			data->args = tec::SplitString(command_args);
		}
		tec::EventSystem<tec::ChatCommandEvent>::Get()->Emit(data); // Handle command locally

		if (connection.GetClientID() != 0) { // If connected, send command to server
			tec::proto::ChatCommand chat_command(data->Out());
			tec::networking::MessageOut msg(tec::networking::CHAT_COMMAND);
			chat_command.SerializeToZeroCopyStream(&msg);
			connection.Send(msg);
		}
	});

	os.DetachContext();

	std::thread gameThread([&]() {
		os.MakeCurrent();

		while (!os.Closing()) {
			const double delta = os.GetDeltaTime();

			game.Update(delta);
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
