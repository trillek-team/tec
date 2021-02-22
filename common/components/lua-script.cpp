#include "lua-script.hpp"

#include <map>
#include <memory>

#include "resources/script-file.hpp"
#include "tec-types.hpp"

namespace tec {
LuaScript::LuaScript() { this->ReloadScript(); }

LuaScript::LuaScript(std::shared_ptr<ScriptFile> scriptfile) : script_name(scriptfile->GetName()), script(scriptfile) {
	this->ReloadScript();
}

void LuaScript::SetupEnvironment(sol::state* _global_state) {
	this->global_state = _global_state;
	this->environment = sol::environment(*this->global_state, sol::create, this->global_state->globals());
}

void LuaScript::ReloadScript() {
	if (!this->global_state) {
		return;
	}
	if (!this->script_name.empty()) {
		this->global_state->script(this->script->GetScript(), environment);
	}
}

void LuaScript::Out(proto::Component* target) {
	proto::LuaScript* comp = target->mutable_luascript();
	comp->set_script_name(this->script_name);
}

extern std::unordered_map<std::string, std::function<void(std::string)>> file_factories;

void LuaScript::In(const proto::Component& source) {
	const proto::LuaScript& comp = source.luascript();

	if (comp.has_script_name()) {
		this->script_name = comp.script_name();
		if (!ScriptMap::Has(this->script_name)) {
			std::string ext = this->script_name.substr(this->script_name.find_last_of(".") + 1);
			if (file_factories.find(ext) != file_factories.end()) {
				file_factories[ext](this->script_name);
			}
		}
		this->script = ScriptMap::Get(this->script_name);
		this->ReloadScript();
	}
}
} // namespace tec
