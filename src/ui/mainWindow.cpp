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
}

MainWindow::MainWindow()
: Fl_Window(640, 480)
{
	show();
}


int MainWindow::handle(int event)
{
	switch(event)
	{
		case FL_FOCUS:
		case FL_UNFOCUS: 
		{
			return 1;   // Enables receiving Keyboard events
		}
		case FL_KEYDOWN:
		{
			if (keyPressed_)
				break;
			if (Fl::event_key() == FL_Up)
				engine::api::setPitch(engine::PitchDir::UP);
			else
			if (Fl::event_key() == FL_Down)
				engine::api::setPitch(engine::PitchDir::DOWN);
			else
			if (Fl::event_key() == FL_Left)
				engine::api::nudgePitch_begin(engine::PitchDir::DOWN);
			else
			if (Fl::event_key() == FL_Right)
				engine::api::nudgePitch_begin(engine::PitchDir::UP);
			keyPressed_ = true;
			return 1;
		}
		case FL_KEYUP:
		{
			if (Fl::event_key() == FL_Left || Fl::event_key() == FL_Right)
				engine::api::nudgePitch_end();
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
			std::vector<std::string> paths = utils::string::split(Fl::event_text(), "\n");
			std::string path = utils::fs::uriToPath(paths[0]);
			engine::api::loadAudioFile(path);
			engine::api::play();
			return 1;
		}
		default:
			break;
	}

	return Fl_Window::handle(event);
}


int MainWindow::run()
{
	return Fl::run();
}
} // geena::ui::