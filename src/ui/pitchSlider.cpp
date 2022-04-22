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
	step(G_PITCH_DELTA);
	slider_size(0.14);
	value(G_DEFAULT_PITCH);
}
} // namespace geena::ui