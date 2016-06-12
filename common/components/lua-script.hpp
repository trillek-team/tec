// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <string>
#include <memory>

#include <spdlog/spdlog.h>
#include <selene.h>

#include "components.pb.h"

namespace tec {

	class ScriptFile;

	struct LuaScript {

		LuaScript(std::shared_ptr<ScriptFile> scriptfile);
		LuaScript();

		void Out(proto::Component* target);
		void In(const proto::Component& source);

		void ReloadScript();

		std::string script_name;
		std::shared_ptr<ScriptFile> script;

		sel::State state;
	};
}
