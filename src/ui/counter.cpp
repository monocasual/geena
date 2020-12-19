#include <string> 
#include <FL/fl_draw.H> 
#include "counter.hpp"


namespace geena::ui
{
Counter::Counter(int x, int y, int w, int h)
: Fl_Widget (x, y, w, h)
, m_position(0)
, m_length  (0)
{

}


/* -------------------------------------------------------------------------- */


void Counter::draw()
{
    float percent = (m_position / (float) m_length) * 100;

    std::string s = std::to_string(m_position) + " / " + std::to_string(m_length) 
                  + " - " + std::to_string(percent) + " %";

    fl_rectf(x(), y(), w(), h(), fl_rgb_color(255, 255, 255));
    fl_rect(x(), y(), w(), h(), fl_rgb_color(0, 0, 0));

    fl_color(fl_rgb_color(0, 0, 0));
    fl_draw(s.c_str(), x(), y(), w(), h(), FL_ALIGN_CENTER);
}


/* -------------------------------------------------------------------------- */


void Counter::refresh(Frame position, Frame length)
{
    m_position = position;
    m_length   = length;
    
    redraw();
}
} // geena::ui::