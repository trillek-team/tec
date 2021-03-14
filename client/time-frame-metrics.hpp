#pragma once

namespace tec {

struct TimeFrameMetrics {
	// Elapsed time spend on each section
	float outside_game_time;
	float state_queue_time;
	float vcomputer_time;
	float sound_system_time;
	float render_system_time;
	float lua_system_time;
	float other_time;

	float total_time;
};

} // namespace tec
