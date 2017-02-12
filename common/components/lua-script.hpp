// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <string>
#include <memory>

#include <sol/sol.hpp>
#include <components.pb.h>

#include "types.hpp"

namespace tec {
	class ScriptFile;

	struct LuaScript {
		LuaScript(std::shared_ptr<ScriptFile> scriptfile);
		LuaScript();

		void Out(proto::Component* target);
		void In(const proto::Component& source);

		void SetupEnvironment(sol::state* global_state);
		void ReloadScript();

		std::string script_name;
		std::shared_ptr<ScriptFile> script;

		sol::environment environment;
		sol::state* global_state;
	};
}
