
#include "lua-system.hpp"

#include "manipulators/placement.hpp"

TEC_RegisterLuaType(tec::manipulator, Placement) {
	// clang-format off
	state.new_usertype<Placement>(
		"Placement", sol::no_constructor,
		"set_mesh", sol::resolve<void(const std::string&)>(&Placement::SetMesh),
		"clear_mesh", &Placement::ClearMesh
	);
	// clang-format on
}

#include "os.hpp"

TEC_RegisterLuaType(tec, OS) {
	// clang-format off
	state.new_usertype<OS>(
		"OS", sol::no_constructor, // single instance
		"quit", &OS::Quit,
		"exit", &OS::Quit,
		"get_window_width", &OS::GetWindowWidth,
		"get_window_height", &OS::GetWindowHeight,
		"get_mouse_x", []() {
			double x = 0;
			OS::GetMousePosition(&x, nullptr);
			return x;
		},
		"get_mouse_y", []() {
			double y = 0;
			OS::GetMousePosition(nullptr, &y);
			return y;
		}
	);
	// clang-format on
}
