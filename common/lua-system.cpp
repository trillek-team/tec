#include "lua-system.hpp"

#include "entity.hpp"
#include "events.hpp"
#include "multiton.hpp"
#include "proto-load.hpp"
#include "resources/script-file.hpp"

namespace tec {
using LuaScriptMap = Multiton<eid, LuaScript*>;

static int LuaSystemPanicHandler(sol::optional<std::string> maybe_msg) {
	if (maybe_msg) {
		spdlog::get("console_log")->warn("A Lua script panic occured {}", maybe_msg.value());
		throw std::runtime_error(maybe_msg.value());
	}
	throw std::runtime_error(std::string("An unexpected Lua script panic occurred"));
}

LuaSystem::LuaSystem() {
	this->lua.open_libraries( // load the essentials
			sol::lib::base,
			sol::lib::bit32,
			sol::lib::math,
			sol::lib::package,
			sol::lib::string,
			sol::lib::table);
	// log exceptions that occur in the middle of scripts (assuming they aren't script errors)
	this->lua.set_exception_handler(
			[](lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
				std::string extra;
				if (maybe_exception.has_value()) {
					const std::type_info& exception_type = typeid(maybe_exception.value());
					if (exception_type == typeid(sol::error)) {
						return sol::stack::push(L, description);
					}
					extra = exception_type.name();
				}
				spdlog::get("console_log")->warn("Exception in script:{} {}", extra, description);
				return sol::stack::push(L, description);
			});

	// probably not going to be called, but might be helpful
	this->lua.set_panic(sol::c_call<decltype(&LuaSystemPanicHandler), &LuaSystemPanicHandler>);

	// a "simple" loader for require() to find scripts in the assets path
	this->lua.add_package_loader(
			[this](sol::string_view package_name) -> sol::protected_function {
				spdlog::get("console_log")->debug("request to load Lua package \"{}\"", package_name);
				static FilePath scripts_base = FilePath::GetAssetsBasePath() / "scripts";
				std::string package_path(package_name.data(), package_name.size());
				if (package_path.find_first_of('.') == std::string::npos) {
					package_path.append(".lua");
				}
				// for now, we are going to look for a single lua file with by the same name as the package
				FilePath package_script_path = scripts_base / package_path;
				std::string source;
				if (!package_script_path.FileExists()) {
					return sol::nil;
				}
				try {
					source = LoadAsString(package_script_path);
				}
				catch (std::exception&) {
					return sol::nil;
				}
				sol::load_result r = this->lua.load(source, package_script_path.toString());
				if (!r.valid()) {
					return sol::nil;
				}
				sol::protected_function f = r;
				return f;
			},
			false);

	this->lua["print"] = [this](sol::variadic_args print_args) {
		std::string message;
		for (auto value : print_args) {
			std::string str;
			if (value.is<std::string>()) {
				str = value.get<std::string>();
			}
			else { // explicitly convert arguments to strings if they are not
				str = this->lua["tostring"](value);
			}
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
	// global state functions
	if (this->lua[function_name].valid()) {
		functions.push_back(this->lua[function_name]);
	}
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
