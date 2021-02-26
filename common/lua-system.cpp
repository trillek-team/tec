#include "lua-system.hpp"

#include "entity.hpp"
#include "events.hpp"
#include "multiton.hpp"
#include "resources/script-file.hpp"

namespace tec {
using LuaScriptMap = Multiton<eid, LuaScript*>;

	LuaSystem::LuaSystem() {
		this->lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
    this->lua["print"] = [](std::string str) { spdlog::get("console_log")->info(str); };
	}

	void LuaSystem::Update(const double delta) {
		ProcessEvents();
		
		for (auto itr = LuaScriptMap::Begin(); itr != LuaScriptMap::End(); itr++) {
			//auto entity_id = itr->first; <------ commented as it is currently unused
			if (!itr->second->script_name.empty() && itr->second->environment["onUpdate"].valid()) {
				itr->second->environment["onUpdate"](delta);
			}
		}
	}
}
  
  void LuaSystem::ProcessEvents() {
		ProcessCommandQueue();
		EventQueue<EntityCreated>::ProcessEventQueue();
		EventQueue<EntityDestroyed>::ProcessEventQueue();
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
			case proto::Component::kSpo tLight:
			case proto::Component::kVoxelVolume:
			case proto::Component::kComputer:
			case proto::Component::COMPONENT_NOT_SET:
				break;
			}
		}
	}
}

	// TODO: Add parameters to function calls
	void LuaSystem::CallFunction(std::string function_name) {
		// multiton <eid, LuaScript*>
		for (auto itr = LuaScriptMap::Begin(); itr != LuaScriptMap::End(); itr++) {
			if (!itr->second->script_name.empty() && itr->second->environment[function_name].valid()) {
				itr->second->environment[function_name]();
			}
		}
		// list<scripts>
		for (auto script = scripts.begin(); script != scripts.end(); script++) {
			if (!script->script_name.empty() && script->environment[function_name].valid()) {
				script->environment[function_name]();
			}
		}
	}

	std::shared_ptr<LuaScript> LuaSystem::LoadFile(FilePath filepath) {
		std::shared_ptr<LuaScript> script = std::make_shared<LuaScript>(ScriptFile::Create(filepath));
		script->SetupEnvironment(&this->lua);
		script->ReloadScript();

		// add to list of scripts
		scripts.push_back(*script);
		return script;
	}

void LuaSystem::On(std::shared_ptr<EntityDestroyed> data) { LuaScriptMap::Remove(data->entity_id); }

void LuaSystem::ExecuteString(std::string script_string) { this->lua.script(script_string); }
} // namespace tec
