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

void UserList::AddUser(User user) { this->users.emplace_back(user); }

User* UserList::CreateUser(uid user_id) {
	User user;
	user.SetUserId(user_id);
	this->AddUser(user);
	return GetUser(user_id);
}

User* UserList::GetUser(uid id) {
	auto existing_user = this->GetUserItr(id);

	return existing_user != this->users.end() ? &*existing_user : nullptr;
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

bool SaveGame::Load(std::string _filename) { return this->Load(Path(_filename)); }

bool SaveGame::Load(const Path _filepath) {
	auto _log = spdlog::get("console_log");
	this->filepath = _filepath;
	if (!_filepath.FileExists()) {
		_log->error("File does not exist: {}", _filepath.FileName());
		return false;
	}

	auto json_string = LoadAsString(this->filepath);
	auto status = google::protobuf::util::JsonStringToMessage(json_string, &this->save);
	if (!status.ok()) {
		_log->error("Failed to parse save data {}", status.ToString());
		return false;
	}
	this->LoadUsers();
	this->LoadWorld();
	return true;
}

bool SaveGame::Reload() { return this->Load(this->filepath); }

bool SaveGame::Reload(const Path _filepath) { return this->Load(_filepath); }

bool SaveGame::Save() { return this->Save(this->filepath); }

bool SaveGame::Save(const Path _filepath) {
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
		_log->error("Failed to serialize save data {}", status.ToString());
		return false;
	}

	try {
		return SaveFromString(_filepath, json_string);
	}
	catch (std::runtime_error& err) {
		_log->error("Failed to save to file: {}", _filepath.FileName());
		return false;
	}
}

UserList* SaveGame::GetUserList() { return &this->user_list; }

void SaveGame::LoadUsers() {
	auto users = this->save.users();

	for (const auto& _user : users) {
		User user;
		user.In(_user);
		this->user_list.AddUser(user);
	}
}

void SaveGame::SaveUsers() {
	auto users = this->save.mutable_users();
	users->Clear();
	for (auto user : *user_list.GetUsers()) {
		user.Out(users->Add());
	}
}

void SaveGame::LoadWorld() {
	auto _log = spdlog::get("console_log");
	auto world = this->save.world();
	for (int i = 0; i < world.entity_file_list_size(); i++) {
		Path entity_filename = Path::GetAssetPath(world.entity_file_list(i));
		if (entity_filename.isValidPath() && entity_filename.FileExists()) {
			std::string json_string = LoadAsString(entity_filename);
			proto::Entity entity;
			auto status = google::protobuf::util::JsonStringToMessage(json_string, &entity);
			if (status.ok()) {
				EventSystem<EntityCreated>::Get()->Emit(std::make_shared<EntityCreated>(EntityCreated{entity}));
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
	//auto world = this->save.mutable_world();
	// world->Clear();
	// TODO: Find way to get active entities in the world and iterate over them
}
} // namespace tec
