#include "active-entity-tooltip.hpp"
#include "game.hpp"

namespace tec {
ActiveEntityTooltip::ActiveEntityTooltip(Game& game) : game(game) { this->window_name = "active_entity"; }

void ActiveEntityTooltip::Update(double) {}

void ActiveEntityTooltip::Draw(IMGUISystem*) {
	const eid active_entity = game.GetActiveEntity();
	if (active_entity > -1) {
		ImGui::SetTooltip("#%" PRI_EID, active_entity);
	}
}
} // namespace tec
