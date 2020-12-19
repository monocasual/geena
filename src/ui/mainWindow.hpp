#pragma once


#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>


namespace geena::ui
{
class MainWindow : public Fl_Window
{
public:
	
	MainWindow();

	int handle(int event) override;

	int run();

private:

	Fl_Button m_btn_playPause;
	Fl_Button m_btn_rewind;
};
} // geena::ui::