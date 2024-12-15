#pragma once

#include <memory>

#include <commands.pb.h>

#include "event-list.hpp"
#include "events.hpp"
#include "game-state.hpp"
#include "tec-types.hpp"

namespace tec {

// TODO: Create Controller system that calls update on all controller
// instances.
struct Controller {
	explicit Controller(const eid entity_id) : entity_id(entity_id) {}
	Controller(const Controller& other) = default;

	Controller(Controller&& other) noexcept :
			entity_id(other.entity_id), keyboard_focus(other.keyboard_focus), mouse_focus(other.mouse_focus) {}

	Controller& operator=(const Controller& other) {
		if (this == &other)
			return *this;
		entity_id = other.entity_id;
		keyboard_focus = other.keyboard_focus;
		mouse_focus = other.mouse_focus;
		return *this;
	}

	Controller& operator=(Controller&& other) noexcept {
		if (this == &other)
			return *this;
		entity_id = other.entity_id;
		keyboard_focus = other.keyboard_focus;
		mouse_focus = other.mouse_focus;
		return *this;
	}

	virtual ~Controller() = default;

	virtual void Update(double, GameState&, EventList&) {}

	virtual proto::ClientCommands GetClientCommands() = 0;

	virtual void ApplyClientCommands(proto::ClientCommands) = 0;

	/// \brief called to indicate focus has been restored to controller
	virtual void SetFocus(bool keyboard, bool mouse) {
		this->keyboard_focus = keyboard || this->keyboard_focus;
		this->mouse_focus = mouse || this->mouse_focus;
	}

	/// \brief called to indicate focus has been captured from controller
	virtual void ClearFocus(bool keyboard, bool mouse) {
		this->keyboard_focus = this->keyboard_focus && !keyboard;
		this->mouse_focus = this->mouse_focus && !mouse;
	}

	eid entity_id;
	bool keyboard_focus = true;
	bool mouse_focus = true;
};

// TODO: Remove this class as it is only for testing and should really be
// implemented in script.
struct FPSController : public Controller {
	FPSController(const eid _entity_id) : Controller(_entity_id) {}
	~FPSController() override = default;

	FPSController(const FPSController& other) = default;

	FPSController(FPSController&& other) noexcept :
			Controller(std::move(other)), forward(other.forward), backward(other.backward),
			right_strafe(other.right_strafe), left_strafe(other.left_strafe), current_delta(other.current_delta),
			mouse_look(other.mouse_look), orientation(other.orientation), KEY_A_FIRST(other.KEY_A_FIRST),
			KEY_W_FIRST(other.KEY_W_FIRST), KEY_W_DOWN(other.KEY_W_DOWN), KEY_A_DOWN(other.KEY_A_DOWN),
			KEY_S_DOWN(other.KEY_S_DOWN), KEY_D_DOWN(other.KEY_D_DOWN) {}

	FPSController& operator=(const FPSController& other) {
		if (this == &other)
			return *this;
		Controller::operator=(other);
		forward = other.forward;
		backward = other.backward;
		right_strafe = other.right_strafe;
		left_strafe = other.left_strafe;
		current_delta = other.current_delta;
		mouse_look = other.mouse_look;
		orientation = other.orientation;
		KEY_A_FIRST = other.KEY_A_FIRST;
		KEY_W_FIRST = other.KEY_W_FIRST;
		KEY_W_DOWN = other.KEY_W_DOWN;
		KEY_A_DOWN = other.KEY_A_DOWN;
		KEY_S_DOWN = other.KEY_S_DOWN;
		KEY_D_DOWN = other.KEY_D_DOWN;
		return *this;
	}

	FPSController& operator=(FPSController&& other) noexcept {
		if (this == &other)
			return *this;
		forward = other.forward;
		backward = other.backward;
		right_strafe = other.right_strafe;
		left_strafe = other.left_strafe;
		current_delta = other.current_delta;
		mouse_look = other.mouse_look;
		orientation = other.orientation;
		KEY_A_FIRST = other.KEY_A_FIRST;
		KEY_W_FIRST = other.KEY_W_FIRST;
		KEY_W_DOWN = other.KEY_W_DOWN;
		KEY_A_DOWN = other.KEY_A_DOWN;
		KEY_S_DOWN = other.KEY_S_DOWN;
		KEY_D_DOWN = other.KEY_D_DOWN;
		Controller::operator=(std::move(other));
		return *this;
	}

	void Handle(const KeyboardEvent& data, const GameState& state);
	void Handle(const MouseBtnEvent& data, const GameState& state);
	void Handle(const MouseMoveEvent& data, const GameState& state);

	void Update(double delta, GameState& state, EventList& commands) override;

	proto::ClientCommands GetClientCommands() override;

	bool forward{false};
	bool backward{false};
	bool right_strafe{false};
	bool left_strafe{false};

	double current_delta{0.0};
	bool mouse_look{false};

	Orientation orientation;

	// These tell us which was pressed first.
	bool KEY_A_FIRST{false};
	bool KEY_W_FIRST{false};

	// These tell us which are pressed.
	bool KEY_W_DOWN{false};
	bool KEY_A_DOWN{false};
	bool KEY_S_DOWN{false};
	bool KEY_D_DOWN{false};

	void ApplyClientCommands(proto::ClientCommands proto_client_commands) override;
};

} // end namespace tec
