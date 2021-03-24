#pragma once

#include <list>

#include <sol/sol.hpp>

#include <save_game.pb.h>

#include "filesystem.hpp"
#include "tec-types.hpp"
#include "user.hpp"

namespace tec {

class UserList {
	using user_list_t = std::list<std::shared_ptr<User>>;

public:
	template <typename T> void SetUsers(T begin, T end);
	void AddUser(std::shared_ptr<User>);
	std::shared_ptr<User> CreateUser(uid, std::string);
	std::shared_ptr<User> GetUser(uid);
	std::shared_ptr<User> FindUser(std::string);
	bool RemoveUser(uid);
	bool HasUser(uid);

	const user_list_t* GetUsers() { return &this->users; }

	static void RegisterLuaType(sol::state&);

private:
	user_list_t users;
	user_list_t::iterator GetUserItr(uid user_id) {
		return std::find_if(this->users.begin(), this->users.end(), [user_id](std::shared_ptr<User> user) {
			return user->GetUserId() == user_id;
		});
	}
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
