#pragma once
/**
 * Lua system
 */

#include <limits>
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

struct LuaClassList;

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

	template <typename... Args> void CallFunctions(std::string function_name, Args&&... args) {
		for (auto& fn : GetAllFunctions(function_name)) {
			fn(args...);
		}
	}

private:
	sol::state lua;
	std::list<LuaScript> scripts;

	std::list<sol::protected_function> GetAllFunctions(std::string);

	friend LuaClassList;
	static LuaClassList* lua_userclasses;
};

// a linked list of functions which get called when lua system is initialized
struct LuaClassList {
	std::function<void(sol::state&)> load;
	LuaClassList* next;
	LuaClassList(std::function<void(sol::state&)> load_fn) : load(load_fn), next(nullptr) {
		this->next = LuaSystem::lua_userclasses;
		LuaSystem::lua_userclasses = this;
	}
};

// this helper macro does the setup to add types to the linked list
// it is also the start of the function definition for RegisterLuaType
// this should be ideally be used one of the following places:
// common/lua-system.cpp client/lua-types.cpp server/lua-types.cpp
// this could be used anywhere, but it's not recommended
// types registered this way will need a forward declaration (preferably in a header)
#define TEC_RegisterLuaType(ns, a) \
	static tec::LuaClassList LUA_USERCLASS_##a(ns::a::RegisterLuaType); \
	void ns::a::RegisterLuaType(sol::state& state)

} // namespace tec
