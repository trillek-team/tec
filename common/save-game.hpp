#pragma once

#include <list>

#include <sol/sol.hpp>

#include <save_game.pb.h>

#include "filesystem.hpp"
#include "tec-types.hpp"
#include "user.hpp"

namespace tec {

class UserList {
public:
	template <typename T> void SetUsers(T begin, T end);
	void AddUser(User);
	User* CreateUser(uid, std::string);
	User* GetUser(uid);
	User* FindUser(std::string);
	bool RemoveUser(uid);
	bool HasUser(uid);

	const std::list<User>* GetUsers();

	static void RegisterLuaType(sol::state&);

private:
	std::list<User> users;
	std::list<User>::iterator GetUserItr(uid);
};

class SaveGame {
public:
	bool Load(const FilePath);
	bool Load(std::string);
	bool Reload();
	bool Reload(const FilePath);
	bool Save();
	bool Save(const FilePath);

	UserList* GetUserList();

	static void RegisterLuaType(sol::state&);

private:
	void LoadUsers();
	void SaveUsers();
	void LoadWorld();
	void SaveWorld();

	proto::SaveGame save;
	FilePath filepath;
	UserList user_list;
};
} // namespace tec
