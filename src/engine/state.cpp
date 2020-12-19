#include <cassert>
#include "state.hpp"


namespace geena::engine
{
State::State()
: status   (ReadStatus::STOP)
, position (0)
, pitch    (1.0f)
{
}
} // geena::engine::