#include "debug-info.hpp"

namespace tec {
DebugInfo::DebugInfo(Game& game) : game(game) {}

void DebugInfo::Update(double) {}

void DebugInfo::Draw() {
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::Begin(
			"debug_info",
			nullptr,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
					| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	ImGui::Text("FPS: %i", game.fps);
	ImGui::SetWindowPos("debug_info", ImVec2(10, 30));
	ImGui::End();
	ImGui::SetWindowSize("debug_info", ImVec2(0, 0));
	ImGui::PopStyleColor();
}
} // namespace tec
