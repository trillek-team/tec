// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once
/**
 * Lua system
 */

#include <spdlog/spdlog.h>
#include <selene.h>

#include "multiton.hpp"
#include "types.hpp"
#include "event-system.hpp"
#include "command-queue.hpp"
#include "components/lua-script.hpp"

namespace tec {
	
	class LuaSystem;
	typedef Command<LuaSystem> LuaCommand;
	
	class LuaSystem :
		public CommandQueue<LuaSystem> {
	public:
		LuaSystem();
		~LuaSystem();
		
		void Update(const double delta);
		
	private:
		typedef Multiton<eid, std::shared_ptr<LuaScript>> ScriptsMap;
	};
	
}
