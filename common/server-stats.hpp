#ifndef TRILLEK_CLIENT_SERVER_STATS_HPP
#define TRILLEK_CLIENT_SERVER_STATS_HPP

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
	glm::vec3 client_entity_position;
};

}

#endif
