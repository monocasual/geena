#pragma once

#include "types.hpp"
#include <FL/Fl_Widget.H>

namespace geena::core
{
struct CurrentState;
}

namespace geena::ui
{
class Counter : public Fl_Widget
{
public:
	Counter(int x, int y, int w, int h, core::CurrentState&);

	void draw() override;
	int  handle(int event) override;

	void refresh(core::CurrentState&);

private:
	void onFileDrop(const char* s) const;

	core::CurrentState& m_state;
};
} // namespace geena::ui