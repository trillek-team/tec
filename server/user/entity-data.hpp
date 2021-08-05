#pragma once

#include "components/transforms.hpp"

namespace tec {
namespace user {

struct EntityData {
	static void RegisterLuaType(sol::state&);

	Position position;
	Orientation orientation;
};

} // namespace user
} // namespace tec
