#pragma once

#include <algorithm>
#include <list>

#include <sol/sol.hpp>

#include <save_game.pb.h>

#include "data-source.hpp"
#include "filesystem.hpp"
#include "tec-types.hpp"
#include "user/user.hpp"

namespace tec {
using namespace user;
class UserList {
public:
	void AddUser(User);
	User* CreateUser(uid);
	User* GetUser(uid);
	bool RemoveUser(uid);
	bool HasUser(uid);

	std::list<User>* GetUsers() { return &this->users; }

	static void RegisterLuaType(sol::state&);

private:
	std::list<User> users;
	std::list<User>::iterator GetUserItr(uid user_id) {
		return std::find_if(this->users.begin(), this->users.end(), [user_id](const User& user) {
			return user.GetUserId() == user_id;
		});
	}
};

class UserListDataSource : public util::DataSource<User> {
public:
	UserListDataSource(UserList& _user_list) : user_list(_user_list) {}
	void Add(User user) override { this->user_list.AddUser(user); }
	void RemoveWhere(std::function<bool(const User&)> predicate) override {
		std::list<User>* users = this->user_list.GetUsers();
		std::remove_if(users->begin(), users->end(), predicate);
	}
	User* FindWhere(std::function<bool(const User&)> predicate) override {
		std::list<User>* users = this->user_list.GetUsers();
		auto itr = std::find_if(users->begin(), users->end(), predicate);
		return itr != users->end() ? &*itr : nullptr;
	}
private:
	UserList& user_list;
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
