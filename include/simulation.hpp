#pragma once
#include <memory>
#include <map>

#include "types.hpp"
#include "game-state.hpp"
#include "physics-system.hpp"
#include "vcomputer-system.hpp"
#include "proto/components.pb.h"

namespace tec {
	struct Controller;

	class Simulation final : public EventQueue<KeyboardEvent>, public EventQueue<MouseBtnEvent>,
		public EventQueue<MouseMoveEvent>, public EventQueue < MouseClickEvent > {
	public:
		Simulation() : current_frame_id(0), last_server_frame_id(0) { }
		~Simulation() { }

		void Simulate(const double delta_time);

		void Interpolate(const double delta_time);

		// Used for testing only.
		void PopulateBaseState();

		PhysicsSystem& GetPhysicsSystem() {
			return this->phys_sys;
		}

		VComputerSystem& GetVComputerSystem() {
			return this->vcomp_sys;
		}

		const GameState& GetClientState();

		void AddController(Controller* controller);
		void On(std::shared_ptr<KeyboardEvent> data);
		void On(std::shared_ptr<MouseBtnEvent> data);
		void On(std::shared_ptr<MouseMoveEvent> data);
		void On(std::shared_ptr<MouseClickEvent> data);

	private:
		PhysicsSystem phys_sys;
		VComputerSystem vcomp_sys;
		CommandList current_command_list;
		GameState client_state; // Post-simulation state (for rendering or such)
		frame_id_t current_frame_id;
		std::list<Controller*> controllers;
	};
}
