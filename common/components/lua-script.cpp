// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "lua-script.hpp"

#include <memory>
#include <map>

#include "types.hpp"
#include "resources/script-file.hpp"

namespace tec {
	LuaScript::LuaScript()
		: state() {
		this->ReloadScript();
	}

	LuaScript::LuaScript(std::shared_ptr<ScriptFile> scriptfile)
		: script_name(scriptfile->GetName()), script(scriptfile), state() {
		this->ReloadScript();
	}

	void LuaScript::ReloadScript() {
		auto _log = spdlog::get("console_log");
		if (!this->script_name.empty()) {
			this->state.ForceGC();
			auto print = [](std::string str1) { //, std::string str2=std::string(), std::string str3=std::string(), std::string str4=std::string()) {
				spdlog::get("console_log")->info(str1); //, str2, str3, str4);
			};
			this->state["print"] = print;
			/*for (auto& add_kv : in_functors) {
				std::string name = TypeName.at(add_kv.first);
				name = "add" + name;
				this->state[name.c_str()] = add_kv.second;
			}*/

			this->state.Load(this->script->GetScript());
		}
	}

	void LuaScript::Out(proto::Component* target) {
		proto::LuaScript* comp = target->mutable_luascript();
		comp->set_script_name(this->script_name);
	}

	extern std::map<std::string, std::function<void(std::string)>> file_factories;

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

}
