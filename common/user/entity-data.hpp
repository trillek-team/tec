#pragma once

#include "components/transforms.hpp"

namespace tec {
namespace user {
struct EntityData {
	Position position;
	Orientation orientation;

	// Called from User::RegisterLuaType
	static void RegisterLuaType(sol::state& state) {
		// clang-format off
		state.new_usertype<EntityData>(
			"EntityData", sol::no_constructor,
			"position", &EntityData::position,
			"orientation", &EntityData::orientation
		);
		// clang-format on
	}
};
} // namespace user
} // namespace tec
