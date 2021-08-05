#pragma once

namespace tec {
namespace user {

struct Credentials {
	static void RegisterLuaType(sol::state&);

	std::string user_id;
	std::string username;
	std::string password;
};

} // namespace user
} // namespace tec
