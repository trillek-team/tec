#include "active-entity-tooltip.hpp"
#include "game.hpp"

namespace tec {
	ActiveEntityTooltip::ActiveEntityTooltip(Game& game) : game(game) {}

	void ActiveEntityTooltip::Update(double) {}

	void ActiveEntityTooltip::Draw() {
		const eid active_entity = game.GetActiveEntity();
		if (active_entity > -1) {
			ImGui::SetTooltip("#%" PRI_EID, active_entity);
		}
	}
}
