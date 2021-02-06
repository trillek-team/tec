#pragma once

#include <memory>

#include <commands.pb.h>

#include "types.hpp"
#include "game-state.hpp"
#include "events.hpp"

namespace tec {
	// TODO: Create Controller system that calls update on all controller
	// instances.
	struct Controller {
		Controller(eid entity_id) : entity_id(entity_id) {}

		virtual void Update(double, GameState&, EventList&) {}

		virtual ~Controller() = default;

		virtual proto::ClientCommands GetClientCommands() = 0;

		virtual void ApplyClientCommands(proto::ClientCommands) = 0;

		eid entity_id;
	};

	// TODO: Remove this class as it is only for testing and should really be
	// implemented in script.
	struct FPSController : public Controller {
		FPSController(const eid entity_id) : Controller(entity_id) {}
		virtual ~FPSController() = default;

		void Handle(const KeyboardEvent& data, const GameState& state);
		void Handle(const MouseBtnEvent& data, const GameState& state);
		void Handle(const MouseMoveEvent& data, const GameState& state);

		void Update(double delta, GameState& state, EventList& commands) override;

		proto::ClientCommands GetClientCommands() override;

		bool forward{ false };
		bool backward{ false };
		bool right_strafe{ false };
		bool left_strafe{ false };

		double current_delta{ 0.0 };
		bool mouse_look{ false };

		std::unique_ptr<Orientation> orientation;

		// These tell us which was pressed first.
		bool KEY_A_FIRST{ false };
		bool KEY_W_FIRST{ false };

		// These tell us which are pressed.
		bool KEY_W_DOWN{ false };
		bool KEY_A_DOWN{ false };
		bool KEY_S_DOWN{ false };
		bool KEY_D_DOWN{ false };

		void ApplyClientCommands(proto::ClientCommands proto_client_commands) override;
	};
}
