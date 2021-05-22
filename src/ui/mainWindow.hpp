#pragma once

#include "counter.hpp"
#include "engine/state.hpp"
#include "progress.hpp"
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>

namespace geena::ui
{
class MainWindow : public Fl_Window
{
public:
	MainWindow(int x, int y, int w, int h);

	int handle(int event) override;

	int  run();
	void refresh();

private:
	Fl_Button m_btn_playPause;
	Fl_Button m_btn_rewind;
	Fl_Button m_btn_unload;
	Counter   m_counter;
	Progress  m_progress;
};
} // namespace geena::ui