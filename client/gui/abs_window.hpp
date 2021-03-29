#pragma once

#include <string>

/**
 * Abstract base class for GUI windows
 */

namespace tec {
class IMGUISystem;

class AbstractWindow {
public:
	virtual ~AbstractWindow() = default;

	virtual void Update(double){};

	virtual void Draw(IMGUISystem*) = 0;

	void SetAlpha(float a) {
		a = (a < 0) ? 0 : a; // Clamp [0, 1.0]
		a = (a > 1.0f) ? 1.0f : a;
		this->alpha = a;
	}

	float GetAlpha() const { return alpha; }

	virtual bool isCollapsed() const { return collapsed; }

	bool isVisible() const { return show; }

	void Hide() { show = false; }

	void Show() { show = true; }

	std::string GetWindowName() { return this->window_name; }

protected:
	float alpha = 1.0f;
	bool collapsed = true; /// Collapsed ?
	bool show = true; /// Visible ?
	std::string window_name;
};
} // namespace tec
