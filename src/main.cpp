#include "os.hpp"
#include "render-system.hpp"
#include "physics-system.hpp"
#include "sound-system.hpp"
#include "component-update-system.hpp"
#include "controllers/fps-controller.hpp"

#include <thread>

namespace tec {
	extern void IntializeComponents();
	extern void BuildTestEntities();
}

std::list<std::function<void(tec::frame_id_t)>> tec::ComponentUpdateSystemList::update_funcs;

int main(int argc, char* argv[]) {
	tec::OS os;

	os.InitializeWindow(800, 600, "TEC 0.1", 3, 2);

	tec::RenderSystem rs;

	rs.SetViewportSize(800, 600);

	tec::PhysicsSystem ps;
	
	tec::SoundSystem ss;

	std::int64_t frame_id = 1;

	tec::IntializeComponents();
	tec::BuildTestEntities();

	tec::FPSController camera_controller(1);

	double delta = os.GetDeltaTime();
	while (!os.Closing()) {
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
		ps.DebugDraw();
		os.SwapBuffers();
		frame_id++;
		ps_thread.join();
		ss_thread.join();
	}

	return 0;
}
