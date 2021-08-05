#pragma once

#include "tec-types.hpp"
#include <save_game.pb.h>

#include "credentials.hpp"
#include "entity-data.hpp"
#include "entity.hpp"

namespace tec {
struct FPSController;

namespace user {
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
	User() = default;

	~User();

	/**
	* \brief Add the user's entity to the world by emitting EntityCreated and ControllerAddedEvent events.
	*
	* Call this after all user entity state has been set.
	*/
	void AddEntityToWorld();

	/**
	* \brief Add the user's entity to the world by emitting EntityDestroy and ControllerRemovedEvent events.
	*
	* Call this when the user is no longer in the world e.g. disconnect.
	*/
	void RemoveEntityFromWorld();

	/**
	* \brief Get the user's ID.
	*
	* This ID is different than an entity ID in that it is assign during registration
	* and is not meant to vary whereas an entity is assigned at random when the user's
	* entity is added to the world.
	* \return uid Users' ID.
	*/
	uid GetUserId() const { return this->credentials.user_id; }
	void SetUserId(uid user_id) { this->credentials.user_id = user_id; }

	/**
	* \brief Get the underlying entity's ID.
	*
	* \return eid The underlying entity's ID.
	*/
	eid GetEntityId() const { return this->entity_id; }

	/**
	* \brief Get the username associated with this user.
	*
	* \return string User's username.
	*/
	std::string GetUsername() const { return this->credentials.username; }
	void SetUsername(std::string username) { this->credentials.username = username; }

	static void RegisterLuaType(sol::state&);

	/**
	* \brief Serialize to a proto::User.
	*
	* \param proto::User* target The proto::User to serialize to.
	*/
	void Out(proto::User* target);

	/**
	* \brief Deserialize from a proto::User.
	*
	* \param const proto::User& source The proto::User to Deserialize from.
	*/
	void In(const proto::User& source);

private:
	eid entity_id{0};
	EntityData entity_data;
	Credentials credentials;

	/**
	* \brief Update entity data state.
	*/
	void UpdateEntityData();

	std::shared_ptr<FPSController> controller; /**< Used to handle (client) or replay (server) input. */
};
} // namespace user
} // namespace tec
