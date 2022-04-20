#pragma once

#include "core/state.hpp"
#include "deps/atomic-swapper/src/atomic-swapper.hpp"

namespace geena::core
{
class Engine
{
public:
	Engine();

	State                      state;
	Data                       data;
	mcl::AtomicSwapper<Layout> layout;
};
} // namespace geena::core