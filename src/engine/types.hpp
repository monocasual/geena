#pragma once


#include <memory>
#include <string>


namespace geena::engine
{
using Frame = int;

enum class Status { STOP, PLAY, PAUSE };
enum class PitchDir { UP, DOWN };

} // geena::engine::