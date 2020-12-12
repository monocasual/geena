#include "state.hpp"


namespace geena::engine
{
State::State()
: status  (Status::OFF)
, position(0)
, pitch   (0.0f)
{
}
} // geena::engine::