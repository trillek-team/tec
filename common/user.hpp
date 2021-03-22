#pragma once

#include <sol/sol.hpp>
#include <string>

#include <save_game.pb.h>

#include "entity.hpp"
#include "tec-types.hpp"

namespace tec {
struct FPSController;

class User {
public:
	~User();

	void Build(eid);

	uid GetUserId() const { return this->user_id; }
	void SetUserId(uid _user_id) { this->user_id = _user_id; }

	eid GetEntityId() const { return this->entity.GetID(); }

	std::string GetUsername() const { return this->username; }
	void SetUsername(std::string _username) { this->username = _username; }

	static void RegisterLuaType(sol::state&);
	
	void Out(proto::User* target);
	void In(const proto::User& source);

private:
	std::string username;
	Entity entity{0};
	uid user_id;

	std::shared_ptr<FPSController> controller;
};
} // namespace tec
