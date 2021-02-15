#pragma once

#include <memory>
#include <queue>

#include "physics-system.hpp"
#include "vcomputer-system.hpp"
#include "event-queue.hpp"

namespace tec {
	extern double UPDATE_RATE;
	extern double TICKS_PER_SECOND;

	struct Controller;

	// Predicted client state after simulation
	struct PredictedClientState {
		Position pos;
		Orientation ori;
		Velocity vel;
	};

	class Simulation final : public CommandQueue < Simulation >,
		public EventQueue<KeyboardEvent>, public EventQueue<MouseBtnEvent>,
		public EventQueue<MouseMoveEvent>, public EventQueue<MouseClickEvent>,
		public EventQueue<ClientCommandsEvent>, public EventQueue<ControllerAddedEvent>,
		public EventQueue<ControllerRemovedEvent>, public EventQueue<FocusCapturedEvent>,
		public EventQueue<FocusBlurEvent> {
	public:
		GameState Simulate(const double delta_time, GameState& interpolated_state);

		PhysicsSystem& GetPhysicsSystem() {
			return this->phys_sys;
		}

		VComputerSystem& GetVComputerSystem() {
			return this->vcomp_sys;
		}

		void AddController(Controller* controller);
		void RemoveController(Controller* controller);

		using EventQueue<KeyboardEvent>::On;
		using EventQueue<MouseBtnEvent>::On;
		using EventQueue<MouseMoveEvent>::On;
		using EventQueue<MouseClickEvent>::On;
		using EventQueue<ClientCommandsEvent>::On;
		using EventQueue<ControllerAddedEvent>::On;
		using EventQueue<ControllerRemovedEvent>::On;
		void On(std::shared_ptr<KeyboardEvent> data) override;
		void On(std::shared_ptr<MouseBtnEvent> data) override;
		void On(std::shared_ptr<MouseMoveEvent> data) override;
		void On(std::shared_ptr<MouseClickEvent> data) override;
		void On(std::shared_ptr<ClientCommandsEvent> data) override;
		void On(std::shared_ptr<ControllerAddedEvent> data) override;
		void On(std::shared_ptr<ControllerRemovedEvent> data) override;
		void On(std::shared_ptr<FocusCapturedEvent> data) override;
		void On(std::shared_ptr<FocusBlurEvent> data) override;
	private:
		PhysicsSystem phys_sys;
		VComputerSystem vcomp_sys;

		EventList event_list;

		std::list<Controller*> controllers;
	};
} // end namespace tec
