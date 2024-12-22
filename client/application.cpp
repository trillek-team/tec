#include "application.hpp"
#include "default-config.hpp"
#include "net-message.hpp"

namespace tec {

std::shared_ptr<spdlog::logger> Application::log;

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

Application::Application() :
		game(os), active_entity_tooltip(game), server_connect_window(game.GetServerConnection()),
		ping_times_window(game.GetServerConnection()), debug_info_window(game) {
	log = spdlog::get("console_log");
	// TODO: Invert registration, so Console registers itself as a sink and remove itself in the destructor
	log->sinks().push_back(std::make_shared<tec::ConsoleSink>(console));

	InitializeWindow();
	InitializeGUI();
	InitializeConsoleCommands();

	auto lua_system = game.GetLuaSystem();
	lua_system->GetGlobalState().set("OS", &os);
	auto& server_connection = game.GetServerConnection();
	server_connection.RegisterMessageHandler(networking::CLIENT_ID, [this](networking::MessageIn& message) {
		std::string client_id = message.ToString();
		log->info("You are connected as client ID {}", client_id);
		gui.ShowWindow(ping_times_window.GetWindowName());
	});
	server_connection.RegisterMessageHandler(
			tec::networking::MessageType::AUTHENTICATED,
			[this](networking::MessageIn&) { gui.HideWindow(server_connect_window.GetWindowName()); });
}

Application::~Application() {
	log->info("Shutting down...");
	// TODO: Remove this when Console registers itself as a sink
	log->sinks().pop_back(); // Remove console sink
}

void Application::InitializeWindow() {
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
}

void Application::InitializeGUI() {
	log->info("Initializing GUI system...");
	gui.Initialize(os.GetWindow());
	gui.CreateGUI();
	gui.AddWindowDrawFunction(server_connect_window.GetWindowName(), [&]() { server_connect_window.Draw(&gui); });
	gui.ShowWindow(server_connect_window.GetWindowName());
	gui.AddWindowDrawFunction(ping_times_window.GetWindowName(), [&]() { ping_times_window.Draw(&gui); });
	gui.AddWindowDrawFunction(console.GetWindowName(), [&]() { console.Draw(&gui); });
	gui.ShowWindow(console.GetWindowName());
	gui.AddWindowDrawFunction(active_entity_tooltip.GetWindowName(), [&]() { active_entity_tooltip.Draw(&gui); });
	gui.ShowWindow(active_entity_tooltip.GetWindowName());
	gui.AddWindowDrawFunction(debug_info_window.GetWindowName(), [&]() { debug_info_window.Draw(&gui); });
}

void Application::InitializeConsoleCommands() {
	console.AddConsoleCommand("exit", "exit : Exit from TEC", [&](const std::string&) { os.Quit(); });
	game.Startup(console);

	console.AddConsoleCommand("msg", "msg : Send a message to all clients.", [&](const std::string& message) {
		game.GetServerConnection().SendChatMessage(message);
	});

	console.AddConsoleCommand(
			"connect",
			"connect [username][ip] : Connect to a server [ip] with the provided [username]",
			[&](const std::string& args) {
				std::vector<std::string> splitArgs = SplitString(args, " ");
				if (splitArgs.size() >= 2) {
					std::string username = splitArgs[0];
					std::string ip = splitArgs[1];
					spdlog::get("console_log")->info("Connecting to {}", ip);
					game.GetServerConnection().RegisterConnectFunc([&, username]() {
						proto::UserLogin user_login;
						user_login.set_username(username);
						user_login.set_password("");
						networking::MessageOut msg(networking::LOGIN);
						user_login.SerializeToZeroCopyStream(&msg);
						game.GetServerConnection().Send(msg);
					});
					game.GetServerConnection().Connect(ip);
				}
			});

	console.AddSlashHandler([&](const std::string& chat_command_message) {
		std::size_t argument_break_offset = chat_command_message.find_first_of(" ");
		std::shared_ptr<ChatCommandEvent> data = std::make_shared<ChatCommandEvent>();
		data->command = chat_command_message.substr(0, argument_break_offset);

		if (argument_break_offset < chat_command_message.size()) {
			std::string command_args = chat_command_message.substr(argument_break_offset + 1);
			data->args = SplitString(command_args);
		}
		EventSystem<ChatCommandEvent>::Get()->Emit(data);

		if (game.GetServerConnection().GetClientID() != 0) {
			proto::ChatCommand chat_command(data->Out());
			networking::MessageOut msg(networking::CHAT_COMMAND);
			chat_command.SerializeToZeroCopyStream(&msg);
			game.GetServerConnection().Send(msg);
		}
	});
}

void Application::Run() {
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
	OS::Terminate();
}

} // namespace tec
