#include "engine.hpp"

namespace geena::core
{
Engine::Engine()
{
	layout.get().state = &state;
	layout.swap();
}
} // namespace geena::core