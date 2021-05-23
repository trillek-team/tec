#pragma once

#include <future>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include <vcomputer.hpp>

#include "client-game-state-queue.hpp"
#include "lua-system.hpp"
#include "multiton.hpp"
#include "os.hpp"
#include "physics-system.hpp"
#include "render-system.hpp"
#include "server-connection.hpp"
#include "simulation.hpp"
#include "sound-system.hpp"
#include "time-frame-metrics.hpp"
#include "vcomputer-system.hpp"

#include "manipulators/placement.hpp"

namespace tec {
struct FPSController;
struct KeyboardEvent;

using networking::ServerConnection;

enum ENGINE_ENTITIES { MANIPULATOR = 1 };

class Game : public EventQueue<KeyboardEvent> {
public:
	Game(OS& _os, std::string config_file_name = "scripts/config.lua");

	~Game();

	void Startup();

	void Update(double delta, double mouse_x, double mouse_y, int window_width, int window_height);

	ServerConnection& GetServerConnection() { return this->server_connection; }

	const eid GetActiveEntity() const { return this->active_entity; }

	LuaSystem* GetLuaSystem() { return &this->lua_sys; }

	void CreateEngineEntities();

	std::shared_ptr<LuaScript> config_script;

	// Frames per second
	unsigned int fps;
	float avg_frame_time;

	TimeFrameMetrics tfm;

private:
	static void UpdateVComputerScreenTextures();

	typedef Multiton<eid, Computer*> ComputerComponentMap;

	using EventQueue<KeyboardEvent>::On;
	void On(std::shared_ptr<KeyboardEvent> data);

	// Frames per second
	unsigned int frames = 0;
	double frame_deltas = 0.0;

	// Elapsed time since last call, same as OS::GetDeltaTime(), but as a float
	float GetElapsedTime();
	double last_time = 0.0;

	ServerStats stats;
	Simulation simulation;
	OS& os;
	ClientGameStateQueue game_state_queue;
	ServerConnection server_connection;
	RenderSystem rs;
	PhysicsSystem& ps;
	VComputerSystem& vcs;
	SoundSystem ss;
	LuaSystem lua_sys;

	double delta_accumulator = 0.0; // Accumulated deltas since the last update was sent.
	state_id_t command_id = 0;
	eid active_entity{-1};
	eid player_entity_id{-1};
	std::shared_ptr<tec::FPSController> player_camera{nullptr};

	std::thread sound_thread;
	std::thread* asio_thread = nullptr;
	std::thread* sync_thread = nullptr;

	manipulator::Placement placement;

	std::string placeable_meshes[10]{"bob/bob.md5mesh", "vidstand/VidStand_Full.obj", "rooms/hanger/hanger.md5mesh"};
};
} // end namespace tec
