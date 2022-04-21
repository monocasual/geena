#include "ui/pitchSlider.hpp"
#include "const.hpp"

namespace geena::ui
{
PitchSlider::PitchSlider(int x, int y, int w, int h)
: Fl_Slider(x, y, w, h)
{
	type(FL_VERTICAL);
	minimum(G_MAX_PITCH);
	maximum(G_MIN_PITCH);
	slider_size(0.14);
	value(1.0);
}
} // namespace geena::ui