#pragma once


#include <FL/Fl_Window.H>


namespace geena::ui
{
class MainWindow : public Fl_Window
{
public:
	
	MainWindow();

	int handle(int event) override;

	int run();
};
} // geena::ui::