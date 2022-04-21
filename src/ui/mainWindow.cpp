#include "mainWindow.hpp"

#include "const.hpp"
#include "core/api.hpp"
#include "deps/mcl-utils/src/fs.hpp"
#include "deps/mcl-utils/src/log.hpp"
#include "deps/mcl-utils/src/math.hpp"
#include "deps/mcl-utils/src/string.hpp"
#include "types.hpp"
#include <FL/Fl.H>
#include <filesystem>
#include <iostream>

namespace geena::ui
{
namespace
{
bool keyPressed_ = false;

/* -------------------------------------------------------------------------- */

void refresh_(void* w)
{
	MainWindow* window = static_cast<MainWindow*>(w);
	window->refresh();
	Fl::repeat_timeout(G_UI_REFRESH_RATE, refresh_, static_cast<void*>(window));
}

/* -------------------------------------------------------------------------- */

void onKeyDown_(int key)
{
	if (key == FL_Up)
		core::api::setPitch(PitchDir::UP);
	else if (key == FL_Down)
		core::api::setPitch(PitchDir::DOWN);
	else if (key == FL_Left)
		core::api::nudgePitch_begin(PitchDir::DOWN);
	else if (key == FL_Right)
		core::api::nudgePitch_begin(PitchDir::UP);
}

/* -------------------------------------------------------------------------- */

void onKeyUp_(int key)
{
	if (key == FL_Left || key == FL_Right)
		core::api::nudgePitch_end();
}

/* -------------------------------------------------------------------------- */

void onFileDrop_(const char* s)
{
	std::vector<std::string> paths = mcl::utils::string::split(s, "\n");
	std::string              path  = mcl::utils::fs::uriToPath(paths[0]);
	core::api::loadAudioFile(path);
}
} // namespace

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

MainWindow::MainWindow(int x, int y, int w, int h)
: Fl_Window(x, y, w, h)
, m_btn_playPause(8, 216, 100, 100, "Play/Pause")
, m_btn_rewind(118, 216, 100, 100, "Rewind")
, m_btn_unload(228, 216, 100, 100, "Unload")
, m_counter(8, 8, w - 16, 100)
, m_progress(8, 118, w - 16, 20)
, m_pitchSlider(w - 28, 216, 20, 200)
{
	end();

	Fl::add_timeout(G_UI_REFRESH_RATE, refresh_, static_cast<void*>(this));

	m_btn_playPause.callback([](Fl_Widget* /*w*/, void* /*v*/) {
		core::api::playPauseToggle();
	});

	m_btn_rewind.callback([](Fl_Widget* /*w*/, void* /*v*/) {
		core::api::rewind();
	});

	m_btn_unload.callback([](Fl_Widget* /*w*/, void* /*v*/) {
		core::api::unloadAudioFile();
	});

	m_progress.onClick = [](Frame f) {
		core::api::goToFrame(f);
	};

	m_pitchSlider.callback([](Fl_Widget* w, void* /*v*/) {
		const float v = static_cast<PitchSlider*>(w)->value();
		core::api::setPitch(mcl::utils::math::map(v, G_MIN_PITCH, G_MAX_PITCH));
	});

	show();
}

/* -------------------------------------------------------------------------- */

int MainWindow::handle(int event)
{
	switch (event)
	{
	case FL_FOCUS:
	case FL_UNFOCUS:
	{
		return 1; // Enables receiving keyboard events
	}
	case FL_KEYDOWN:
	{
		if (keyPressed_)
			break;
		onKeyDown_(Fl::event_key());
		keyPressed_ = true;
		return 1;
	}
	case FL_KEYUP:
	{
		onKeyUp_(Fl::event_key());
		keyPressed_ = false;
		return 1;
	}
	case FL_DND_ENTER:
	case FL_DND_DRAG:
	case FL_DND_RELEASE:
	{
		return 1; // enable dnd
	}
	case FL_PASTE: // drop (paste) operation
	{
		onFileDrop_(Fl::event_text());
		return 1;
	}
	default:
		break;
	}

	return Fl_Window::handle(event);
}

/* -------------------------------------------------------------------------- */

void MainWindow::refresh()
{
	Frame position = core::api::getCurrentPosition();
	Frame length   = core::api::getAudioFileLength();
	m_counter.refresh(position, length);
	m_progress.refresh(position, length);
}

/* -------------------------------------------------------------------------- */

int MainWindow::run()
{
	return Fl::run();
}
} // namespace geena::ui