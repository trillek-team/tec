#include "save-game.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>

#include <google/protobuf/util/json_util.h>
#include <spdlog/spdlog.h>

#include "event-system.hpp"
#include "events.hpp"
#include "proto-load.hpp"

namespace tec {
eid GetNextEntityId();

template <typename T> void UserList::SetUsers(T begin, T end) {
	for (auto itr = begin; itr != end; itr++) {
		auto user = std::make_shared<User>(GetNextEntityId());
		user->In(*itr);
		this->users.emplace_back(user);
	}
}

void UserList::AddUser(std::shared_ptr<User> user) { this->users.emplace_back(user); }

std::shared_ptr<User> UserList::CreateUser(uid user_id, std::string username) {
	auto user = std::make_shared<User>(GetNextEntityId());
	user->SetUserId(user_id);
	user->SetUsername(username);
	this->users.emplace_back(user);
	return GetUser(user_id);
}

std::shared_ptr<User> UserList::GetUser(uid id) {
	auto existing_user = this->GetUserItr(id);
	if (existing_user != this->users.end()) {
		return *existing_user;
	}
	return nullptr;
}

std::shared_ptr<User> UserList::FindUser(std::string username) {
	for (auto& user : this->users) {
		if (user->GetUsername() == username) {
			return user;
		}
	}
	return nullptr;
}

bool UserList::RemoveUser(uid id) {
	auto existing_user = this->GetUserItr(id);
	if (existing_user != this->users.end()) {
		this->users.erase(existing_user);
		return true;
	}
	return false;
}

bool UserList::HasUser(uid id) { return this->GetUserItr(id) != this->users.end(); }

void UserList::RegisterLuaType(sol::state& state) {
	// clang-format off
	state.new_usertype<UserList>(
			"UserList", sol::no_constructor,
			"AddUser", &UserList::AddUser,
			"CreateUser", &UserList::CreateUser,
			"GetUser", &UserList::GetUser,
			"FindUser", &UserList::FindUser,
			"RemoveUser", &UserList::RemoveUser,
			"HasUser", &UserList::HasUser
		);
	// clang-format on
}

bool SaveGame::Load(std::string _filename) { return this->Load(FilePath(_filename)); }

bool SaveGame::Load(const FilePath _filepath) {
	auto _log = spdlog::get("console_log");
	this->filepath = _filepath;
	if (!_filepath.FileExists()) {
		_log->error("File does not exist: {}", _filepath.FileName());
		return false;
	}

	auto json_string = LoadAsString(this->filepath);
	auto status = google::protobuf::util::JsonStringToMessage(json_string, &this->save);
	if (!status.ok()) {
		_log->error("Failed to parse save data");
		return false;
	}
	this->LoadUsers();
	this->LoadWorld();
	return true;
}

bool SaveGame::Reload() { return this->Load(this->filepath); }

bool SaveGame::Reload(const FilePath _filepath) { return this->Load(_filepath); }

bool SaveGame::Save() { return this->Save(this->filepath); }

bool SaveGame::Save(const FilePath _filepath) {
	auto _log = spdlog::get("console_log");
	if (!_filepath.FileExists() || !_filepath.isValidPath()) {
		_log->error("File does not exist or path is invalid: {}", _filepath.toString());
		return false;
	}
	this->filepath = _filepath;
	this->SaveUsers();
	this->SaveWorld();

	std::string json_string;

	auto status = google::protobuf::util::MessageToJsonString(this->save, &json_string);

	if (!status.ok()) {
		_log->error("Failed to serialize save data");
		return false;
	}

	try {
		return SaveFromString(_filepath, json_string);
	}
	catch (std::runtime_error err) {
		_log->error("Failed to save to file: {}", _filepath.FileName());
		return false;
	}
}

UserList* SaveGame::GetUserList() { return &this->user_list; }

void SaveGame::RegisterLuaType(sol::state& state) {
	// clang-format off
	state.new_usertype<SaveGame>(
			"SaveGame", sol::no_constructor,
			/*"load", sol::overload(
				sol::resolve<bool(std::string)>(&SaveGame::Load),
				sol::resolve<bool(FilePath)>(&SaveGame::Load)
			), */
			"save", sol::overload(
				sol::resolve<bool()>(&SaveGame::Save)/*,
				sol::resolve<bool(FilePath)>(&SaveGame::Save)*/
			),
			"reload", sol::overload(
				sol::resolve<bool()>(&SaveGame::Reload)/*,
				sol::resolve<bool(FilePath)>(&SaveGame::Reload)*/
			),
			"user_list", sol::readonly(&SaveGame::user_list)
		);
	// clang-format on
}

void SaveGame::LoadUsers() {
	auto users = this->save.mutable_users();
	user_list.SetUsers(users->begin(), users->end());
}

void SaveGame::SaveUsers() {
	auto users = this->save.mutable_users();
	users->Clear();
	for (const auto& user : *user_list.GetUsers()) {
		auto save_user = users->Add();
		user->Out(save_user);
	}
}

void SaveGame::LoadWorld() {
	auto _log = spdlog::get("console_log");
	auto world = this->save.world();
	for (int i = 0; i < world.entity_file_list_size(); i++) {
		FilePath entity_filename = FilePath::GetAssetPath(world.entity_file_list(i));
		if (entity_filename.isValidPath() && entity_filename.FileExists()) {
			std::string json_string = LoadAsString(entity_filename);
			proto::Entity entity;
			auto status = google::protobuf::util::JsonStringToMessage(json_string, &entity);
			if (status.ok()) {
				EventSystem<EntityCreated>::Get()->Emit(
						std::make_shared<EntityCreated>(EntityCreated{static_cast<eid>(entity.id()), entity}));
			}
			else {
				_log->error("Failed to parse entity data from file: {}", entity_filename.toString());
			}
		}
		else {
			_log->error("File does not exist or path is invalid: {}", entity_filename.toString());
		}
	}
}

void SaveGame::SaveWorld() {
	auto world = this->save.mutable_world();
	// world->Clear();
	// TODO: Find way to get active entities in the world and iterate over them
}
} // namespace tec
