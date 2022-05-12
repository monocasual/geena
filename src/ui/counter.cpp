#include "ui/counter.hpp"
#include "core/api.hpp"
#include "core/state.hpp"
#include "deps/mcl-utils/src/fs.hpp"
#include "deps/mcl-utils/src/math.hpp"
#include "deps/mcl-utils/src/string.hpp"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <fmt/core.h>
#include <string>

using namespace mcl;

namespace geena::ui
{
Counter::Counter(int x, int y, int w, int h, core::CurrentState& state)
: Fl_Flex(x, y, w, h, Fl_Flex::Direction::VERTICAL, 20)
, m_state(state)
{
	m_title = new Fl_Box(0, 0, 0, 0, "Title");
	m_time  = new Fl_Box(0, 0, 0, 0, "Time");
	m_pitch = new Fl_Box(0, 0, 0, 0, "Pitch");
	add(m_title);
	add(m_time);
	add(m_pitch);
	end();
}

/* -------------------------------------------------------------------------- */

void Counter::draw()
{
	const float percentTime  = m_state.audioFileLength > 0 ? (m_state.position / (float)m_state.audioFileLength) * 100 : 0;
	const float percentPitch = utils::math::map(m_state.pitch, 1.0f, 2.0f, 0.0, 100.0);

	const std::string title = m_state.audioFilePath;
	const std::string time  = fmt::format("{} / {} - {:.1f}%", m_state.position, m_state.audioFileLength, percentTime);
	const std::string pitch = fmt::format("Pitch: {} | {:.2f}%", m_state.pitch, percentPitch);

	m_title->copy_label(title.c_str());
	m_time->copy_label(time.c_str());
	m_pitch->copy_label(pitch.c_str());

	fl_rectf(x(), y(), w(), h(), fl_rgb_color(255, 255, 255));
	fl_rect(x(), y(), w(), h(), fl_rgb_color(0, 0, 0));

	draw_children();
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