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
	struct ClientConnectedEvent;
	struct ClientDisconnectedEvent;

	class LuaSystem : public CommandQueue< LuaSystem >,
		public EventQueue < EntityCreated >,
		public EventQueue < EntityDestroyed >,
		public EventQueue < ClientConnectedEvent >,
		public EventQueue < ClientDisconnectedEvent > {
	public:
		LuaSystem();

		void Update(const double delta);
		void ProcessEvents();

		using EventQueue<EntityCreated>::On;
		using EventQueue<EntityDestroyed>::On;
		using EventQueue<ClientConnectedEvent>::On;
		using EventQueue<ClientDisconnectedEvent>::On;
		void On(std::shared_ptr<EntityCreated> data);
		void On(std::shared_ptr<EntityDestroyed> data);
		void On(std::shared_ptr<ClientConnectedEvent> data);
		void On(std::shared_ptr<ClientDisconnectedEvent> data);

		void ExecuteString(std::string script_string);

		std::shared_ptr<LuaScript> LoadFile(FilePath filepath);

		sol::state& GetGlobalState() {
			return this->lua;
		}
	private:
		sol::state lua;
		std::list<LuaScript> scripts;

		void CallFunction(std::string);
	};
}
