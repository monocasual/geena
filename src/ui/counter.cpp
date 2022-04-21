#include "ui/counter.hpp"
#include "core/state.hpp"
#include <FL/fl_draw.H>
#include <string>

namespace geena::ui
{
Counter::Counter(int x, int y, int w, int h, core::CurrentState& state)
: Fl_Widget(x, y, w, h)
, m_state(state)
{
}

/* -------------------------------------------------------------------------- */

void Counter::draw()
{
	float percent = m_state.audioFileLength > 0 ? (m_state.position / (float)m_state.audioFileLength) * 100 : 0;

	std::string s = std::to_string(m_state.position) +
	                " / " + std::to_string(m_state.audioFileLength) +
	                " - " + std::to_string(percent) + " %" +
	                " --- pitch: " + std::to_string(m_state.pitch);

	fl_rectf(x(), y(), w(), h(), fl_rgb_color(255, 255, 255));
	fl_rect(x(), y(), w(), h(), fl_rgb_color(0, 0, 0));

	fl_color(fl_rgb_color(0, 0, 0));
	fl_draw(s.c_str(), x(), y(), w(), h(), FL_ALIGN_CENTER);
}

/* -------------------------------------------------------------------------- */

void Counter::refresh(core::CurrentState& state)
{
	m_state = state;

	redraw();
}
} // namespace geena::ui