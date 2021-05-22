#include "progress.hpp"
#include "utils/math.hpp"
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace geena::ui
{
Progress::Progress(int x, int y, int w, int h)
: Fl_Widget(x, y, w, h)
, m_position(0)
, m_length(0)
{
}

/* -------------------------------------------------------------------------- */

void Progress::draw()
{
	fl_rectf(x(), y(), w(), h(), fl_rgb_color(255, 255, 255));
	fl_rect(x(), y(), w(), h(), fl_rgb_color(0, 0, 0));
	fl_rectf(x(), y(), frameToPixel(m_position), h(), fl_rgb_color(0, 0, 0));
}

/* -------------------------------------------------------------------------- */

int Progress::handle(int event)
{
	switch (event)
	{
	case FL_PUSH:
		if (onClick != nullptr)
			onClick(pixelToFrame(Fl::event_x() - x()));
		return 1;
	default:
		break;
	}

	return Fl_Widget::handle(event);
}

/* -------------------------------------------------------------------------- */

void Progress::refresh(Frame position, Frame length)
{
	m_position = position;
	m_length   = length;

	redraw();
}

/* -------------------------------------------------------------------------- */

int Progress::frameToPixel(Frame f)
{
	return utils::math::map(f, m_length, w());
}

Frame Progress::pixelToFrame(int p)
{
	return utils::math::map(p, w(), m_length);
}
} // namespace geena::ui