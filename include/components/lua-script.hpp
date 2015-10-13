#pragma once

#include <string>
#include <memory>

#include <spdlog/spdlog.h>
#include <selene.h>

#include "../proto/components.pb.h"

namespace tec {
	
	class ScriptFile;
	struct ReflectionComponent;
	
	struct LuaScript {
		
		LuaScript(std::shared_ptr<ScriptFile> scriptfile);
		LuaScript();

		
		static ReflectionComponent Reflection(LuaScript* val);

		void Out(proto::Component* target);
		void In(const proto::Component& source);
	
		void ReloadScript();

		std::string script_name;
		std::shared_ptr<ScriptFile> script;
		
		sel::State state;
	};
}
