#pragma once

#include <imgui.h>

#include "abs_window.hpp"
#include "game.hpp"

namespace tec {
class Game;
class IMGUISystem;

class DebugInfo : public AbstractWindow {
public:
	DebugInfo(Game& game);

	void Update(double) override;

	void Draw(IMGUISystem*) override;

private:
	Game& game;
};
} // namespace tec
