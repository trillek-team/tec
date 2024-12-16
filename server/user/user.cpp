#include "user.hpp"

#include "components/collision-body.hpp"
#include "components/velocity.hpp"
#include "controllers/fps-controller.hpp"
#include "event-system.hpp"
#include "events.hpp"

namespace tec {
eid GetNextEntityId();

namespace user {
User::~User() {}

void User::AddEntityToWorld() {
	this->entity_id = GetNextEntityId();
	Entity entity(this->entity_id);

	entity.Add<Position, Orientation, Velocity>(entity_data.position, entity_data.orientation, Velocity());

	CollisionBody* body = new CollisionBody();
	body->mass = 10.0f;
	body->disable_deactivation = true;
	body->disable_rotation = true;
	body->SetCapsuleShape(0.5f, 1.6f);
	body->entity_id = this->entity_id;
	entity.Add(body);
	{
		std::shared_ptr<EntityCreated> data = std::make_shared<EntityCreated>();
		entity.Out<Position, Orientation, Velocity, CollisionBody>(data->entity);
		data->entity.set_id(this->entity_id);
		auto renderable = data->entity.add_components()->mutable_renderable();
		renderable->set_mesh_name("bot/botplay.obj");
		renderable->set_shader_name("deferred");
		renderable->mutable_position()->set_z(0.5f);
		EventSystem<EntityCreated>::Get()->Emit(data);
	}
	{
		this->controller = std::make_shared<FPSController>(this->entity_id);
		std::shared_ptr<ControllerAddedEvent> data = std::make_shared<ControllerAddedEvent>();
		data->controller = this->controller;
		EventSystem<ControllerAddedEvent>::Get()->Emit(data);
	}
}

void User::RemoveEntityFromWorld() {
	{
		if (this->entity_id != 0) {
			std::shared_ptr<EntityDestroyed> data = std::make_shared<EntityDestroyed>();
			EventSystem<EntityDestroyed>::Get()->Emit(this->entity_id, data);
			this->entity_id = 0;
		}
	}
	{
		if (this->controller) {
			std::shared_ptr<ControllerRemovedEvent> data = std::make_shared<ControllerRemovedEvent>();
			data->controller = this->controller;
			EventSystem<ControllerRemovedEvent>::Get()->Emit(data);
			this->controller.reset();
		}
	}
}

void User::UpdateEntityData() {
	Entity entity(this->entity_id);
	auto [position, orientation] = entity.GetList<Position, Orientation>();
	if (position) {
		this->entity_data.position = *position;
	}
	if (orientation) {
		this->entity_data.orientation = *orientation;
	}
}

void User::Out(proto::User* target) {
	target->set_id(this->credentials.user_id);
	target->set_username(this->credentials.username);
	this->UpdateEntityData();
	auto components = target->mutable_entity_data()->mutable_component_states();
	this->entity_data.position.Out(components->Add());
	this->entity_data.orientation.Out(components->Add());
}

void User::In(const proto::User& source) {
	this->credentials.user_id = source.id();
	this->credentials.username = source.username();

	for (const auto& component : source.entity_data().component_states()) {
		switch (component.component_case()) {
		case proto::Component::kPosition:
		{
			this->entity_data.position.In(component.position());
			break;
		}
		case proto::Component::kOrientation:
		{
			this->entity_data.orientation.In(component.orientation());
			break;
		}
		default: break;
		}
	}
}

} // namespace user
} // namespace tec
