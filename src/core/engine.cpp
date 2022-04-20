#include "engine.hpp"

namespace geena::core
{
Engine::Engine()
{
	layout.get().shared = &m_shared;
	layout.swap();
}
} // namespace geena::core