
#include "lua-system.hpp"

#include "client-connection.hpp"
#include "save-game.hpp"
#include "server.hpp"

TEC_RegisterLuaType(tec, SaveGame) {
	// clang-format off
	state.new_usertype<SaveGame>(
		"SaveGame", sol::no_constructor,
		/*"load", sol::overload(
			sol::resolve<bool(std::string)>(&SaveGame::Load),
			sol::resolve<bool(Path)>(&SaveGame::Load)
		), */
		"save", sol::overload(
			sol::resolve<bool()>(&SaveGame::Save)/*,
			sol::resolve<bool(Path)>(&SaveGame::Save)*/
		),
		"reload", sol::overload(
			sol::resolve<bool()>(&SaveGame::Reload)/*,
			sol::resolve<bool(Path)>(&SaveGame::Reload)*/
		),
		"user_list", sol::readonly(&SaveGame::user_list)
	);
	// clang-format on
}

TEC_RegisterLuaType(tec, UserList) {
	// clang-format off
	state.new_usertype<UserList>(
		"UserList", sol::no_constructor,
		"AddUser", &UserList::AddUser,
		"CreateUser", &UserList::CreateUser,
		"GetUser", &UserList::GetUser,
		"RemoveUser", &UserList::RemoveUser,
		"HasUser", &UserList::HasUser
	);
	// clang-format on
}

TEC_RegisterLuaType(tec::user, User) {
	// clang-format off
	state.new_usertype<User>(
		"User", sol::no_constructor,
		"user_id", sol::property(&User::GetUserId, &User::SetUserId),
		"entity_id", sol::readonly(&User::entity_id),
		"entity_data", sol::readonly(&User::entity_data),
		"credentials", sol::readonly(&User::credentials)
	);
	// clang-format on
}

TEC_RegisterLuaType(tec::user, Credentials) {
	// clang-format off
	state.new_usertype<Credentials>(
		"Credentials", sol::no_constructor,
		"user_id", &Credentials::user_id,
		"username", &Credentials::username
	);
	// clang-format on
}

// position and orientation should probably go in a "transform" type...
TEC_RegisterLuaType(tec::user, EntityData) {
	// clang-format off
	state.new_usertype<EntityData>(
		"EntityData", sol::no_constructor,
		"position", &EntityData::position,
		"orientation", &EntityData::orientation
	);
	// clang-format on
}

TEC_RegisterLuaType(tec::networking, ClientConnection) {
	// clang-format off
	state.new_usertype<ClientConnection>(
		"ClientConnection", sol::no_constructor,
		"user", &ClientConnection::user
	);
	// clang-format on
}

TEC_RegisterLuaType(tec::networking, UserLoginEvent) {
	// clang-format off
	state.new_usertype<UserLoginEvent>(
		"UserLoginInfo", sol::no_constructor,
		"username", &UserLoginEvent::username,
		"user_id", &UserLoginEvent::user_id,
		"reason", &UserLoginEvent::reason,
		"authenticated", &UserLoginEvent::authenticated,
		"reject", &UserLoginEvent::reject
	);
	// clang-format on
}

TEC_RegisterLuaType(tec::networking, ClientConnectionEvent) {
	// clang-format off
	state.new_usertype<ClientConnectionEvent>(
		"ClientConnectionInfo", sol::no_constructor,
		"cancel", &ClientConnectionEvent::cancel,
		"reason", &ClientConnectionEvent::reason,
		"port", sol::readonly(&ClientConnectionEvent::port),
		"address", sol::readonly(&ClientConnectionEvent::address),
		"family", sol::readonly(&ClientConnectionEvent::family),
		"protocol", sol::readonly(&ClientConnectionEvent::protocol)
	);
	// clang-format on
}
