#pragma once

#include <vector>

#include <save_game.pb.h>

#include "filesystem.hpp"

#include "tec-types.hpp"

namespace tec {
using uid = decltype(proto::User().id());

class UserList {
public:
	template <typename T> void SetUsers(T begin, T end);
	void AddUser(proto::User);
	const std::vector<proto::User>* GetUsers();
	proto::User* GetUser(uid);
	bool RemoveUser(uid);
	bool UserExists(uid);

private:
	std::vector<proto::User> users;
	std::vector<proto::User>::iterator GetUserItr(uid);
};

class SaveGame {
public:
	bool Load(const FilePath);
	bool Save();
	bool Save(const FilePath);

	UserList* GetUserList();

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
