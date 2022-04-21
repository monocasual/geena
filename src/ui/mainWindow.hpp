#pragma once

#include "core/state.hpp"
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>

namespace geena::ui
{
class Counter;
class Progress;
class PitchSlider;
class MainWindow : public Fl_Window
{
public:
	MainWindow(int x, int y, int w, int h);

	int handle(int event) override;

	int  run();
	void refresh();

private:
	core::CurrentState m_state;

	Fl_Button*   m_btn_playPause;
	Fl_Button*   m_btn_rewind;
	Fl_Button*   m_btn_unload;
	Counter*     m_counter;
	Progress*    m_progress;
	PitchSlider* m_pitchSlider;
};
} // namespace geena::ui