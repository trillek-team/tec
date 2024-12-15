#pragma once

#include "events.hpp"
#include <list>

namespace tec {
struct EventList {
	std::list<KeyboardEvent> keyboard_events;
	std::list<MouseBtnEvent> mouse_button_events;
	std::list<MouseMoveEvent> mouse_move_events;
	std::list<MouseClickEvent> mouse_click_events;

	EventList() {}

	EventList(const EventList&) = delete;
	EventList(EventList&& other) noexcept {
		this->keyboard_events = std::move(other.keyboard_events);
		this->mouse_button_events = std::move(other.mouse_button_events);
		this->mouse_move_events = std::move(other.mouse_move_events);
		this->mouse_click_events = std::move(other.mouse_click_events);
	}

	EventList& operator=(const EventList& other) = delete;
	EventList& operator=(EventList&& other) noexcept {
		if (this != &other) {
			this->keyboard_events = std::move(other.keyboard_events);
			this->mouse_button_events = std::move(other.mouse_button_events);
			this->mouse_move_events = std::move(other.mouse_move_events);
			this->mouse_click_events = std::move(other.mouse_click_events);
		}
		return *this;
	}

	EventList CollectEvents() {
		EventList event_list;

		// Combine identical keyboard events
		for (const auto& event : keyboard_events) {
			auto it = std::find(event_list.keyboard_events.begin(), event_list.keyboard_events.end(), event);
			if (it == event_list.keyboard_events.end()) {
				event_list.keyboard_events.push_back(event);
			}
		}

		// Combine identical mouse button events
		for (const auto& event : mouse_button_events) {
			auto it = std::find(event_list.mouse_button_events.begin(), event_list.mouse_button_events.end(), event);
			if (it == event_list.mouse_button_events.end()) {
				event_list.mouse_button_events.push_back(event);
			}
		}

		// Combine identical mouse move events
		for (const auto& event : mouse_move_events) {
			auto it = std::find(event_list.mouse_move_events.begin(), event_list.mouse_move_events.end(), event);
			if (it == event_list.mouse_move_events.end()) {
				event_list.mouse_move_events.push_back(event);
			}
		}

		// Combine identical mouse click events
		for (const auto& event : mouse_click_events) {
			auto it = std::find(event_list.mouse_click_events.begin(), event_list.mouse_click_events.end(), event);
			if (it == event_list.mouse_click_events.end()) {
				event_list.mouse_click_events.push_back(event);
			}
		}

		return event_list;
	}
};
} // namespace tec
