#include "os.hpp"
#include "events.hpp"
#include "filesystem.hpp"
#include "reflection.hpp"
#include "render-system.hpp"
#include "physics-system.hpp"
#include "voxelvolume.hpp"
#include "vcomputer-system.hpp"
#include "sound-system.hpp"
#include "imgui-system.hpp"
#include "simulation.hpp"
#include "component-update-system.hpp"
#include "controllers/fps-controller.hpp"

#include "gui/entity_tree.hpp"
#include "gui/console.hpp"

#include <spdlog/spdlog.h>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include "client/server_connection.hpp"

namespace tec {
	extern void InitializeComponents();
	extern void InitializeFileFactories();
	extern void BuildTestEntities();
	extern void ProtoSave();
	extern void ProtoLoad();
	extern std::map<std::string, std::function<void(std::string)>> file_factories;
	extern std::map<std::string, std::function<void(eid)>> component_factories;
	extern std::map<std::string, std::function<void(eid)>> component_removal_factories;

	ReflectionEntityList entity_list;
	eid active_entity;

	struct FileListener : public EventQueue < FileDropEvent > {
		void Update(double delta) {
			ProcessEventQueue();
		}

		void On(std::shared_ptr<FileDropEvent> fd_event) {
			auto _log = spdlog::get("console_log");
			for (auto file : fd_event->filenames) {
				FilePath path(file);
				if (!path.isValidPath() || !path.FileExists()) {
					_log->error() << "Can't find file : " << path.FileName();
					continue;
				}
				auto ext = path.FileExtension();
				if (ext.empty()) {
					_log->error() << "No extension!";
					continue;
				}
				if (path.isAbsolutePath()) {
					// We try to work always with relative paths to assets folder
					path = path.SubpathFrom("assets");
					auto fullpath = FilePath::GetAssetPath(path.toGenericString());
					if (!fullpath.isValidPath() || !fullpath.FileExists()) {
						_log->error() << "File isn't on assets folder! Please copy/move it to the assets folder.";
						continue;
					}

				}
				if (file_factories.find(ext) == file_factories.end()) {
					_log->warn() << "No loader for extension: " << ext;
					continue;
				}

				_log->info() << "Loading: " << path;
				file_factories[ext](path.toString());
			}
		}
	};

}
std::list<std::function<void(tec::frame_id_t)>> tec::ComponentUpdateSystemList::update_funcs;

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
	os.InitializeWindow(1024, 768, "TEC 0.1", 3, 3);
	console.AddConsoleCommand("exit",
		"exit : Exit from TEC",
		[&os] (const char* args) {
		os.Quit();
	});
	std::thread* asio_thread = nullptr;
	std::thread* sync_thread = nullptr;
	tec::networking::ServerConnection connection;
	console.AddConsoleCommand("msg",
		"msg : Send a message to all clients.",
		[&connection] (const char* args) {
		const char* end_arg = args;
		while (*end_arg != '\0') {
			end_arg++;
		}
		// Args now points were the arguments begins
		std::string message(args, end_arg - args);
		connection.Write(message);
	});
	asio_thread = new std::thread([&connection] () {
		connection.StartRead();
	});
	sync_thread = new std::thread([&connection] () {
		connection.StartSync();
	});
	console.AddConsoleCommand("connect",
		"connect ip : Connects to the server at ip",
		[&connection] (const char* args) {
		const char* end_arg = args;
		while (*end_arg != '\0' && *end_arg != ' ') {
			end_arg++;
		}
		// Args now points were the arguments begins
		std::string ip(args, end_arg - args);
		connection.Connect(ip);
	});

	log->info("Initializing GUI system...");
	tec::IMGUISystem gui(os.GetWindow());
	tec::EntityTree ent_tree_widget;

	log->info("Initializing rendering system...");
	tec::RenderSystem rs;
	rs.SetViewportSize(os.GetWindowWidth(), os.GetWindowHeight());

	log->info("Initializing simulation system...");
	tec::Simulation simulation;
	tec::PhysicsSystem& ps = simulation.GetPhysicsSystem();

	log->info("Initializing sound system...");
	tec::SoundSystem ss;

	std::int64_t frame_id = 1;

	log->info("Initializing voxel system...");
	tec::VoxelSystem vox_sys;

	tec::BuildTestEntities();
	tec::ProtoLoad();
	tec::FileListener flistener;

	tec::FPSController* camera_controller = nullptr;
	gui.AddWindowDrawFunction("connect_window", [&camera_controller, &connection, &log, &gui] () {
		ImGui::SetNextWindowPosCenter();
		ImGui::Begin("Connect to Server", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

		static int octets[4] = {123, 123, 123, 123};

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
			if (connection.Connect(ip.str())) {
				std::thread on_connect([&connection, &camera_controller, &log] () {
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
				});
				on_connect.detach();
				gui.HideWindow("connect_window");
			}
			else {
				log->error("Failed to connect to " + ip.str());
			}
		}
		ImGui::End();
		ImGui::SetWindowSize("Connect to Server", ImVec2(0, 0));
	});

	gui.AddWindowDrawFunction("sample_window", [ ] () {
		ImGui::ShowTestWindow();
	});

	gui.AddWindowDrawFunction("active_entity", [ ] () {
		if (tec::active_entity != 0) {
			ImGui::SetTooltip("#%i", tec::active_entity);
		}
	});
	gui.ShowWindow("active_entity");
	gui.AddWindowDrawFunction("main_menu", [&os, &connection, &gui] () {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Load PROTO", "CTRL+L")) { }
				if (ImGui::MenuItem("Reload PROTO", "CTRL+R")) {
					//tec::ProtoLoad();
				}
				if (ImGui::MenuItem("Save PROTO", "CTRL+S")) {
					tec::ProtoSave();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Quit", "Alt+F4")) {
					tec::ProtoSave();
					os.Quit();
				}
				if (ImGui::MenuItem("Quit w/o Saving", "Alt+F4")) {
					os.Quit();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo", "CTRL+Z")) { }
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) { }  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) { }
				if (ImGui::MenuItem("Copy", "CTRL+C")) { }
				if (ImGui::MenuItem("Paste", "CTRL+V")) { }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Windows")) {
				bool visible = gui.IsWindowVisible("connect_window");
				if (ImGui::MenuItem("Connect", "", visible)) {
					if (visible) {
						gui.HideWindow("connect_window");
					}
					else {
						gui.ShowWindow("connect_window");
					}
				}
				visible = gui.IsWindowVisible("entity_tree");
				if (ImGui::MenuItem("Entity Tree", "", gui.IsWindowVisible("entity_tree"))) {
					if (visible) {
						gui.HideWindow("entity_tree");
					}
					else {
						gui.ShowWindow("entity_tree");
					}
				}
				ImGui::EndMenu();
			}
			ImGui::Text("Ping %i", connection.GetAveragePing());
			ImGui::EndMainMenuBar();
		}
	});
	gui.ShowWindow("main_menu");
	gui.AddWindowDrawFunction("ping_times", [&connection] () {
		ImGui::Begin("ping_times");
		static float arr[10];
		std::list<tec::networking::ping_time_t> recent_pings = connection.GetRecentPings();
		std::size_t i = 0;
		for (tec::networking::ping_time_t ping : recent_pings) {
			arr[i++] = static_cast<float>(ping);
		}
		ImGui::PlotHistogram("Recent Ping Times", arr, 10, 0, nullptr, 0.0f, 100.0f);
		ImGui::End();
	});
	gui.AddWindowDrawFunction("entity_tree", [&ent_tree_widget] () {
		ent_tree_widget.Draw();
	});

	gui.AddWindowDrawFunction("console", [&console] () {
		console.Draw();
	});
	gui.ShowWindow("console");

	double delta = os.GetDeltaTime();
	double mouse_x, mouse_y;
	while (!os.Closing()) {
		os.OSMessageLoop();
		delta = os.GetDeltaTime();

		tec::ComponentUpdateSystemList::UpdateAll(frame_id);

		std::thread ss_thread([&] () {
			ss.Update(delta);
		});
		std::thread vv_thread([&] () {
			vox_sys.Update(delta);
		});

		flistener.Update(delta);
		simulation.Simulate(delta);

		std::map<tec::eid, std::map<tec::tid, tec::proto::Component>>&& results = simulation.GetResults();
		if (results.find(connection.GetClientID()) != results.end()) {
			tec::proto::Entity entity;
			entity.set_id(connection.GetClientID());
			for (const auto& compoennt_update : results.at(connection.GetClientID())) {
				tec::proto::Component* comp = entity.add_components();
				*comp = compoennt_update.second;
			}
			tec::networking::ServerMessage msg;
			msg.SetBodyLength(entity.ByteSize());
			entity.SerializeToArray(msg.GetBodyPTR(), msg.GetBodyLength());
			msg.SetMessageType(tec::networking::ENTITY_UPDATE);
			msg.encode_header();
			connection.Send(msg);
		}

		rs.Update(delta);

		ss_thread.join();
		vv_thread.join();

		if (camera_controller) {
			camera_controller->Update(delta);
		}

		os.GetMousePosition(&mouse_x, &mouse_y);
		tec::active_entity = ps.RayCastMousePick(1, mouse_x, mouse_y,
			static_cast<float>(os.GetWindowWidth()), static_cast<float>(os.GetWindowHeight()));
		//ps.DebugDraw();

		gui.Update(delta);
		console.Update(delta);

		os.SwapBuffers();
		frame_id++;
	}

	connection.Disconnect();
	connection.StopRead();
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
