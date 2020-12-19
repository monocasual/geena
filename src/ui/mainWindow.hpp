#pragma once


#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include "engine/state.hpp"
#include "counter.hpp"


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
	Counter   m_counter;
};
} // geena::ui::