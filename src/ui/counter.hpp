#pragma once

#include "deps/mcl-fl_flex/src/fl_flex.hpp"
#include "types.hpp"
#include <FL/Fl_Box.H>
#include <FL/Fl_Widget.H>

namespace geena::core
{
struct CurrentState;
}

namespace geena::ui
{
class Counter : public mcl::Fl_Flex
{
public:
	Counter(int x, int y, int w, int h, core::CurrentState&);

	void draw() override;
	int  handle(int event) override;

	void refresh(core::CurrentState&);

private:
	void onFileDrop(const char* s) const;

	core::CurrentState& m_state;

	Fl_Box* m_title;
	Fl_Box* m_time;
	Fl_Box* m_pitch;
};
} // namespace geena::ui