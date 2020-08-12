// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "lua-system.hpp"

#include "entity.hpp"
#include "events.hpp"
#include "multiton.hpp"
#include "components/lua-script.hpp"

namespace tec {
	using LuaScriptMap = Multiton<eid, LuaScript*>;

	LuaSystem::LuaSystem() {
		this->lua.open_libraries(sol::lib::base, sol::lib::package);

		this->lua["print"] = [] (std::string str) {
			spdlog::get("console_log")->info(str);
		};
	}

	void LuaSystem::Update(const double delta) {
		ProcessCommandQueue();
		EventQueue<EntityCreated>::ProcessEventQueue();
		EventQueue<EntityDestroyed>::ProcessEventQueue();

		for (auto itr = LuaScriptMap::Begin(); itr != LuaScriptMap::End(); itr++) {
			auto entity_id = itr->first;
			if (!itr->second->script_name.empty() && itr->second->environment["onUpdate"].valid()) {
				itr->second->environment["onUpdate"](delta);
			}
		}
	}

	void LuaSystem::On(std::shared_ptr<EntityCreated> data) {
		eid entity_id = data->entity.id();
		for (int i = 0; i < data->entity.components_size(); ++i) {
			const proto::Component& comp = data->entity.components(i);
			switch (comp.component_case()) {
			case proto::Component::kLuaScript:
			{
				LuaScript* script = new LuaScript();
				script->SetupEnvironment(&this->lua);
				script->In(comp);
				LuaScriptMap::Set(entity_id, script);
			}
			break;
			case proto::Component::kCollisionBody:
			case proto::Component::kRenderable:
			case proto::Component::kPosition:
			case proto::Component::kOrientation:
			case proto::Component::kView:
			case proto::Component::kAnimation:
			case proto::Component::kScale:
			case proto::Component::kVelocity:
			case proto::Component::kAudioSource:
			case proto::Component::kPointLight:
			case proto::Component::kDirectionalLight:
			case proto::Component::kSpotLight:
			case proto::Component::kVoxelVolume:
			case proto::Component::kComputer:
			case proto::Component::COMPONENT_NOT_SET:
				break;
			}
		}
	}

	void LuaSystem::On(std::shared_ptr<EntityDestroyed> data) {
		LuaScriptMap::Remove(data->entity_id);
	}

	void LuaSystem::ExecuteString(std::string script_string) {
		this->lua.script(script_string);
	}
}
