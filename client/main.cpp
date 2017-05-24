// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "client/server-connection.hpp"
#include "controllers/fps-controller.hpp"
#include "events.hpp"
#include "filesystem.hpp"
#include "gui/console.hpp"
#include "imgui-system.hpp"
#include "lua-system.hpp"
#include "os.hpp"
#include "graphics/view.hpp"
#include "physics-system.hpp"
#include "render-system.hpp"
#include "simulation.hpp"
#include "game-state-queue.hpp"
#include "sound-system.hpp"
#include "vcomputer-system.hpp"
#include "voxel-volume.hpp"

#include <future>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <thread>

namespace tec {
	extern void InitializeComponents();
	extern void InitializeFileFactories();
	extern void BuildTestEntities();
	extern void ProtoLoad();
	eid active_entity;
}

int main(int argc, char* argv[]) {
	auto loglevel = spdlog::level::info;

	tec::InitializeComponents();
	tec::InitializeFileFactories();
	// TODO write a proper arguments parser
	// Now only search for -v or -vv to set log level
	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]).compare("-v")) {
			loglevel = spdlog::level::debug;
		}
		else if (std::string(argv[i]).compare("-vv")) {
			loglevel = spdlog::level::trace;
		}
	}
	// Console and logging initialization
	tec::Console console;

	spdlog::set_async_mode(1048576);
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
	sinks.push_back(std::make_shared<tec::ConsoleSink>(console));
	auto log = std::make_shared<spdlog::logger>("console_log", begin(sinks), end(sinks));
	log->set_level(loglevel);
	log->set_pattern("%v"); // [%l] [thread %t] %v"); // Format on stdout
	spdlog::register_logger(log);

	log->info("Initializing OpenGL...");
	tec::OS os;
	if (!os.InitializeWindow(1024, 768, "TEC 0.1", 3, 1)) {
		log->info("Exiting. The context wasn't created properly please update drivers and try again.");
		exit(1);
	}
	console.AddConsoleCommand("exit",
		"exit : Exit from TEC",
		[&os](const char* args) {
		os.Quit();
	});
	std::thread* asio_thread = nullptr;
	std::thread* sync_thread = nullptr;
	tec::Simulation simulation;
	tec::GameStateQueue game_state_queue;
	tec::networking::ServerConnection connection;
	console.AddConsoleCommand("msg",
		"msg : Send a message to all clients.",
		[&connection](const char* args) {
		const char* end_arg = args;
		while (*end_arg != '\0') {
			end_arg++;
		}
		// Args now points were the arguments begins
		std::string message(args, end_arg - args);
		connection.SendChatMessage(message);
	});
	console.AddConsoleCommand("connect",
		"connect ip : Connects to the server at ip",
		[&connection](const char* args) {
		const char* end_arg = args;
		while (*end_arg != '\0' && *end_arg != ' ') {
			end_arg++;
		}
		// Args now points were the arguments begins
		std::string ip(args, end_arg - args);
		connection.Connect(ip);
	});
	log->info(std::string("Loading assets from: ") + tec::FilePath::GetAssetsBasePath());

	log->info("Initializing GUI system...");
	tec::IMGUISystem gui(os.GetWindow());

	log->info("Initializing rendering system...");
	tec::RenderSystem rs;
	rs.SetViewportSize(os.GetWindowWidth(), os.GetWindowHeight());

	log->info("Initializing simulation system...");
	tec::PhysicsSystem& ps = simulation.GetPhysicsSystem();
	tec::VComputerSystem vcs;

	log->info("Initializing sound system...");
	tec::SoundSystem ss;

	log->info("Initializing voxel system...");
	tec::VoxelSystem vox_sys;

	log->info("Initializing script system...");
	tec::LuaSystem lua_sys;

	tec::BuildTestEntities();
	tec::ProtoLoad();

	tec::FPSController* camera_controller = nullptr;
	gui.AddWindowDrawFunction("connect_window", [&]() {
		ImGui::SetNextWindowPosCenter();
		ImGui::Begin("Connect to Server", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

		static int octets[4] = { 127, 0, 0, 1 };

		float width = ImGui::CalcItemWidth();
		ImGui::PushID("IP");
		ImGui::AlignFirstTextHeightToWidgets();
		ImGui::TextUnformatted("IP");
		ImGui::SameLine();
		for (int i = 0; i < 4; i++) {
			ImGui::PushItemWidth(width / 4.0f);
			ImGui::PushID(i);

			bool invalid_octet = false;
			if (octets[i] > 255) {
				octets[i] = 255;
				invalid_octet = true;
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
			if (octets[i] < 0) {
				octets[i] = 0;
				invalid_octet = true;
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
			}
			ImGui::InputInt("##v", &octets[i], 0, 0, ImGuiInputTextFlags_CharsDecimal);
			if (invalid_octet) {
				ImGui::PopStyleColor();
			}
			ImGui::SameLine();
			ImGui::PopID();
			ImGui::PopItemWidth();
		}
		ImGui::PopID();
		ImGui::SameLine();
		if (ImGui::Button("Connect")) {
			std::stringstream ip;
			ip << octets[0] << "." << octets[1] << "." << octets[2] << "." << octets[3];
			log->info("Connecting to " + ip.str());
			connection.Disconnect();
			if (connection.Connect(ip.str())) {
				std::thread on_connect([&simulation, &connection, &camera_controller, &log]() {
					unsigned int tries = 0;
					while (connection.GetClientID() == 0) {
						tries++;
						if (tries < 2000) {
							std::this_thread::sleep_for(std::chrono::milliseconds(1));
						}
						else {
							log->error("Failed to get client ID!");
							return;
						}
					}
					log->info("You are connected as client ID " + std::to_string(connection.GetClientID()));
					camera_controller = new tec::FPSController(connection.GetClientID());
					tec::Entity camera(connection.GetClientID());
					camera.Add<tec::Velocity>();
					camera.Add<tec::View>(true);
					simulation.AddController(camera_controller);
				});
				on_connect.detach();
				gui.HideWindow("connect_window");

				asio_thread = new std::thread([&connection]() {
					connection.StartRead();
				});
				sync_thread = new std::thread([&connection]() {
					connection.StartSync();
				});
			}
			else {
				log->error("Failed to connect to " + ip.str());
			}
		}
		ImGui::End();
		ImGui::SetWindowSize("Connect to Server", ImVec2(0, 0));
	});

	gui.AddWindowDrawFunction("sample_window", []() {
		ImGui::ShowTestWindow();
	});

	gui.AddWindowDrawFunction("active_entity", []() {
		if (tec::active_entity != 0) {
			ImGui::SetTooltip("#%" PRI_EID, tec::active_entity);
		}
	});
	gui.ShowWindow("active_entity");
	gui.AddWindowDrawFunction("main_menu", [&os, &connection, &gui]() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Connect")) {
				bool visible = gui.IsWindowVisible("connect_window");
				if (ImGui::MenuItem("Connect to server...", "", visible)) {
					if (visible) {
						gui.HideWindow("connect_window");
					}
					else {
						gui.ShowWindow("connect_window");
					}
				}
				ImGui::EndMenu();
			}
			ImGui::Text("Ping %" PRI_PING_TIME_T, connection.GetAveragePing());
			ImGui::EndMainMenuBar();
		}
	});
	gui.ShowWindow("main_menu");
	gui.AddWindowDrawFunction("ping_times", [&connection]() {
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::Begin("ping_times", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
		static float arr[10];
		std::list<tec::networking::ping_time_t> recent_pings = connection.GetRecentPings();
		std::size_t i = 0;
		for (tec::networking::ping_time_t ping : recent_pings) {
			arr[i++] = static_cast<float>(ping);
		}
		ImGui::PlotHistogram("Ping", arr, 10, 0, nullptr, 0.0f, 100.0f);
		ImGui::SetWindowPos("ping_times", ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::GetWindowSize().x - 10, 20));
		ImGui::End();
		ImGui::SetWindowSize("ping_times", ImVec2(0, 0));
		ImGui::PopStyleColor();
	});
	gui.ShowWindow("ping_times");

	gui.AddWindowDrawFunction("console", [&console]() {
		console.Draw();
	});
	gui.ShowWindow("console");

	double delta = os.GetDeltaTime();
	double mouse_x, mouse_y;

	std::thread ss_thread([&]() {
		ss.Update();
	});

	double delta_accumulator = 0.0; // Accumulated deltas since the last update was sent.
	while (!os.Closing()) {
		os.OSMessageLoop();
		delta = os.GetDeltaTime();
		delta_accumulator += delta;

		ss.SetDelta(delta);
		/*std::async(std::launch::async, [&vox_sys, delta] () {
			vox_sys.Update(delta);
		});*/

		game_state_queue.Interpolate(delta);

		auto client_state = simulation.Simulate(delta, game_state_queue.GetInterpolatedState());
		if (delta_accumulator >= tec::UPDATE_RATE / 2.0) {
			if (camera_controller) {
				tec::networking::ServerMessage update_message;
				tec::proto::ClientCommands client_commands = camera_controller->GetClientCommands();
				update_message.SetStateID(connection.GetLastRecvStateID());
				update_message.SetMessageType(tec::networking::CLIENT_COMMAND);
				client_commands.SerializeToArray(update_message.GetBodyPTR(), client_commands.ByteSize());
				update_message.SetBodyLength(client_commands.ByteSize());
				update_message.encode_header();
				connection.Send(update_message);
			}

			delta_accumulator -= tec::UPDATE_RATE / 2.0;
		}

		vcs.Update(delta);

		rs.Update(delta, client_state);

		lua_sys.Update(delta);

		os.GetMousePosition(&mouse_x, &mouse_y);
		tec::active_entity = ps.RayCastMousePick(connection.GetClientID(), mouse_x, mouse_y,
			static_cast<float>(os.GetWindowWidth()), static_cast<float>(os.GetWindowHeight()));
		//ps.DebugDraw();
		if (camera_controller != nullptr) {
			if (camera_controller->mouse_look) {
				os.EnableMouseLock();
			}
			else {
				os.DisableMouseLock();
			}
		}

		gui.Update(delta);
		console.Update(delta);
		os.SwapBuffers();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	ss.Stop();
	ss_thread.join();
	connection.Disconnect();
	connection.Stop();
	if (asio_thread) {
		asio_thread->join();
	}
	if (sync_thread) {
		sync_thread->join();
	}

	if (camera_controller) {
		delete camera_controller;
	}

	return 0;
}
