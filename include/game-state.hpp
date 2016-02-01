#pragma once

#include <map>
#include <list>
#include "types.hpp"
#include "proto/game_state.pb.h"
#include "components/transforms.hpp"
#include "components/velocity.hpp"

namespace tec {
	struct GameState {
		std::unordered_map<eid, Position> positions;
		std::unordered_map<eid, Orientation> orientations;
		std::unordered_map<eid, Velocity> velocties;
		std::list<std::function<void(void)>> commands;

		GameState() : state_id(0) { }

		GameState(const GameState&) = delete;
		GameState(GameState&& other) {
			this->positions = std::move(other.positions);
			this->orientations = std::move(other.orientations);
			this->velocties = std::move(other.velocties);
			this->state_id = other.state_id;
		}

		GameState& operator=(const GameState& other) = delete;
		GameState& operator=(GameState&& other) {
			if (this != &other) {
				this->positions = std::move(other.positions);
				this->orientations = std::move(other.orientations);
				this->velocties = std::move(other.velocties);
				this->state_id = other.state_id;
			}
			return *this;
		}

		void In(const proto::GameStateUpdate& gsu) {
			this->state_id = gsu.state_id();
			for (int e = 0; e < gsu.entity_size(); ++e) {
				const proto::Entity& entity = gsu.entity(e);
				eid entity_id = entity.id();
				for (int i = 0; i < entity.components_size(); ++i) {
					const proto::Component& comp = entity.components(i);
					switch (comp.component_case()) {
						case proto::Component::kPosition:
							{
								Position pos;
								pos.In(comp);
								this->positions[entity_id] = pos;
							}
							break;
						case proto::Component::kOrientation:
							{
								Orientation orientation;
								orientation.In(comp);
								this->orientations[entity_id] = orientation;
							}
							break;
						case proto::Component::kVelocity:
							{
								Velocity vel;
								vel.In(comp);
								this->velocties[entity_id] = vel;
							}
							break;
					}
				}
			}
		}

		void Out(proto::GameStateUpdate* gsu) const {
			gsu->set_state_id(this->state_id);
			for (auto pos : this->positions) {
				tec::proto::Entity* entity = gsu->add_entity();
				entity->set_id(pos.first);
				pos.second.Out(entity->add_components());
				if (this->orientations.find(pos.first) != this->orientations.end()) {
					tec::Orientation ori = this->orientations.at(pos.first);
					ori.Out(entity->add_components());
				}
				if (this->velocties.find(pos.first) != this->velocties.end()) {
					tec::Velocity vel = this->velocties.at(pos.first);
					vel.Out(entity->add_components());
				}
			}
		}
		state_id_t state_id;
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
