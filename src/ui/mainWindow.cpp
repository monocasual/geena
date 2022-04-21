#include "ui/mainWindow.hpp"
#include "const.hpp"
#include "core/api.hpp"
#include "core/state.hpp"
#include "deps/mcl-fl_flex/src/fl_flex.hpp"
#include "deps/mcl-utils/src/log.hpp"
#include "deps/mcl-utils/src/math.hpp"
#include "types.hpp"
#include "ui/counter.hpp"
#include "ui/pitchSlider.hpp"
#include "ui/progress.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <filesystem>
#include <iostream>

using namespace mcl;

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
} // namespace

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

MainWindow::MainWindow(int x, int y, int w, int h)
: Fl_Window(x, y, w, h)
, m_state(core::api::getCurrentState())
{
	end();

	Fl_Flex* container = new Fl_Flex(geompp::Rect(0, 0, w, h).reduced({30}), Fl_Flex::Direction::VERTICAL, 30);
	{
		m_counter  = new Counter(0, 0, 0, 0, m_state);
		m_progress = new Progress(0, 0, 0, 0, m_state);

		Fl_Flex* buttons = new Fl_Flex(Fl_Flex::Direction::HORIZONTAL, 20);
		{
			m_btn_playPause = new Fl_Button(0, 0, 0, 0, "Play/Pause");
			m_btn_rewind    = new Fl_Button(0, 0, 0, 0, "Rewind");
			m_btn_unload    = new Fl_Button(0, 0, 0, 0, "Unload");
			m_pitchSlider   = new PitchSlider(0, 0, 0, 0);
			buttons->add(m_btn_playPause, 100);
			buttons->add(m_btn_rewind, 100);
			buttons->add(m_btn_unload, 100);
			buttons->add(new Fl_Box(0, 0, 0, 0));
			buttons->add(m_pitchSlider, 25);
			buttons->end();
		}

		container->add(m_counter);
		container->add(m_progress, 20);
		container->add(buttons, 200);
		container->end();
	}

	add(container);
	resizable(container);

	Fl::add_timeout(G_UI_REFRESH_RATE, refresh_, static_cast<void*>(this));

	m_btn_playPause->callback([](Fl_Widget* /*w*/, void* /*v*/) {
		core::api::playPauseToggle();
	});

	m_btn_rewind->callback([](Fl_Widget* /*w*/, void* /*v*/) {
		core::api::rewind();
	});

	m_btn_unload->callback([](Fl_Widget* /*w*/, void* /*v*/) {
		core::api::unloadAudioFile();
	});

	m_progress->onClick = [](Frame f) {
		core::api::goToFrame(f);
	};

	m_pitchSlider->callback([](Fl_Widget* w, void* /*v*/) {
		core::api::setPitch(static_cast<PitchSlider*>(w)->value());
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
	default:
		break;
	}

	return Fl_Window::handle(event);
}

/* -------------------------------------------------------------------------- */

void MainWindow::refresh()
{
	m_state = core::api::getCurrentState();

	m_counter->refresh(m_state);
	m_progress->refresh(m_state);
}

/* -------------------------------------------------------------------------- */

int MainWindow::run()
{
	return Fl::run();
}
} // namespace geena::ui