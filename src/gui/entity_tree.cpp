#include "gui/entity_tree.hpp"

#include <sstream>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>

#include "reflection.hpp"

namespace tec {
	extern std::map<std::string, std::function<void(std::string)>> file_factories;
	extern std::map<std::string, std::function<void(eid)>> component_factories;
	extern std::map<std::string, std::function<void(eid)>> component_removal_factories;
	extern ReflectionEntityList entity_list;

	const ImGuiWindowFlags window_flags = 0;

	void EntityTree::Draw() {
		if (!show) {
			return;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
		if (! ImGui::Begin("Entity Tree", &collapsed, window_flags)) {
			ImGui::End();
			ImGui::PopStyleVar();
			return;
		}

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
			for (auto itr = entity_list.entities.begin(); itr != entity_list.entities.end(); ++itr) {
				if (ImGui::Selectable(std::to_string(itr->first).c_str())) {
					for (auto factory : component_removal_factories) {
						factory.second(itr->first);
					}
					entity_list.entities.erase(itr);
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
											for (std::size_t item = 0; item < keys.size(); ++item) {
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
		ImGui::PopStyleVar();
	}
}
