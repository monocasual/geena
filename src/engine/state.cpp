#include <cassert>
#include "state.hpp"


namespace geena::engine
{
State::State()
: status   (Status::STOP)
, position (0)
, pitch    (1.0f)
{
}
} // geena::engine::