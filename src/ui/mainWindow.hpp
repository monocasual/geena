#pragma once


#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include "engine/state.hpp"
#include "counter.hpp"
#include "progress.hpp"


namespace geena::ui
{
class MainWindow : public Fl_Window
{
public:
	
	MainWindow(int x, int y, int w, int h);

	int handle(int event) override;

	int run();
	void refresh();

private:

	Fl_Button m_btn_playPause;
	Fl_Button m_btn_rewind;
	Fl_Button m_btn_unload;
	Counter   m_counter;
	Progress  m_progress;
};
} // geena::ui::