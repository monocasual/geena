#pragma once

#include "types.hpp"
#include <FL/Fl_Widget.H>

namespace geena::ui
{
class Counter : public Fl_Widget
{
public:
	Counter(int x, int y, int w, int h);

	void draw() override;

	void refresh(Frame position, Frame length);

private:
	Frame m_position;
	Frame m_length;
};
} // namespace geena::ui