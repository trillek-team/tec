#pragma once

#include <sol/sol.hpp>
#include <string>

#include <save_game.pb.h>

#include "entity.hpp"
#include "tec-types.hpp"

namespace tec {
struct FPSController;

/**
* \brief Container used to store information about a logged in user.
*
* Information such as the user's id, username, inventory, etc will be connect here.
* 
* Additionally users contain an Entity that can be used to access the user's state
* in the world for purposes of retrieving position, health, etc components.
*/
class User {
public:
	User(eid entity_id);
	User() = default;

	// No copy as destructor removes by entity from world by id
	User(const User& other) = delete;

	User(User&& other) noexcept {
		user_id = std::move(other.user_id);
		username = std::move(other.username);
		controller = std::move(other.controller);
		entity = std::move(other.entity);
		other.entity = Entity(-1);
	}

	// No copy as destructor removes by entity from world by id
	User& operator=(const User& other) = delete;

	User& operator=(User&& other) noexcept {
		user_id = std::move(other.user_id);
		username = std::move(other.username);
		controller = std::move(other.controller);
		entity = std::move(other.entity);
		other.entity = Entity(-1);
	}

	~User();

	/**
	* \brief Add the user's entity to the world by emitting EntityCreated and ControllerAddedEvent events.
	*
	* Call this after all user entity state has been set.
	*/
	void AddEntityToWorld();

	/**
	* \brief Get the user's ID.
	*
	* This ID is different than an entity ID in that it is assign during registration
	* and is not meant to vary whereas an entity is assigned at random when the user's
	* entity is added to the world.
	* \return uid Users' ID.
	*/
	uid GetUserId() const { return this->user_id; }
	void SetUserId(uid _user_id) { this->user_id = _user_id; }

	/**
	* \brief Get the underlying entity's ID.
	*
	* \return eid The underlying entity's ID.
	*/
	eid GetEntityId() const { return this->entity.GetID(); }

	/**
	* \brief Get the username associated with this user.
	*
	* \return string User's username.
	*/
	std::string GetUsername() const { return this->username; }
	void SetUsername(std::string _username) { this->username = _username; }

	static void RegisterLuaType(sol::state&);

	/**
	* \brief Serialize to a proto::User.
	*
	* \param proto::User* target The proto::User to serialize to.
	*/
	void Out(proto::User* target) const;

	/**
	* \brief Deserialize from a proto::User.
	*
	* \param const proto::User& source The proto::User to Deserialize from.
	*/
	void In(const proto::User& source);

private:
	std::string username;
	Entity entity{0};
	uid user_id;

	std::shared_ptr<FPSController> controller; /**< Used to handle (client) or replay (server) input. */
};
} // namespace tec
