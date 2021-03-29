#pragma once

#include <sstream>

#include <imgui.h>

#include "abs_window.hpp"

namespace tec {
class Game;
class IMGUISystem;

class ActiveEntityTooltip : public AbstractWindow {
public:
	ActiveEntityTooltip(Game& game);

	void Update(double) override;

	void Draw(IMGUISystem*) override;

private:
	Game& game;
};
} // namespace tec
