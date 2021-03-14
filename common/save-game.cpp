#include "save-game.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>

#include <google/protobuf/util/json_util.h>
#include <spdlog/spdlog.h>

#include "event-system.hpp"
#include "events.hpp"

namespace tec {
template <typename T> void UserList::SetUsers(T begin, T end) { this->users.assign(begin, end); }

void UserList::AddUser(proto::User user) { return this->users.push_back(user); }

const std::list<proto::User>* UserList::GetUsers() { return &this->users; }

proto::User* UserList::GetUser(uid id) {
	auto existing_user = this->GetUserItr(id);
	if (existing_user != this->users.end()) {
		return &*existing_user;
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

bool UserList::UserExists(uid id) { return this->GetUserItr(id) != this->users.end(); }

std::list<proto::User>::iterator UserList::GetUserItr(uid id) {
	return std::find_if(this->users.begin(), this->users.end(), [id](proto::User user) { return user.id() == id; });
}

extern std::string LoadJSON(const FilePath& fname);

bool SaveJSON(const FilePath& fname, std::string contents) {
	std::fstream output(fname.GetNativePath(), std::ios::out);
	if (!output.good())
		throw std::runtime_error("can't open ." + fname.toString());

	output.write(contents.c_str(), contents.length());
	output.close();
	return true;
}

bool SaveGame::Load(const FilePath _filepath) {
	auto _log = spdlog::get("console_log");
	this->filepath = _filepath;
	if (!_filepath.FileExists()) {
		_log->error("File does not exist: {}", _filepath.FileName());
		return false;
	}

	auto json_string = LoadJSON(this->filepath);
	auto status = google::protobuf::util::JsonStringToMessage(json_string, &this->save);
	if (!status.ok()) {
		_log->error("Failed to parse save data");
		return false;
	}
	this->LoadUsers();
	this->LoadWorld();
	return true;
}

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
		return SaveJSON(_filepath, json_string);
	}
	catch (std::runtime_error err) {
		_log->error("Failed to save to file: {}", _filepath.FileName());
		return false;
	}
}

UserList* SaveGame::GetUserList() { return &this->user_list; }

void SaveGame::LoadUsers() {
	auto users = this->save.mutable_users();
	user_list.SetUsers(users->begin(), users->end());
}

void SaveGame::SaveUsers() {
	auto users = this->save.mutable_users();
	users->Clear();
	for (const auto& user : *user_list.GetUsers()) {
		auto save_user = users->Add();
		save_user->CopyFrom(user);
	}
}

void SaveGame::LoadWorld() {
	auto _log = spdlog::get("console_log");
	auto world = this->save.world();
	for (int i = 0; i < world.entity_file_list_size(); i++) {
		FilePath entity_filename = FilePath::GetAssetPath(world.entity_file_list(i));
		if (entity_filename.isValidPath() && entity_filename.FileExists()) {
			std::string json_string = LoadJSON(entity_filename);
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
