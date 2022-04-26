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
, m_status(HandleStatus::NORMAL)
{
}

/* -------------------------------------------------------------------------- */

void Progress::draw()
{
	const int p = m_status == HandleStatus::NORMAL ? frameToPixel(m_state.position) : getCursorX();

	fl_rectf(x(), y(), w(), h(), fl_rgb_color(255, 255, 255));
	fl_rect(x(), y(), w(), h(), fl_rgb_color(0, 0, 0));
	fl_rectf(x(), y(), p, h(), fl_rgb_color(0, 0, 0));
}

/* -------------------------------------------------------------------------- */

int Progress::handle(int event)
{
	switch (event)
	{
	case FL_PUSH:
	case FL_DRAG:
		m_status = HandleStatus::DRAGGED;
		return 1;
	case FL_RELEASE:
		if (onClick != nullptr)
			onClick(pixelToFrame(getCursorX()));
		m_status = HandleStatus::NORMAL;
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

int Progress::frameToPixel(Frame f) const
{
	return mcl::utils::math::map(f, m_state.audioFileLength, w());
}

Frame Progress::pixelToFrame(int p) const
{
	return mcl::utils::math::map(p, w(), m_state.audioFileLength);
}

/* -------------------------------------------------------------------------- */

int Progress::getCursorX() const
{
	return std::min(std::max(0, Fl::event_x() - x()), w());
}
} // namespace geena::ui