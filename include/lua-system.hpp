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
