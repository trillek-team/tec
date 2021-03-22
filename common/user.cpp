#include "user.hpp"

#include "components/collision-body.hpp"
#include "components/transforms.hpp"
#include "components/velocity.hpp"
#include "controllers/fps-controller.hpp"
#include "event-system.hpp"
#include "events.hpp"

namespace tec {
Entity CreateUserEntity(eid entity_id) {
	Entity entity(entity_id);
	entity.Add<Position, Orientation, Velocity>(glm::vec3(5, 1.0, 5), glm::vec3(.5f, 0.f, 0.f), Velocity());
	CollisionBody* body = new CollisionBody();
	body->mass = 10.0f;
	body->disable_deactivation = true;
	body->disable_rotation = true;
	body->SetCapsuleShape(0.5f, 1.6f);
	body->entity_id = entity_id;
	entity.Add(body);
	return entity;
}

void User::Build(eid entity_id) {
	this->entity = CreateUserEntity(entity_id);
	{
		std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
		this->entity.Out<Position, Orientation, Velocity, CollisionBody>(data->entity);
		data->entity.set_id(entity_id);
		data->entity_id = entity_id;
		EventSystem<EntityCreated>::Get()->Emit(data);
	}
	{
		this->controller = std::make_shared<FPSController>(entity_id);
		std::shared_ptr<ControllerAddedEvent> data = std::make_shared<ControllerAddedEvent>();
		data->controller = controller;
		EventSystem<ControllerAddedEvent>::Get()->Emit(data);
	}
}

User::~User() {
	{
		std::shared_ptr<EntityDestroyed> data = std::make_shared<EntityDestroyed>();
		data->entity_id = this->entity.GetID();
		EventSystem<EntityDestroyed>::Get()->Emit(data);
	}
	{
		std::shared_ptr<ControllerRemovedEvent> data = std::make_shared<ControllerRemovedEvent>();
		data->controller = this->controller;
		EventSystem<ControllerRemovedEvent>::Get()->Emit(data);
	}
}

void User::Out(proto::User* target) {
	target->set_id(this->user_id);
	target->set_username(this->username);
}

void User::In(const proto::User& source) {
	this->user_id = source.id();
	this->username = source.username();
}

void User::RegisterLuaType(sol::state& state) {
	// clang-format off
	state.new_usertype<User>(
			"User", sol::no_constructor,
			"user_id", sol::property(&User::GetUserId, &User::SetUserId),
			"entity_id", sol::property(&User::GetEntityId),
			"username", sol::readonly(&User::username),
			"position", sol::property([](const User& user) {
				auto position = user.entity.Get<Position>();
				return position->value;
			}, [](User user, glm::vec3 position) {
				user.entity.Update<Position>(new Position(position));
			})
		);
	state.new_usertype<glm::vec3>(
			"vec3", sol::no_constructor,
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z
		);
	// clang-format on
}
} // namespace tec
