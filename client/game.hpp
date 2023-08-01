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
#include "voxel-volume.hpp"

#include "manipulators/placement.hpp"

namespace tec {
struct FPSController;
struct KeyboardEvent;
class Console;

using networking::ServerConnection;

enum ENGINE_ENTITIES { MANIPULATOR = 1 };

class Game : public EventQueue<KeyboardEvent>, public EventQueue<MouseClickEvent> {
public:
	Game(OS& _os, std::string config_file_name = "scripts/config.lua");

	~Game();

	void Startup(Console&);

	void Update(double delta);

	ServerConnection& GetServerConnection() { return this->server_connection; }

	const eid GetActiveEntity() const { return this->active_entity; }

	LuaSystem* GetLuaSystem() { return &this->lua_sys; }

	std::shared_ptr<LuaScript> config_script;

	// Frames per second
	unsigned int fps{};
	float avg_frame_time{};

	TimeFrameMetrics tfm{};

private:
	static void UpdateVComputerScreenTextures();

	typedef Multiton<eid, Computer*> ComputerComponentMap;

	void ProcessEvents();

	void On(eid, std::shared_ptr<KeyboardEvent> data) override;
	void On(eid, std::shared_ptr<MouseClickEvent> data) override;

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
	VoxelSystem vox_sys;

	double delta_accumulator = 0.0; // Accumulated deltas since the last update was sent.
	state_id_t command_id = 0;
	eid active_entity{0};
	eid player_entity_id{0};
	std::shared_ptr<tec::FPSController> player_camera{nullptr};

	std::thread sound_thread;
	std::thread* asio_thread = nullptr;

	manipulator::Placement placement;
};
} // end namespace tec
