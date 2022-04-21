#include "ui/counter.hpp"
#include "core/api.hpp"
#include "core/state.hpp"
#include "deps/mcl-utils/src/fs.hpp"
#include "deps/mcl-utils/src/string.hpp"
#include <FL/Fl.H>
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

int Counter::handle(int event)
{
	switch (event)
	{
	case FL_DND_ENTER:
	case FL_DND_DRAG:
	case FL_DND_RELEASE:
	{
		return 1; // enable dnd
	}
	case FL_PASTE: // drop (paste) operation
	{
		onFileDrop(Fl::event_text());
		return 1;
	}
	default:
		break;
	}

	return Fl_Widget::handle(event);
}

/* -------------------------------------------------------------------------- */

void Counter::onFileDrop(const char* s) const
{
	std::vector<std::string> paths = mcl::utils::string::split(s, "\n");
	std::string              path  = mcl::utils::fs::uriToPath(paths[0]);
	core::api::loadAudioFile(path);
}

/* -------------------------------------------------------------------------- */

void Counter::refresh(core::CurrentState& state)
{
	m_state = state;

	redraw();
}
} // namespace geena::ui