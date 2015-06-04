#include "os.hpp"
#include "render-system.hpp"
#include "component-update-system.hpp"
#include "components/camera.hpp"

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

	std::int64_t frame_id = 1;

	tec::IntializeComponents();
	tec::BuildTestEntities();

	tec::CameraMover cam_mover(1);

	while (!os.Closing()) {
		tec::ComponentUpdateSystemList::UpdateAll(frame_id);

		cam_mover.Update(0.0);
		rs.Update(os.GetDeltaTime());
		os.OSMessageLoop();
		os.SwapBuffers();
		frame_id++;
	}

	return 0;
}
