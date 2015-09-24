#pragma once
/**
 * Window that display the entities on game and allow to add/remove entities and components
 */

#include "gui/abs_window.hpp"

namespace tec {

	class EntityTree : public AbstractWindow {
	public:
		
		void Draw();
		
	private:
		static const int MAX_COMBO_ITEM_SLOTS = 10;
		
		int current_combo_item_slot = 0;
		int current_combo_item[MAX_COMBO_ITEM_SLOTS] = {0};
	};

}
