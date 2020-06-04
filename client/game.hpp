#pragma once

#include <future>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include <vcomputer.hpp>

#include "server-connection.hpp"
#include "lua-system.hpp"
#include "physics-system.hpp"
#include "render-system.hpp"
#include "simulation.hpp"
#include "game-state-queue.hpp"
#include "sound-system.hpp"
#include "vcomputer-system.hpp"
#include "multiton.hpp"

namespace tec {
	struct FPSController;

	using networking::ServerConnection;

	class Game {
	public:
		Game(const unsigned int viewport_width, const unsigned int viewport_height);

		~Game();

		void Update(double delta, double mouse_x, double mouse_y, int window_width, int window_height);

		ServerConnection& GetServerConnection() {
			return this->server_connection;
		}

		const eid GetActiveEntity() const {
			return this->active_entity;
		}

		LuaSystem* GetLuaSystem() {
			return &this->lua_sys;
		}

	private:
		static void UpdateVComputerScreenTextures();

		typedef Multiton<eid, Computer*> ComputerComponentMap;

		Simulation simulation;
		GameStateQueue game_state_queue;
		ServerConnection server_connection;
		RenderSystem rs;
		VComputerSystem& vcs;
		PhysicsSystem& ps;
		SoundSystem ss;
		LuaSystem lua_sys;

		double delta_accumulator = 0.0; // Accumulated deltas since the last update was sent.
		state_id_t command_id = 0;
		eid active_entity{ -1 };
		std::shared_ptr<tec::FPSController> player_camera;

		std::thread sound_thread;
		std::thread* asio_thread = nullptr;
		std::thread* sync_thread = nullptr;
	};
}
