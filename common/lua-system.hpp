


#pragma once
/**
 * Lua system
 */

#include <spdlog/spdlog.h>
#include <sol/sol.hpp>

#include "types.hpp"
#include "event-system.hpp"
#include "command-queue.hpp"

namespace tec {
	class LuaSystem;
	typedef Command<LuaSystem> LuaCommand;
	struct EntityCreated;
	struct EntityDestroyed;

	class LuaSystem : public CommandQueue< LuaSystem >,
		public EventQueue < EntityCreated >,
		public EventQueue < EntityDestroyed > {
	public:
		LuaSystem();

		void Update(const double delta);

		using EventQueue<EntityCreated>::On;
		using EventQueue<EntityDestroyed>::On;
		void On(std::shared_ptr<EntityCreated> data);
		void On(std::shared_ptr<EntityDestroyed> data);

		void ExecuteString(std::string script_string);

		sol::state& GetGlobalState() {
			return this->lua;
		}
	private:
		sol::state lua;
	};
}
