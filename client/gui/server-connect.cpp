#include "server-connect.hpp"

namespace tec {
	ServerConnectWindow::ServerConnectWindow(ServerConnection& server_connection) : server_connection(server_connection) {}

	void ServerConnectWindow::Update(double) {}

	void ServerConnectWindow::Draw() {
		if (this->show) {
			ImGuiIO& io = ImGui::GetIO();
			ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), 0, ImVec2(0.5f, 0.5f));
			ImGui::Begin("Connect to Server", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

			static int octets[4] = { 127, 0, 0, 1 };

			float width = ImGui::CalcItemWidth();
			ImGui::PushID("IP");
			ImGui::AlignTextToFramePadding();
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
				printf("Connecting to %s\n", ip.str().c_str());
				this->server_connection.Connect(ip.str());
			}
			ImGui::End();
			ImGui::SetWindowSize("Connect to Server", ImVec2(0, 0));
		}
	}

	PingTimesWindow::PingTimesWindow(ServerConnection& server_connection) : server_connection(server_connection) {}

	void PingTimesWindow::Update(double) {}

	void PingTimesWindow::Draw() {
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::Begin("ping_times", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
		static float arr[10];
		std::list<tec::networking::ping_time_t> recent_pings = this->server_connection.GetRecentPings();
		std::size_t i = 0;
		for (tec::networking::ping_time_t ping : recent_pings) {
			arr[i++] = static_cast<float>(ping);
		}
		ImGui::PlotHistogram("Ping", arr, 10, 0, nullptr, 0.0f, 100.0f);
		ImGui::SameLine();
		ImGui::Text("%" PRI_PING_TIME_T, this->server_connection.GetAveragePing());
		ImGui::SetWindowPos("ping_times", ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::GetWindowSize().x - 10, 20));
		ImGui::End();
		ImGui::SetWindowSize("ping_times", ImVec2(0, 0));
		ImGui::PopStyleColor();
	}
}
