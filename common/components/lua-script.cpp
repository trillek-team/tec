#include "lua-script.hpp"

#include <map>
#include <memory>

#include "file-factories.hpp"
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
		this->global_state->safe_script(
				this->script->GetScript(),
				environment,
				[](lua_State*, sol::protected_function_result pfr) {
					sol::error err = pfr;
					spdlog::get("console_log")->warn("Script error: {}", err.what());
					// if we don't throw anything, then we have to return this
					return pfr;
				},
				this->script_name);
	}
}

void LuaScript::Out(proto::Component* target) {
	proto::LuaScript* comp = target->mutable_luascript();
	comp->set_script_name(this->script_name);
}

void LuaScript::In(const proto::Component& source) {
	const proto::LuaScript& comp = source.luascript();

	if (comp.has_script_name()) {
		this->script_name = comp.script_name();
		this->script = GetResource<ScriptFile>(this->script_name);
		if (!this->script) {
			spdlog::get("console_log")->error("LuaScript::In: script is null: {}", this->script_name);
			throw std::exception();
		}
		this->ReloadScript();
	}
}
} // namespace tec
