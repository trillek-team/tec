#pragma once

#include "components.pb.h"
#include "event-system.hpp"
#include "events.hpp"

namespace tec {

enum ENGINE_ENTITIES { MANIPULATOR = 1 };

namespace { // Make the follow functions header local so they can't be invoked directly.
void CreateManipulatorEntity() {
	proto::Entity entity;
	entity.set_id(ENGINE_ENTITIES::MANIPULATOR);
	entity.add_components()->mutable_renderable();
	EventSystem<EntityCreated>::Get()->Emit(std::make_shared<EntityCreated>(EntityCreated{entity}));
}
} // namespace

inline void CreateEngineEntities() { CreateManipulatorEntity(); }

} // namespace tec
