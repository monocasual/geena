#pragma once

#include "types.hpp"
#include <FL/Fl_Slider.H>

namespace geena::ui
{
class PitchSlider : public Fl_Slider
{
public:
	PitchSlider(int x, int y, int w, int h);

private:
};
} // namespace geena::ui