#pragma once

namespace tec {
namespace user {

struct Credentials {
	std::string user_id;
	std::string username;
	std::string password;

	// Called from User::RegisterLuaType
	static void RegisterLuaType(sol::state& state) {
		// clang-format off
		state.new_usertype<Credentials>(
			"Credentials", sol::no_constructor,
			"user_id", &Credentials::user_id,
			"username", &Credentials::username
		);
		// clang-format on
	}
};
} // namespace user
} // namespace tec
