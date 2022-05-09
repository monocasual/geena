#include "ui/mainWindow.hpp"
#include "const.hpp"
#include "core/api.hpp"
#include "core/state.hpp"
#include "deps/mcl-fl_flex/src/fl_flex.hpp"
#include "deps/mcl-utils/src/log.hpp"
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
void refresh_(void* w)
{
	MainWindow* window = static_cast<MainWindow*>(w);
	window->refresh();
	Fl::repeat_timeout(G_UI_REFRESH_RATE, refresh_, static_cast<void*>(window));
}
} // namespace

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

MainWindow::MainWindow(int x, int y, int w, int h)
: Fl_Window(x, y, w, h)
, m_state(core::api::getCurrentState())
, m_keyPressed(false)
{
	end();

	Fl::visible_focus(0);

	Fl_Flex* container = new Fl_Flex(geompp::Rect(0, 0, w, h).reduced({20}), Fl_Flex::Direction::VERTICAL, 20);
	{
		Fl_Flex* top = new Fl_Flex(Fl_Flex::Direction::HORIZONTAL, 20);
		{
			m_btn_unload = new Fl_Button(0, 0, 0, 0, "Unload");
			top->add(new Fl_Box(0, 0, 0, 0));
			top->add(m_btn_unload, 100);
			top->end();
		}

		m_counter  = new Counter(0, 0, 0, 0, m_state);
		m_progress = new Progress(0, 0, 0, 0, m_state);

		Fl_Flex* buttons = new Fl_Flex(Fl_Flex::Direction::HORIZONTAL, 20);
		{
			Fl_Flex* play = new Fl_Flex(Fl_Flex::Direction::VERTICAL, 20);
			{
				m_btn_rewind    = new Fl_Button(0, 0, 0, 0, "|<< Rewind");
				m_btn_cue       = new Fl_Button(0, 0, 0, 0, "Cue");
				m_btn_playPause = new Fl_Button(0, 0, 0, 0, "Play/Pause");
				play->add(m_btn_rewind, 30);
				play->add(m_btn_cue);
				play->add(m_btn_playPause);
				play->end();
			}

			m_btn_nudgeDown = new Fl_Button(0, 0, 0, 0, "<<");
			m_btn_nudgeUp   = new Fl_Button(0, 0, 0, 0, ">>");
			m_pitchSlider   = new PitchSlider(0, 0, 0, 0);

			Fl_Flex* resetPitch = new Fl_Flex(Fl_Flex::Direction::VERTICAL, 30);
			{
				m_btn_resetPitch = new Fl_Button(0, 0, 0, 0, "R");
				resetPitch->add(new Fl_Box(0, 0, 0, 0));
				resetPitch->add(m_btn_resetPitch, 30);
				resetPitch->add(new Fl_Box(0, 0, 0, 0));
			}

			buttons->add(play, 120);
			buttons->add(new Fl_Box(0, 0, 0, 0));
			buttons->add(m_btn_nudgeDown, 90);
			buttons->add(m_btn_nudgeUp, 90);
			buttons->add(new Fl_Box(0, 0, 0, 0));
			buttons->add(resetPitch, 30);
			buttons->add(m_pitchSlider, 30);
			buttons->end();
		}

		container->add(top, 30);
		container->add(m_counter);
		container->add(m_progress, 30);
		container->add(buttons, 350);
		container->end();
	}

	add(container);
	resizable(container);

	Fl::add_timeout(G_UI_REFRESH_RATE, refresh_, static_cast<void*>(this));

	m_btn_playPause->callback([](Fl_Widget* /*w*/, void* /*v*/) {
		core::api::playPauseToggle();
	});

	m_btn_cue->when(FL_WHEN_CHANGED);
	m_btn_cue->callback([](Fl_Widget* w, void* /*v*/) {
		core::api::setCue(static_cast<Fl_Button*>(w)->value());
	});

	m_btn_rewind->callback([](Fl_Widget* /*w*/, void* /*v*/) {
		core::api::rewind();
	});

	m_btn_unload->callback([](Fl_Widget* /*w*/, void* /*v*/) {
		core::api::unloadAudioFile();
	});

	m_btn_nudgeDown->when(FL_WHEN_CHANGED);
	m_btn_nudgeDown->callback([](Fl_Widget* w, void* /*v*/) {
		if (static_cast<Fl_Button*>(w)->value())
			core::api::nudgePitch_begin(PitchDir::DOWN);
		else
			core::api::nudgePitch_end();
	});

	m_btn_nudgeUp->when(FL_WHEN_CHANGED);
	m_btn_nudgeUp->callback([](Fl_Widget* w, void* /*v*/) {
		if (static_cast<Fl_Button*>(w)->value())
			core::api::nudgePitch_begin(PitchDir::UP);
		else
			core::api::nudgePitch_end();
	});

	m_progress->onClick = [](Frame f) {
		core::api::goToFrame(f);
	};

	m_pitchSlider->callback([](Fl_Widget* w, void* /*v*/) {
		core::api::setPitch(static_cast<PitchSlider*>(w)->value());
	});

	m_btn_resetPitch->callback([](Fl_Widget* /*w*/, void* v) {
		MainWindow* mainWindow = static_cast<MainWindow*>(v);
		core::api::setPitch(G_DEFAULT_PITCH);
		mainWindow->m_pitchSlider->value(G_DEFAULT_PITCH);
	},
	    (void*)this);

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
	case FL_SHORTCUT: // In case widget that isn't ours has focus
	case FL_KEYDOWN:
	{
		if (m_keyPressed)
			break;
		onKeyDown(Fl::event_key());
		m_keyPressed = true;
		return 1;
	}
	case FL_KEYUP:
	{
		onKeyUp(Fl::event_key());
		m_keyPressed = false;
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

/* -------------------------------------------------------------------------- */

void MainWindow::onKeyDown(int key)
{
	if (key == FL_Up)
		m_pitchSlider->value(core::api::setPitch(PitchDir::UP));
	else if (key == FL_Down)
		m_pitchSlider->value(core::api::setPitch(PitchDir::DOWN));
	else if (key == FL_Left)
		core::api::nudgePitch_begin(PitchDir::DOWN);
	else if (key == FL_Right)
		core::api::nudgePitch_begin(PitchDir::UP);
}

/* -------------------------------------------------------------------------- */

void MainWindow::onKeyUp(int key)
{
	if (key == FL_Left || key == FL_Right)
		core::api::nudgePitch_end();
}
} // namespace geena::ui