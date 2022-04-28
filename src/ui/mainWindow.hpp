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
	void onKeyDown(int key);
	void onKeyUp(int key);

	core::CurrentState m_state;
	bool               m_keyPressed;

	Fl_Button*   m_btn_playPause;
	Fl_Button*   m_btn_cue;
	Fl_Button*   m_btn_rewind;
	Fl_Button*   m_btn_unload;
	Fl_Button*   m_btn_nudgeUp;
	Fl_Button*   m_btn_nudgeDown;
	Counter*     m_counter;
	Progress*    m_progress;
	PitchSlider* m_pitchSlider;
	Fl_Button*   m_btn_resetPitch;
};
} // namespace geena::ui