#pragma once

namespace tec::graphics {

struct Viewport {
	glm::uvec2 view_size;
	glm::vec2 inv_view_size;
	glm::mat4 projection;
};

} // namespace tec::graphics
