#pragma once

#include "core/state.hpp"
#include "deps/atomic-swapper/src/atomic-swapper.hpp"

namespace geena::core
{
class Engine
{
public:
	Engine();

	mcl::AtomicSwapper<Layout> layout;

private:
	Shared m_shared;
};
} // namespace geena::core