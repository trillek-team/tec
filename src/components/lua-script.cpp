#include "components/lua-script.hpp"

#include <memory>
#include <map>

#include "resources/script-file.hpp"
#include "reflection.hpp"

namespace tec {

	LuaScript::LuaScript (std::shared_ptr<ScriptFile> scriptfile)
		: script_name(scriptfile->GetName()), script(scriptfile), state(true) {
		this->ReloadScript();

		/*
		auto print = [](const char* str) {
			spdlog::get("console_log")->info() << str;
		};
		this->state["print"] = &print;
		*/
	}
	
	ReflectionComponent LuaScript::Reflection(LuaScript* val) {
		ReflectionComponent refcomp;
		Property dprop(Property::DROPDOWN);
		dropdown_t key_func_script = std::make_pair(ScriptMap::Keys, val->script_name);
		(refcomp.properties["Script Picker"] = dprop).Set(key_func_script);
		refcomp.properties["Script Picker"].update_func = [val] (Property& prop) {
			dropdown_t key_func = prop.Get<dropdown_t>();
			val->script_name = key_func.second;
			val->script = ScriptMap::Get(val->script_name);
			spdlog::get("console_log")->info() << val->script->GetScript();
			val->ReloadScript();
		};

		return std::move(refcomp);
	}

	void LuaScript::ReloadScript()	{
		if (!this->script_name.empty()) {
			this->state.LoadStr(this->script->GetScript());
			spdlog::get("console_log")->info() << this->state["foo"];
		}
	}

	void LuaScript::Out(proto::Component* target) {
		proto::LuaScript* comp = target->mutable_script();
		comp->set_script_name(this->script_name);
	}

	extern std::map<std::string, std::function<void(std::string)>> file_factories;

	void LuaScript::In(const proto::Component& source) {
		const proto::LuaScript& comp = source.script();
		
		if (comp.has_script_name()) {
			this->script_name = comp.script_name();
			if (!ScriptMap::Has(this->script_name)) {
				std::string ext = this->script_name.substr(this->script_name.find_last_of(".") + 1);
				if (file_factories.find(ext) != file_factories.end()) {
					file_factories[ext](this->script_name);
				}
			}
			this->script = ScriptMap::Get(this->script_name);
		}
	}
	
}
