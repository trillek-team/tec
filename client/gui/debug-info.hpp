#pragma once

#include <imgui.h>

#include "abs_window.hpp"
#include "game.hpp"

namespace tec {
class Game;

class DebugInfo : public AbstractWindow {
public:
	DebugInfo(Game& game);

	void Update(double) override;

	void Draw() override;

private:
	Game& game;
};
} // namespace tec
