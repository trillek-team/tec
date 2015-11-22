#pragma once

#include <map>
#include <list>
#include "types.hpp"
#include "components/transforms.hpp"
#include "components/velocity.hpp"

namespace tec {
	struct GameState {
		std::unordered_map<eid, Position> positions;
		std::unordered_map<eid, Orientation> orientations;
		std::unordered_map<eid, Velocity> velocties;
		std::list<std::function<void(void)>> commands;

		GameState() : state_sequence_number(0), delta_time(10.0) { }

		GameState(const GameState&) = delete;
		GameState(GameState&& other) {
			this->positions = std::move(other.positions);
			this->orientations = std::move(other.orientations);
			this->velocties = std::move(other.velocties);
			this->delta_time = other.delta_time;
		}

		GameState& operator=(const GameState& other) = delete;
		GameState& operator=(GameState&& other) {
			if (this != &other) {
				this->positions = std::move(other.positions);
				this->orientations = std::move(other.orientations);
				this->velocties = std::move(other.velocties);
				this->delta_time = other.delta_time;
			}
			return *this;
		}
		frame_id_t state_sequence_number;
		double delta_time;
	};

	struct CommandList {
		std::list<KeyboardEvent> keyboard_events;
		std::list<MouseBtnEvent> mouse_button_events;
		std::list<MouseMoveEvent> mouse_move_events;
		std::list<MouseClickEvent> mouse_click_events;

		CommandList() { }

		CommandList(const CommandList&) = delete;
		CommandList(CommandList&& other) {
			this->keyboard_events = std::move(other.keyboard_events);
			this->mouse_button_events = std::move(other.mouse_button_events);
			this->mouse_move_events = std::move(other.mouse_move_events);
			this->mouse_click_events = std::move(other.mouse_click_events);
		}

		CommandList& operator=(const CommandList& other) = delete;
		CommandList& operator=(CommandList&& other) {
			if (this != &other) {
				this->keyboard_events = std::move(other.keyboard_events);
				this->mouse_button_events = std::move(other.mouse_button_events);
				this->mouse_move_events = std::move(other.mouse_move_events);
				this->mouse_click_events = std::move(other.mouse_click_events);
			}
			return *this;
		}
	};
}
