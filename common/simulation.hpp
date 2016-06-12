// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <memory>
#include <queue>

#include "types.hpp"
#include "game-state.hpp"
#include "physics-system.hpp"
#include "vcomputer-system.hpp"
#include "components.pb.h"
#include "event-queue.hpp"

namespace tec {
	extern double UPDATE_RATE;

	struct Controller;

	extern std::map<tid, std::function<void(const proto::Entity&, const proto::Component&)>> in_functors;

	class Simulation final : public CommandQueue < Simulation >,
		public EventQueue<KeyboardEvent>, public EventQueue<MouseBtnEvent>,
		public EventQueue<MouseMoveEvent>, public EventQueue<MouseClickEvent>,
		public EventQueue<EntityCreated>, public EventQueue<EntityUpdated>,
		public EventQueue<EntityDestroyed>  {
	public:
		Simulation() : last_server_state_id(0) { }
		~Simulation() { }

		std::set<eid> Simulate(const double delta_time);

		void Interpolate(const double delta_time);

		PhysicsSystem& GetPhysicsSystem() {
			return this->phys_sys;
		}

		VComputerSystem& GetVComputerSystem() {
			return this->vcomp_sys;
		}

		GameState& GetClientState();

		void AddController(Controller* controller);

		void On(std::shared_ptr<KeyboardEvent> data);
		void On(std::shared_ptr<MouseBtnEvent> data);
		void On(std::shared_ptr<MouseMoveEvent> data);
		void On(std::shared_ptr<MouseClickEvent> data);
		void On(std::shared_ptr<EntityCreated> data);
		void On(std::shared_ptr<EntityUpdated> data);
		void On(std::shared_ptr<EntityDestroyed> data);

		void SetEntityState(proto::Entity& entity);
		void PushServerState(GameState&& new_frame);

	private:
		void onSetEntityState(const proto::Entity& entity);
		void onRemoveEntity(const eid entity_id);

		PhysicsSystem phys_sys;
		VComputerSystem vcomp_sys;

		std::queue<GameState> server_states;
		std::mutex server_state_mutex;
		CommandList current_command_list;
		GameState client_state; // Current client (interpolated) state
		GameState base_state; // The base state we interpolate client state towards server_states.being()
		state_id_t last_server_state_id; // Stores the last server state id (no need for a mutex around server_states by caching).
		double interpolation_accumulator = 0.0;
		std::list<Controller*> controllers;
	};
}
