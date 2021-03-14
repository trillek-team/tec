#pragma once

#include <list>

#include <save_game.pb.h>

#include "filesystem.hpp"

#include "tec-types.hpp"

namespace tec {
// The follow is used to align the type used used for UserList operations
// with the type defined in the proto file definition. E.g if id is if type
// string in the .proto file, UserList::AddUser expects a string id parameter.
using uid = decltype(proto::User().id());

class UserList {
public:
	template <typename T> void SetUsers(T begin, T end);
	void AddUser(proto::User);
	const std::list<proto::User>* GetUsers();
	proto::User* GetUser(uid);
	bool RemoveUser(uid);
	bool UserExists(uid);

private:
	std::list<proto::User> users;
	std::list<proto::User>::iterator GetUserItr(uid);
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
