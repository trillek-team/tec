#pragma once
/**
 * Lua system
 */

#include <sol/sol.hpp>
#include <spdlog/spdlog.h>

#include "command-queue.hpp"
#include "components/lua-script.hpp"
#include "event-system.hpp"
#include "filesystem.hpp"
#include "tec-types.hpp"

namespace tec {
class LuaSystem;
typedef Command<LuaSystem> LuaCommand;
struct EntityCreated;
struct EntityDestroyed;
struct ChatCommandEvent;

class LuaSystem :
		public CommandQueue<LuaSystem>,
		public EventQueue<EntityCreated>,
		public EventQueue<EntityDestroyed>,
		public EventQueue<ChatCommandEvent> {
public:
	LuaSystem();

	void Update(const double delta);
	void ProcessEvents();

	using EventQueue<EntityCreated>::On;
	using EventQueue<EntityDestroyed>::On;
	using EventQueue<ChatCommandEvent>::On;
	void On(std::shared_ptr<EntityCreated> data);
	void On(std::shared_ptr<EntityDestroyed> data);
	void On(std::shared_ptr<ChatCommandEvent> data);

	void ExecuteString(std::string script_string);

	std::shared_ptr<LuaScript> LoadFile(FilePath filepath);

	sol::state& GetGlobalState() { return this->lua; }

private:
	sol::state lua;
	std::list<LuaScript> scripts;

	template <typename... Types> void CallFunction(std::string, Types...);
};
} // namespace tec
