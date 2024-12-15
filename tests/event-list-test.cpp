#include "event-list.hpp"
#include <gtest/gtest.h>

namespace tec {

TEST(EventListTests, CollectEvents) {
	EventList event_list;

	KeyboardEvent key_event1{1, 1, KeyboardEvent::KEY_DOWN, 0};
	KeyboardEvent key_event2{1, 1, KeyboardEvent::KEY_DOWN, 0};
	MouseBtnEvent mouse_btn_event1{MouseBtnEvent::DOWN, MouseBtnEvent::LEFT};
	MouseBtnEvent mouse_btn_event2{MouseBtnEvent::DOWN, MouseBtnEvent::LEFT};
	MouseMoveEvent mouse_move_event1{0.5, 0.5, 0, 0, 100, 100};
	MouseMoveEvent mouse_move_event2{0.5, 0.5, 0, 0, 100, 100};
	MouseClickEvent mouse_click_event1{MouseBtnEvent::LEFT, {0.f, 0.f, 0.f}, 1.0};
	MouseClickEvent mouse_click_event2{MouseBtnEvent::LEFT, {0.f, 0.f, 0.f}, 1.0};

	event_list.keyboard_events.push_back(key_event1);
	event_list.keyboard_events.push_back(key_event2);
	event_list.mouse_button_events.push_back(mouse_btn_event1);
	event_list.mouse_button_events.push_back(mouse_btn_event2);
	event_list.mouse_move_events.push_back(mouse_move_event1);
	event_list.mouse_move_events.push_back(mouse_move_event2);
	event_list.mouse_click_events.push_back(mouse_click_event1);
	event_list.mouse_click_events.push_back(mouse_click_event2);

	EventList collected_events = event_list.CollectEvents();

	EXPECT_EQ(collected_events.keyboard_events.size(), 1);
	EXPECT_EQ(collected_events.mouse_button_events.size(), 1);
	EXPECT_EQ(collected_events.mouse_move_events.size(), 1);
	EXPECT_EQ(collected_events.mouse_click_events.size(), 1);
}

} // namespace tec
