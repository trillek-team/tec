#include "lua-system.hpp"

#include "entity.hpp"
#include "events.hpp"
#include "multiton.hpp"
#include "resources/script-file.hpp"

namespace tec {
using LuaScriptMap = Multiton<eid, LuaScript*>;

LuaSystem::LuaSystem() {
	this->lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
	this->lua["print"] = [](sol::variadic_args va) {
		std::string message;
		for (auto v : va) {
			std::string str = v;
			if (!message.empty()) {
				message.push_back(' ');
			}
			message.append(str);
		}
		spdlog::get("console_log")->info(message);
	};
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

void LuaSystem::ProcessEvents() {
	ProcessCommandQueue();
	EventQueue<EntityCreated>::ProcessEventQueue();
	EventQueue<EntityDestroyed>::ProcessEventQueue();
	EventQueue<ChatCommandEvent>::ProcessEventQueue();
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
			break;
		}
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
		case proto::Component::COMPONENT_NOT_SET: break;
		}
	}
}

std::list<sol::protected_function> LuaSystem::GetAllFunctions(std::string function_name) {
	std::list<sol::protected_function> functions;

	// multiton <eid, LuaScript*>
	for (auto itr = LuaScriptMap::Begin(); itr != LuaScriptMap::End(); itr++) {
		if (!itr->second->script_name.empty() && itr->second->environment[function_name].valid()) {
			functions.push_back(itr->second->environment[function_name]);
		}
	}
	// list<scripts>
	for (auto script = scripts.begin(); script != scripts.end(); script++) {
		if (!script->script_name.empty() && script->environment[function_name].valid()) {
			functions.push_back(script->environment[function_name]);
		}
	}
	return functions;
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

void LuaSystem::On(std::shared_ptr<ChatCommandEvent> data) {
	this->CallFunctions("onChatCommand", data->command, data->args);
}

void LuaSystem::ExecuteString(std::string script_string) { this->lua.script(script_string); }
} // namespace tec
