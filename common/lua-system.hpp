#pragma once
/**
 * Lua system
 */

#include <spdlog/spdlog.h>
#include <sol/sol.hpp>

#include "tec-types.hpp"
#include "event-system.hpp"
#include "command-queue.hpp"
#include "filesystem.hpp"
#include "components/lua-script.hpp"

namespace tec {
	class LuaSystem;
	typedef Command<LuaSystem> LuaCommand;
	struct EntityCreated;
	struct EntityDestroyed;
	struct ControllerAddedEvent;
	struct ControllerRemovedEvent;

	class LuaSystem : public CommandQueue< LuaSystem >,
		public EventQueue < EntityCreated >,
		public EventQueue < EntityDestroyed >,
		public EventQueue < ControllerAddedEvent >,
		public EventQueue < ControllerRemovedEvent > {
	public:
		LuaSystem();

		void Update(const double delta);
		void ProcessEvents();

		using EventQueue<EntityCreated>::On;
		using EventQueue<EntityDestroyed>::On;
		using EventQueue<ControllerAddedEvent>::On;
		using EventQueue<ControllerRemovedEvent>::On;
		void On(std::shared_ptr<EntityCreated> data);
		void On(std::shared_ptr<EntityDestroyed> data);
		void On(std::shared_ptr<ControllerAddedEvent> data);
		void On(std::shared_ptr<ControllerRemovedEvent> data);

		void ExecuteString(std::string script_string);

		std::shared_ptr<LuaScript> LoadFile(FilePath filepath);

		sol::state& GetGlobalState() {
			return this->lua;
		}
	private:
		sol::state lua;
	};
}
