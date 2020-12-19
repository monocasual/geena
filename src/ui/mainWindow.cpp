#include "mainWindow.hpp"


#include <iostream>
#include <filesystem>
#include <FL/Fl.H>
#include "utils/string.hpp"
#include "utils/fs.hpp"
#include "engine/api.hpp"
#include "engine/types.hpp"


namespace geena::ui
{
namespace
{
bool keyPressed_ = false;


/* -------------------------------------------------------------------------- */


void onKeyDown_(int key)
{
	if (key == FL_Up)
		engine::api::setPitch(engine::PitchDir::UP);
	else
	if (key == FL_Down)
		engine::api::setPitch(engine::PitchDir::DOWN);
	else
	if (key == FL_Left)
		engine::api::nudgePitch_begin(engine::PitchDir::DOWN);
	else
	if (key == FL_Right)
		engine::api::nudgePitch_begin(engine::PitchDir::UP);
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


MainWindow::MainWindow()
: Fl_Window(640, 480)
, m_btn_playPause(8,   8, 100, 100, "Play/Pause")
, m_btn_rewind   (118, 8, 100, 100, "Rewind")
{
	end();

	m_btn_playPause.callback([] (Fl_Widget* /*w*/, void* /*v*/) 
	{ 
		engine::api::playPauseToggle(); 
	});

	m_btn_rewind.callback([] (Fl_Widget* /*w*/, void* /*v*/) 
	{ 
		engine::api::rewind(); 
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


int MainWindow::run()
{
	return Fl::run();
}
} // geena::ui::