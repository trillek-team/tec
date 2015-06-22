#include "os.hpp"
#include "render-system.hpp"
#include "physics-system.hpp"
#include "sound-system.hpp"
#include "imgui-system.hpp"
#include "component-update-system.hpp"
#include "components/camera.hpp"

#include <thread>

namespace tec {
	extern void IntializeComponents();
	extern void BuildTestEntities();
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
	tec::BuildTestEntities();

	tec::CameraMover cam_mover(1);
	tec::eid active_entity;
	gui.AddWindowDrawFunction("active_entity", [&active_entity] () {
		if (active_entity != 0) {
			ImVec2 pos(400, 300);
			ImGui::SetNextWindowPos(pos);
			ImGui::BeginTooltip();
			ImGui::Text("#%i", active_entity);
			ImGui::EndTooltip();
		}
	});

	double delta = os.GetDeltaTime();
	while (!os.Closing()) {
		os.OSMessageLoop();
		delta = os.GetDeltaTime();

		tec::ComponentUpdateSystemList::UpdateAll(frame_id);

		cam_mover.Update(0.0);
		std::thread ps_thread([&] () {
			ps.Update(delta);
		});
		std::thread ss_thread([&] () {
			ss.Update(delta);
		});

		rs.Update(delta);
		gui.Update(delta);

		os.SwapBuffers();
		frame_id++;
		ps_thread.join();
		ss_thread.join();
		active_entity = ps.RayCast();
	}

	return 0;
}
