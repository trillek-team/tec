#include "client-app.hpp"

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <default-config.hpp>
#include <file-factories.hpp>
#include <cmath>
#include <numeric>
#include <sstream>
#include <thread>

#include "events.hpp"
#include "filesystem.hpp"
#include "net-message.hpp"
#include "resources/md5anim.hpp"
#include "resources/md5mesh.hpp"
#include "resources/obj.hpp"
#include "resources/script-file.hpp"
#include "resources/vorbis-stream.hpp"
#include "server-connection.hpp"

namespace tec {

namespace {

void RegisterFileFactories() {
	AddFileFactory<MD5Mesh>();
	AddFileFactory<MD5Anim>();
	AddFileFactory<OBJ>();
	AddFileFactory<VorbisStream>();
	AddFileFactory<ScriptFile>();
}

std::string CalculateAspectRatioString(unsigned int window_width, unsigned int window_height) {
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

} // anonymous namespace

ClientApp::ClientApp(int argc, char* argv[]) {
	auto log_level = ParseLogLevel(argc, argv);
	InitializeLogger(log_level);

	log_->info("Asset path: {}", Path::GetAssetsBasePath());

	game_ = std::make_unique<Game>(os_);
	LoadConfiguration();
	InitializeWindow();
	InitializeGame();
	InitializeGUI();
	RegisterConsoleCommands();
	SetupAutoConnect();
	RegisterGUIWindows();

	RegisterFileFactories();
	BuildTestVoxelVolume();
}

spdlog::level::level_enum ClientApp::ParseLogLevel(int argc, char* argv[]) {
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

void ClientApp::InitializeLogger(spdlog::level::level_enum log_level) {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
	sinks.push_back(std::make_shared<ConsoleSink>(console_));
	log_ = std::make_shared<spdlog::logger>("console_log", begin(sinks), end(sinks));
	log_->set_level(log_level);
	log_->set_pattern("%v");
	spdlog::register_logger(log_);
}

void ClientApp::LoadConfiguration() {
	window_width_ = game_->config_script->environment.get_or("window_width", WINDOW_WIDTH);
	window_height_ = game_->config_script->environment.get_or("window_height", WINDOW_HEIGHT);
	default_username_ = game_->config_script->environment.get_or("default_username", std::string(""));
	auto_connect_ = game_->config_script->environment.get_or("auto_connect", false);
}

void ClientApp::InitializeWindow() {
	log_->info("Initializing OpenGL...");
	if (!os_.InitializeWindow(window_width_, window_height_, "Trillek Engine 0.14", 4, 0)) {
		log_->warn("The OpenGL 4.0 context wasn't created properly, attempting fallback");
		if (!os_.InitializeWindow(window_width_, window_height_, "Trillek Engine 0.14", 3, 3)) {
			log_->error("Exiting. Can not create OpenGL 4.0 or 3.3 context. please update drivers and try again.");
			exit(1);
		}
	}

	const std::string default_aspect_ratio = CalculateAspectRatioString(window_width_, window_height_);
	const std::string aspect_ratio = game_->config_script->environment.get_or("aspect_ratio", default_aspect_ratio);
	auto numer = stoi(aspect_ratio.substr(0, aspect_ratio.find(':')));
	auto denom = stoi(aspect_ratio.substr(aspect_ratio.find(':') + 1));
	os_.SetWindowAspectRatio(numer, denom);
}

void ClientApp::InitializeGame() {
	console_.AddConsoleCommand("exit", "exit : Exit from TEC", [this](const std::string&) { os_.Quit(); });
	game_->Startup(console_);
}

void ClientApp::InitializeGUI() {
	log_->info("Initializing GUI system...");
	gui_ = std::make_unique<IMGUISystem>(os_.GetWindow());
	gui_->CreateGUI();

	// Create GUI windows
	auto& connection = game_->GetServerConnection();
	active_entity_tooltip_ = std::make_unique<ActiveEntityTooltip>(*game_);
	server_connect_window_ = std::make_unique<ServerConnectWindow>(connection);
	ping_times_window_ = std::make_unique<PingTimesWindow>(connection);
	debug_info_window_ = std::make_unique<DebugInfo>(*game_);

	ServerConnectWindow::SetUsername(default_username_);
}

void ClientApp::RegisterConsoleCommands() {
	auto& connection = game_->GetServerConnection();
	auto* lua_sys = game_->GetLuaSystem();

	console_.AddConsoleCommand("msg", "msg : Send a message to all clients.", 
		[&connection](const std::string& message) {
			connection.SendChatMessage(message);
		});

	console_.AddConsoleCommand("lua", "lua : Execute a string in lua", 
		[lua_sys](const std::string& message) {
			lua_sys->ExecuteString(message);
		});

	console_.AddConsoleCommand(
		"connect",
		"connect [username][ip] : Connect to a server [ip] with the provided [username]",
		[&connection](const std::string& args) {
			std::vector<std::string> splitArgs = SplitString(args);
			if (splitArgs.size() >= 2) {
				connection.Connect(splitArgs[1]);
				std::string username = splitArgs[0];
				connection.RegisterConnectFunc([&connection, username]() {
					proto::UserLogin user_login;
					user_login.set_username(username);
					user_login.set_password("");
					networking::MessageOut msg(networking::LOGIN);
					user_login.SerializeToZeroCopyStream(&msg);
					connection.Send(msg);
				});
			}
		});

	console_.AddSlashHandler([&connection](const std::string& chat_command_message) {
		std::size_t argument_break_offset = chat_command_message.find_first_of(" ");
		auto data = std::make_shared<ChatCommandEvent>();
		data->command = chat_command_message.substr(0, argument_break_offset);

		if (argument_break_offset < chat_command_message.size()) {
			std::string command_args = chat_command_message.substr(argument_break_offset + 1);
			data->args = SplitString(command_args);
		}
		EventSystem<ChatCommandEvent>::Get()->Emit(data);

		if (connection.GetClientID() != 0) {
			proto::ChatCommand chat_command(data->Out());
			networking::MessageOut msg(networking::CHAT_COMMAND);
			chat_command.SerializeToZeroCopyStream(&msg);
			connection.Send(msg);
		}
	});

	lua_sys->GetGlobalState().set("OS", &os_);
}

void ClientApp::SetupAutoConnect() {
	auto& connection = game_->GetServerConnection();

	if (auto_connect_ && !default_username_.empty()) {
		log_->info("Auto-connecting to localhost as '{}'", default_username_);

		// Register handlers before connecting to avoid race condition
		connection.RegisterConnectFunc([&connection, username = default_username_, log = log_]() {
			proto::UserLogin user_login;
			user_login.set_username(username);
			user_login.set_password("");
			networking::MessageOut msg(networking::LOGIN);
			user_login.SerializeToZeroCopyStream(&msg);
			connection.Send(msg);
			log->info("Auto-login sent for user '{}'", username);
		});

		// Hide the connect window on successful AUTHENTICATED message
		std::string window_name = server_connect_window_->GetWindowName();
		connection.RegisterMessageHandler(
			networking::MessageType::AUTHENTICATED,
			[gui = gui_.get(), window_name, log = log_](networking::MessageIn&) {
				log->info("Auto-connect authentication successful");
				gui->HideWindow(window_name);
			});

		if (!connection.Connect(networking::LOCAL_HOST)) {
			log_->warn("Auto-connect failed, showing connect window");
			gui_->ShowWindow(server_connect_window_->GetWindowName());
		}
	}
	else {
		gui_->ShowWindow(server_connect_window_->GetWindowName());
	}
}

void ClientApp::RegisterGUIWindows() {
	auto& connection = game_->GetServerConnection();

	gui_->AddWindowDrawFunction(server_connect_window_->GetWindowName(),
		[window = server_connect_window_.get(), gui = gui_.get()]() {
			window->Draw(gui);
		});

	gui_->AddWindowDrawFunction(ping_times_window_->GetWindowName(),
		[window = ping_times_window_.get(), gui = gui_.get()]() {
			window->Draw(gui);
		});

	gui_->AddWindowDrawFunction(console_.GetWindowName(),
		[console = &console_, gui = gui_.get()]() {
			console->Draw(gui);
		});
	gui_->ShowWindow(console_.GetWindowName());

	gui_->AddWindowDrawFunction(active_entity_tooltip_->GetWindowName(),
		[window = active_entity_tooltip_.get(), gui = gui_.get()]() {
			window->Draw(gui);
		});
	gui_->ShowWindow(active_entity_tooltip_->GetWindowName());

	gui_->AddWindowDrawFunction(debug_info_window_->GetWindowName(),
		[window = debug_info_window_.get(), gui = gui_.get()]() {
			window->Draw(gui);
		});

	connection.RegisterMessageHandler(
		networking::MessageType::CLIENT_ID,
		[gui = gui_.get(), window = ping_times_window_.get(), log = log_](networking::MessageIn& message) {
			std::string client_id = message.ToString();
			log->info("You are connected as client ID {}", client_id);
			gui->ShowWindow(window->GetWindowName());
		});
}

int ClientApp::Run() {
	os_.DetachContext();

	std::thread gameThread([this]() {
		os_.MakeCurrent();

		while (!os_.Closing()) {
			const double delta = os_.GetDeltaTime();

			game_->Update(delta);
			gui_->Update(delta);
			console_.Update(delta);
			os_.SwapBuffers();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	});

	while (!os_.Closing()) {
		os_.OSMessageLoop();
	}

	gameThread.join();
	OS::Terminate();

	return 0;
}

} // namespace tec
