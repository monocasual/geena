#include "mainWindow.hpp"


#include <iostream>
#include <filesystem>
#include <FL/Fl.H>
#include "const.hpp"
#include "types.hpp"
#include "utils/string.hpp"
#include "utils/fs.hpp"
#include "utils/log.hpp"
#include "engine/api.hpp"


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
		engine::api::setPitch(PitchDir::UP);
	else
	if (key == FL_Down)
		engine::api::setPitch(PitchDir::DOWN);
	else
	if (key == FL_Left)
		engine::api::nudgePitch_begin(PitchDir::DOWN);
	else
	if (key == FL_Right)
		engine::api::nudgePitch_begin(PitchDir::UP);
}


/* -------------------------------------------------------------------------- */


void onKeyUp_(int key)
{
	if (key == FL_Left || key == FL_Right)
		engine::api::nudgePitch_end();
}


/* -------------------------------------------------------------------------- */


void onFileDrop_(const char* s)
{
	std::vector<std::string> paths = utils::string::split(s, "\n");
	std::string path = utils::fs::uriToPath(paths[0]);
	engine::api::loadAudioFile(path);	
}
} // {anonymous}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


MainWindow::MainWindow(int x, int y, int w, int h)
: Fl_Window(x, y, w, h)
, m_btn_playPause(8,   216, 100, 100, "Play/Pause")
, m_btn_rewind   (118, 216, 100, 100, "Rewind")
, m_btn_unload   (228, 216, 100, 100, "Unload")
, m_counter      (8,   8, w - 16, 200)
{
	end();

	Fl::add_timeout(G_UI_REFRESH_RATE, refresh_, static_cast<void*>(this));

	m_btn_playPause.callback([] (Fl_Widget* /*w*/, void* /*v*/) 
	{ 
		engine::api::playPauseToggle(); 
	});

	m_btn_rewind.callback([] (Fl_Widget* /*w*/, void* /*v*/) 
	{ 
		engine::api::rewind(); 
	});

	m_btn_unload.callback([] (Fl_Widget* /*w*/, void* /*v*/) 
	{ 
		engine::api::unloadAudioFile(); 
	});
	show();
}


/* -------------------------------------------------------------------------- */


int MainWindow::handle(int event)
{
	switch(event)
	{
		case FL_FOCUS:
		case FL_UNFOCUS: 
		{
			return 1;   // Enables receiving keyboard events
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
			return 1;   // enable dnd
		}
		case FL_PASTE:  // drop (paste) operation
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
	m_counter.refresh(engine::api::getCurrentPosition(), engine::api::getAudioFileLength());
}


/* -------------------------------------------------------------------------- */


int MainWindow::run()
{
	return Fl::run();
}
} // geena::ui::