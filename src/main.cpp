#include "os.hpp"
#include "reflection.hpp"
#include "render-system.hpp"
#include "physics-system.hpp"
#include "sound-system.hpp"
#include "imgui-system.hpp"
#include "component-update-system.hpp"
#include "controllers/fps-controller.hpp"

#include <thread>
#include <string>
#include <sstream>

namespace tec {
	extern void IntializeComponents();
	extern void IntializeIOFunctors();
	extern void BuildTestEntities();
	extern void ProtoSave();
	extern void ProtoLoad();

	ReflectionEntityList entity_list;
}

std::list<std::function<void(tec::frame_id_t)>> tec::ComponentUpdateSystemList::update_funcs;

int main(int argc, char* argv[]) {
	tec::OS os;

	os.InitializeWindow(800, 600, "TEC 0.1", 3, 2);

	tec::IMGUISystem gui(os.GetWindow());
	ImVec4 clear_color = ImColor(114, 144, 154);
	gui.AddWindowDrawFunction("test", [&clear_color] () {
		static float f = 0.0f;
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	});

	tec::RenderSystem rs;

	rs.SetViewportSize(800, 600);

	tec::PhysicsSystem ps;

	tec::SoundSystem ss;

	std::int64_t frame_id = 1;

	tec::IntializeComponents();
	tec::IntializeIOFunctors();
	tec::BuildTestEntities();
	tec::ProtoLoad();

	tec::FPSController camera_controller(1);

	tec::eid active_entity;
	gui.AddWindowDrawFunction("active_entity", [&active_entity] () {
		if (active_entity != 0) {
			ImGui::SetTooltip("#%i", active_entity);
		}
	});
	gui.AddWindowDrawFunction("test_window", [&active_entity] () {
		static bool opened = true;
		ImGui::ShowTestWindow(&opened);
	});
	gui.AddWindowDrawFunction("entity_tree", [ ] () {
		static bool no_titlebar = false;
		static bool no_border = true;
		static bool no_resize = false;
		static bool no_move = false;
		static bool no_scrollbar = false;
		static bool no_collapse = false;
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
												std::vector<std::pair<std::string, bool>> val = prop.second.Get<std::vector<std::pair<std::string, bool>>>();
												if (current_combo_item_slot < MAX_COMBO_ITEM_SLOTS) {
													current_combo_item_slot++;
												}
												current_combo_item[current_combo_item_slot] = -1;
												std::stringstream joinedValues;
												for (size_t item = 0; item < val.size(); ++item) {
													joinedValues << val[item].first << '\0';
													if (val[item].second) {
														current_combo_item[current_combo_item_slot] = item;
													}
												}
												joinedValues << '\0';
												std::string result = joinedValues.str();
												if (ImGui::Combo("##labellessCombo", &current_combo_item[current_combo_item_slot], result.c_str())) {
													for (size_t item = 0; item < val.size(); ++item) {
														val[item].second = false;
													}
													val[current_combo_item[current_combo_item_slot]].second = true;
													prop.second.Set(val);
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
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			ImGui::End();
		}
	});

	double delta = os.GetDeltaTime();
	while (!os.Closing()) {
		os.OSMessageLoop();
		delta = os.GetDeltaTime();

		tec::ComponentUpdateSystemList::UpdateAll(frame_id);

		camera_controller.Update(delta);
		std::thread ps_thread([&] () {
			ps.Update(delta);
		});
		std::thread ss_thread([&] () {
			ss.Update(delta);
		});

		rs.Update(delta);

		os.OSMessageLoop();

		ps_thread.join();
		ss_thread.join();

		ps.DebugDraw();
		active_entity = ps.RayCast();

		gui.Update(delta);

		os.SwapBuffers();
		frame_id++;
	}
	tec::ProtoSave();

	return 0;
}
