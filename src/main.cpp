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
	os.InitializeWindow(1024, 768, "TEC 0.1", 3, 2);
	console.AddConsoleCommand("exit",
		"exit : Exit from TEC",
		[&os] (const char* args) {
		os.Quit();
	});
	std::thread* asio_thread = nullptr;
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
	connection.Connect();
	asio_thread = new std::thread([&connection] () {
		connection.StartRead();
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

	log->info("Initializing physics system...");
	tec::PhysicsSystem ps;

	log->info("Initializing sound system...");
	tec::SoundSystem ss;

	std::int64_t frame_id = 1;

	log->info("Initializing virtual computer system...");
	tec::VComputerSystem vcs;

	log->info("Initializing voxel system...");
	tec::VoxelSystem vox_sys;

	tec::InitializeComponents();
	tec::InitializeFileFactories();
	tec::BuildTestEntities();
	tec::ProtoLoad();

	tec::FPSController camera_controller(1);

	tec::FileListener flistener;

	gui.AddWindowDrawFunction("sample_window", [ ] () {
		ImGui::ShowTestWindow();
	});

	gui.AddWindowDrawFunction("active_entity", [ ] () {
		if (tec::active_entity != 0) {
			ImGui::SetTooltip("#%i", tec::active_entity);
		}
	});
	gui.AddWindowDrawFunction("main_menu", [&os] () {
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
			ImGui::EndMainMenuBar();
		}
	});
	gui.AddWindowDrawFunction("entity_tree", [&ent_tree_widget] () {
		ent_tree_widget.Draw();
	});



	gui.AddWindowDrawFunction("console", [&console] () {
		console.Draw();
	});

	double delta = os.GetDeltaTime();
	double mouse_x, mouse_y;
	while (!os.Closing()) {
		os.OSMessageLoop();
		delta = os.GetDeltaTime();

		tec::ComponentUpdateSystemList::UpdateAll(frame_id);

		flistener.Update(delta);
		std::thread ps_thread([&] () {
			ps.Update(delta);
		});
		std::thread ss_thread([&] () {
			ss.Update(delta);
		});
		std::thread vv_thread([&] () {
			vox_sys.Update(delta);
		});

		rs.Update(delta);

		vcs.Update(delta);

		ps_thread.join();
		ss_thread.join();
		vv_thread.join();

		camera_controller.Update(delta);

		os.GetMousePosition(&mouse_x, &mouse_y);
		tec::active_entity = ps.RayCastMousePick(1, mouse_x, mouse_y,
			static_cast<float>(os.GetWindowWidth()), static_cast<float>(os.GetWindowHeight()));
		ps.DebugDraw();

		gui.Update(delta);
		console.Update(delta);

		os.SwapBuffers();
		if (camera_controller.mouse_look) {
			//os.SetMousePosition(400, 300);
		}
		frame_id++;
	}

	connection.Disconnect();
	connection.StopRead();
	if (asio_thread) {
		asio_thread->join();
	}
	return 0;
}
