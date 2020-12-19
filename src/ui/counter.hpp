#pragma once


#include <FL/Fl_Widget.H>
#include "engine/state.hpp"
#include "types.hpp"


namespace geena::ui
{
class Counter : public Fl_Widget
{
public:
	
	Counter(int x, int y, int w, int h);
    
    void draw() override;

    void refresh(Frame position, Frame length);

private:

    Frame m_position;
    Frame m_length;
};
} // geena::ui::