#include "server-connect.hpp"
#include "imgui-system.hpp"
#include <commands.pb.h>

namespace tec {
ServerConnectWindow::ServerConnectWindow(ServerConnection& server_connection) : server_connection(server_connection) {
	this->window_name = "connect_window";
}

void ServerConnectWindow::Update(double) {}

void ServerConnectWindow::Draw(IMGUISystem* gui) {
	if (this->show) {
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), 0, ImVec2(0.5f, 0.5f));
		ImGui::Begin(
				"Connect to Server",
				nullptr,
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

		static int octets[4] = {127, 0, 0, 1};

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
			spdlog::get("console_log")->info("Connecting to {}", ip.str());
			this->server_connection.Connect(ip.str());
			gui->HideWindow(this->window_name);
			gui->ShowWindow("login_window");
		}
		ImGui::End();
		ImGui::SetWindowSize("Connect to Server", ImVec2(0, 0));
	}
}

void LoginWindow::Draw(IMGUISystem* gui) {
	if (this->show) {
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), 0, ImVec2(0.5f, 0.5f));
		ImGui::Begin(
				"Login", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		ImGui::TextUnformatted("Username");
		ImGui::SameLine();
		ImGui::InputText("", this->username, IM_ARRAYSIZE(this->username), ImGuiInputTextFlags_CharsNoBlank);
		ImGui::SameLine();
		if (ImGui::Button("Login")) {
			auto hide_login_window = std::bind(&IMGUISystem::HideWindow, gui, window_name);
			this->server_connection.RegisterMessageHandler(
					tec::networking::MessageType::AUTHENTICATED,
					[hide_login_window](tec::networking::MessageIn&) { hide_login_window(); });
			proto::UserLogin user_login;
			user_login.set_username(this->username);
			user_login.set_password("");
			networking::MessageOut msg(tec::networking::LOGIN);
			user_login.SerializeToZeroCopyStream(&msg);
			this->server_connection.Send(msg);
		}
		ImGui::End();
		ImGui::SetWindowSize("Login", ImVec2(0, 0));
	}
}

PingTimesWindow::PingTimesWindow(ServerConnection& server_connection) : server_connection(server_connection) {
	this->window_name = "ping_times";
}

void PingTimesWindow::Update(double) {}

void PingTimesWindow::Draw(IMGUISystem*) {
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::Begin(
			"ping_times",
			nullptr,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
					| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	static float arr[networking::PING_HISTORY_SIZE]; // god help you if the deque overflows this
	std::list<tec::networking::ping_time_t> recent_pings = this->server_connection.GetRecentPings();
	std::size_t i = 0;
	for (tec::networking::ping_time_t ping : recent_pings) {
		arr[i++] = static_cast<float>(ping);
	}
	ImGui::PlotHistogram("Ping", arr, networking::PING_HISTORY_SIZE, 0, nullptr, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::Text("% 3" PRI_PING_TIME_T " ms", this->server_connection.GetAveragePing());
	auto& connection_stats = this->server_connection.stats;
	connection_stats.estimated_delay_accumulator += this->server_connection.GetEstimatedDelay();
	connection_stats.estimated_delay_count++;
	if (connection_stats.estimated_delay_count >= networking::DELAY_HISTORY_SIZE) {
		connection_stats.estimated_delay =
				connection_stats.estimated_delay_accumulator / connection_stats.estimated_delay_count;
		connection_stats.estimated_delay_count = 0;
		connection_stats.estimated_delay_accumulator = 0;
	}
	ImGui::Text("State Delay: %05" PRId64 " ms", connection_stats.estimated_delay);
	ImGui::Text(
			"States:% 2" PRIu64 "  Cmd In-flight:% 2" PRIu64,
			connection_stats.server_state_count,
			connection_stats.current_command_id - connection_stats.current_acked_id);
	ImGui::Text(
			"   Cmd: % 9" PRIu64 " Ack: % 9" PRIu64,
			connection_stats.current_command_id,
			connection_stats.current_acked_id);
	glm::vec3 pos = connection_stats.client_position;
	glm::vec3 vel = connection_stats.client_velocity;
	ImGui::Text("X: %1.3f, %1.3f, %1.3f", pos.x, pos.y, pos.z);
	ImGui::Text("V: %1.3f, %1.3f, %1.3f", vel.x, vel.y, vel.z);
	ImGui::SetWindowPos("ping_times", ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::GetWindowSize().x - 10, 20));
	ImGui::End();
	ImGui::SetWindowSize("ping_times", ImVec2(0, 0));
	ImGui::PopStyleColor();
}
} // namespace tec
