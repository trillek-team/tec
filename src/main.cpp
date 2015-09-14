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
#include "gui/console.hpp"
#include "spdlog/spdlog.h"

#include <iostream>
#include <thread>
#include <string>
#include <sstream>

namespace tec {
	extern void IntializeComponents();
	extern void IntializeFileFactories();
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
		} else if (std::string(argv[i]).compare("-vv")) {
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
	console.AddConsoleCommand( "exit", 
		"exit : Exit from TEC",
		[&os ] (const char* args) {
			os.Quit();
		});
	
	log->info("Initializing GUI system...");
	tec::IMGUISystem gui(os.GetWindow());

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

	tec::IntializeComponents();
	tec::IntializeFileFactories();
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
	gui.AddWindowDrawFunction("entity_tree", [ ] () {
		static bool no_titlebar = false;
		static bool no_border = true;
		static bool no_resize = false;
		static bool no_move = false;
		static bool no_scrollbar = false;
		static bool no_collapse = true;
		static bool no_menu = true;
		static int current_combo_item_slot = 0;
		const int MAX_COMBO_ITEM_SLOTS = 10;
		static int current_combo_item[MAX_COMBO_ITEM_SLOTS] = {0};
		static float bg_alpha = 0.65f;

		// Demonstrate the various window flags. Typically you would just use the default.
		ImGuiWindowFlags window_flags = 0;
		if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
		if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
		if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
		if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
		if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
		if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
		if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
		bool opened = true;
		if (ImGui::Begin("Entity Tree", &opened, ImVec2(550, 680), bg_alpha, window_flags)) {
			if (ImGui::Button("Add")) {
				ImGui::OpenPopup("AddEntity");
			}
			if (ImGui::BeginPopup("AddEntity")) {
				ImGui::Text("ID");
				ImGui::SameLine();
				static int new_entity_id = 0;
				ImGui::InputInt("##labellessInputInt", &new_entity_id);
				if (!ImGui::IsWindowFocused() && new_entity_id != 0) {
					if (tec::entity_list.entities.find(new_entity_id) == tec::entity_list.entities.end()) {
						tec::ReflectionEntity refentity;
						tec::entity_list.entities[new_entity_id] = std::move(refentity);
					}
					else {
						ImGui::OpenPopup("EntityExists");
						if (ImGui::BeginPopup("EntityExists")) {
							ImGui::Text("Entity with that ID exists");
							ImGui::SameLine();
							if (ImGui::Button("Ok")) {
								new_entity_id = 0;
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
					}
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove")) {
				ImGui::OpenPopup("RemoveEntity");
			}
			if (ImGui::BeginPopup("RemoveEntity")) {
				for (auto itr = tec::entity_list.entities.begin(); itr != tec::entity_list.entities.end(); ++itr) {
					if (ImGui::Selectable(std::to_string(itr->first).c_str())) {
						for (auto factory : tec::component_removal_factories) {
							factory.second(itr->first);
						}
						tec::entity_list.entities.erase(itr);
						break;
					}
				}
				ImGui::EndPopup();
			}
			if (ImGui::TreeNode("Entities")) {
				current_combo_item_slot = 0;
				for (auto&& entity : tec::entity_list.entities) {
					if (ImGui::TreeNode((void*)entity.first, "#%d", entity.first)) {
						int i = 0;
						for (auto&& component : entity.second.components) {
							if (ImGui::TreeNode((void*)i++, component.first.c_str())) {
								for (auto&& prop : component.second.properties) {
									ImGui::Text(prop.first.c_str());
									ImGui::SameLine();
									ImGui::PushItemWidth(-1);
									ImGui::PushID(prop.first.c_str());
									switch (prop.second.type) {
										case tec::Property::FLOAT:
											{
												float val = prop.second.Get<float>();
												if (ImGui::InputFloat("##labellessInputFloat", &val)) {
													prop.second.Set(val);
													prop.second.update_func(prop.second);
												}
											}
											break;
										case tec::Property::BOOLEAN:
											{
												bool val = prop.second.Get<bool>();
												if (ImGui::Checkbox("##labellessCheckbox", &val)) {
													prop.second.Set(val);
													prop.second.update_func(prop.second);
												}
											}
											break;
										case tec::Property::INTEGER:
											{
												int val = prop.second.Get<int>();
												if (ImGui::InputInt("##labellessInputInt", &val)) {
													prop.second.Set(val);
													prop.second.update_func(prop.second);
												}
											}
											break;
										case tec::Property::STRING:
											{
												std::string val = prop.second.Get<std::string>();
												val.resize(256);
												if (ImGui::InputText("##labellessInputText", &val[0], 256)) {
													std::string trimmed = val.substr(0, val.find('\0'));
													prop.second.Set(trimmed);
													prop.second.update_func(prop.second);
												}
											}
											break;
										case tec::Property::DROPDOWN:
											{
												tec::dropdown_t key_func = prop.second.Get<tec::dropdown_t>();
												std::vector<std::string>& keys = key_func.first();
												current_combo_item[current_combo_item_slot] = -1;
												std::stringstream joinedValues;
												for (size_t item = 0; item < keys.size(); ++item) {
													joinedValues << keys[item] << '\0';
													if (keys[item] == key_func.second) {
														current_combo_item[current_combo_item_slot] = item;
													}
												}
												joinedValues << '\0';
												std::string result = joinedValues.str();
												if (current_combo_item_slot < MAX_COMBO_ITEM_SLOTS) {
													current_combo_item_slot++;
												}
												if (ImGui::Combo("##labellessCombo", &current_combo_item[current_combo_item_slot - 1], result.c_str())) {
													key_func.second = keys[current_combo_item[current_combo_item_slot - 1]];
													prop.second.Set(key_func);
													prop.second.update_func(prop.second);
												}
											}
											break;
										case tec::Property::RADIO:
											{
												tec::radio_t choices = prop.second.Get<tec::radio_t>();
												std::vector<std::string>& keys = choices.first;
												current_combo_item[current_combo_item_slot] = -1;
												static int radio_choice = 0;
												for (std::size_t i = 0; i < keys.size(); ++i) {
													if (keys[i] == choices.second) {
														radio_choice = i;
													}
													ImGui::SameLine();
													if (ImGui::RadioButton(keys[i].c_str(), (radio_choice == i) ? true : false)) {
														radio_choice = i;
														choices.second = keys[radio_choice];
														prop.second.Set(choices);
														prop.second.update_func(prop.second);
													}
												}
											}
											break;
										case tec::Property::RGB:
											{
												glm::vec3 val = prop.second.Get<glm::vec3>();
												float color[] = {val.r, val.g, val.b};
												if (ImGui::ColorEdit3("Color", color)) {
													prop.second.Set(glm::vec3(color[0], color[1], color[2]));
													prop.second.update_func(prop.second);
												}
											}
											break;
									}
									ImGui::PopID();
								}
								ImGui::TreePop();
							}
						}
						if (ImGui::Button("Add")) {
							ImGui::OpenPopup("AddComponent");
						}
						if (ImGui::BeginPopup("AddComponent")) {
							for (auto factory : tec::component_factories) {
								if (entity.second.components.find(factory.first) == entity.second.components.end()) {
									if (ImGui::Selectable(factory.first.c_str())) {
										factory.second(entity.first);
									}
								}
							}
							ImGui::EndPopup();
						}
						ImGui::SameLine();
						if (ImGui::Button("Remove")) {
							ImGui::OpenPopup("RemoveComponent");
						}
						if (ImGui::BeginPopup("RemoveComponent")) {
							for (auto itr = entity.second.components.begin(); itr != entity.second.components.end();) {
								if (ImGui::Selectable(itr->first.c_str())) {
									tec::component_removal_factories[itr->first.c_str()](entity.first);
									entity.second.components.erase(itr++);
								}
								else {
									++itr;
								}
							}
							ImGui::EndPopup();
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			ImGui::End();
		}
	});
	gui.AddWindowDrawFunction("console", [&console]() {
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

	return 0;
}
