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
	ImGui::Text("FPS: %i | avg: %1.3fs", game.fps, game.avg_frame_time);
	// Procentages of time spend
	float SQ = game.state_queue_time / game.total_time;
	float VC = game.vcomputer_time / game.total_time;
	float SS = game.sound_system_time / game.total_time;
	float RS = game.render_system_time / game.total_time;
	float LS = game.lua_system_time / game.total_time;
	float other = game.other_time / game.total_time;
	float outside = game.outside_game_time / game.total_time;
	ImGui::Text("SQ: %1.3f | VC: %1.3f | SS: %1.3f | RS: %1.3f", SQ, VC, SS, RS);
	ImGui::Text("LS: %1.3f | other: %1.3f | outside: %1.3f", LS, other, outside);
	ImGui::Text("Total time: %1.3f", game.total_time);
	ImGui::ProgressBar(SQ,ImVec2(0, 0),"SQ");
	ImGui::ProgressBar(VC,ImVec2(0, 0),"VC");
	ImGui::ProgressBar(SS,ImVec2(0, 0),"SS");
	ImGui::ProgressBar(RS,ImVec2(0, 0),"RS");
	ImGui::ProgressBar(LS,ImVec2(0, 0),"LS");
	ImGui::ProgressBar(other,ImVec2(0, 0),"other");
	ImGui::ProgressBar(outside,ImVec2(0, 0),"outside");
	ImGui::SetWindowPos("debug_info", ImVec2(10, 30));
	ImGui::End();
	ImGui::SetWindowSize("debug_info", ImVec2(0, 0));
	ImGui::PopStyleColor();
}
} // namespace tec
