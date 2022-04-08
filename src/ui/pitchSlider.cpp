#include "pitchSlider.hpp"

namespace geena::ui
{
PitchSlider::PitchSlider(int x, int y, int w, int h)
: Fl_Slider(x, y, w, h)
{
	type(FL_VERTICAL);
	value(0.5);
}
} // namespace geena::ui