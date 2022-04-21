#include "ui/progress.hpp"
#include "core/state.hpp"
#include "deps/mcl-utils/src/math.hpp"
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace geena::ui
{
Progress::Progress(int x, int y, int w, int h, core::CurrentState& state)
: Fl_Widget(x, y, w, h)
, m_state(state)
{
}

/* -------------------------------------------------------------------------- */

void Progress::draw()
{
	fl_rectf(x(), y(), w(), h(), fl_rgb_color(255, 255, 255));
	fl_rect(x(), y(), w(), h(), fl_rgb_color(0, 0, 0));
	fl_rectf(x(), y(), frameToPixel(m_state.position), h(), fl_rgb_color(0, 0, 0));
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

void Progress::refresh(core::CurrentState& state)
{
	m_state = state;
	redraw();
}

/* -------------------------------------------------------------------------- */

int Progress::frameToPixel(Frame f)
{
	return mcl::utils::math::map(f, m_state.audioFileLength, w());
}

Frame Progress::pixelToFrame(int p)
{
	return mcl::utils::math::map(p, w(), m_state.audioFileLength);
}
} // namespace geena::ui