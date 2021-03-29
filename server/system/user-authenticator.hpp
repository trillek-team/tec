#pragma once
#include <functional>
#include <list>
#include <string>

#include "data-source.hpp"
#include "user/user.hpp"

namespace tec {
using namespace user;
namespace system {
class UserAuthenticator {
public:
	void SetDataSource(util::DataSource<User>* data_source) { this->user_data_source = data_source; }
	void AddUser(std::string user_id, std::string username, std::string password = "") {
		if (!this->user_data_source) {
			return;
		}
		User user;
		user.SetUserId(user_id);
		user.SetUsername(username);
		this->user_data_source->Add(user);
	}
	User* Authenticate(std::string username, std::string password = "") {
		if (this->user_data_source) {
			return this->user_data_source->FindWhere(
					[username](const User& user) { return user.GetUsername() == username; });
		}
		return nullptr;
	}

private:
	util::DataSource<User>* user_data_source{nullptr};
};
} // namespace system
} // namespace tec
