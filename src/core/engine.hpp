#pragma once

#include "core/kernel.hpp"
#include "core/rendering.hpp"
#include "core/state.hpp"
#include "deps/atomic-swapper/src/atomic-swapper.hpp"

namespace geena::core
{
class Engine
{
public:
	Engine();

	core::Renderer             renderer;
	core::Kernel               kernel;
	mcl::AtomicSwapper<Layout> layout;

private:
	Shared m_shared;
};
} // namespace geena::core