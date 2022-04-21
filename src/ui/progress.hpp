#pragma once

#include "types.hpp"
#include <FL/Fl_Widget.H>
#include <functional>

namespace geena::core
{
struct CurrentState;
}

namespace geena::ui
{
class Progress : public Fl_Widget
{
public:
	Progress(int x, int y, int w, int h, core::CurrentState&);

	void draw() override;
	int  handle(int event) override;

	void refresh(core::CurrentState&);

	std::function<void(Frame)> onClick = {nullptr};

private:
	int   frameToPixel(Frame f);
	Frame pixelToFrame(int p);

	core::CurrentState& m_state;
};
} // namespace geena::ui