#pragma once

#include "types.hpp"
#include <FL/Fl_Widget.H>
#include <functional>

namespace geena::ui
{
class Progress : public Fl_Widget
{
public:
	Progress(int x, int y, int w, int h);

	void draw() override;
	int  handle(int event) override;

	void refresh(Frame position, Frame length);

	std::function<void(Frame)> onClick = {nullptr};

private:
	int   frameToPixel(Frame f);
	Frame pixelToFrame(int p);

	Frame m_position;
	Frame m_length;
};
} // namespace geena::ui