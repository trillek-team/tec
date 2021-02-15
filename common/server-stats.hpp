#pragma once

#include <cinttypes>
#include <memory>
#include <atomic>
#include <chrono>

#include <glm/glm.hpp>
#include "tec-types.hpp"

namespace tec {

class ServerStats {
public:
	uint64_t estimated_server_time;
	uint64_t last_state_time;
	size_t server_state_count;
	state_id_t current_command_id;
	state_id_t current_acked_id;
	glm::vec3 server_position;
	glm::vec3 server_position_next;
	glm::vec3 client_position;
	glm::vec3 client_velocity;
	uint64_t estimated_delay;
	uint64_t estimated_delay_accumulator;
	size_t estimated_delay_count;
};

} // end namespace tec
