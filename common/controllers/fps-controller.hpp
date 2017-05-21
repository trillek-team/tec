// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <memory>
#include "types.hpp"
#include "game-state.hpp"
#include "commands.pb.h"
#include "events.hpp"

namespace tec {
	// TODO: Create Controller system that calls update on all controller
	// instances.
	struct Controller {
		Controller(eid entity_id) : entity_id(entity_id) { }

		virtual void Update(double delta, const GameState& state,
			EventList& commands) { }

		virtual ~Controller() = default;

		virtual proto::ClientCommands GetClientCommands() = 0;

		virtual void ApplyClientCommands(proto::ClientCommands) = 0;

		eid entity_id;
	};

	// TODO: Remove this class as it is only for testing and should really be
	// implemented in script.
	struct FPSController : public Controller {
		FPSController(eid entity_id) : Controller(entity_id), mouse_look(false)
		{ }
		virtual ~FPSController() = default;

		void Handle(const KeyboardEvent& data, const GameState& state);
		void Handle(const MouseBtnEvent& data, const GameState& state);
		void Handle(const MouseMoveEvent& data, const GameState& state);

		void Update(double delta, const GameState& state,
			EventList& commands);

		proto::ClientCommands GetClientCommands();

		bool forward = false;
		bool backward = false;
		bool right_strafe = false;
		bool left_strafe = false;

		double current_delta;
		bool mouse_look;

		// These tell us which was pressed first.
		bool KEY_A_FIRST = false;
		bool KEY_W_FIRST = false;

		// These tell us which are pressed.
		bool KEY_W_DOWN = false;
		bool KEY_A_DOWN = false;
		bool KEY_S_DOWN = false;
		bool KEY_D_DOWN = false;

		void ApplyClientCommands(proto::ClientCommands proto_client_commands) override;
	};
}
