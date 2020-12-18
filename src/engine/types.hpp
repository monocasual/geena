#pragma once


#include <memory>
#include <string>


namespace geena::engine
{
using Frame = int;

enum class Status { OFF, PLAY, PAUSE };
enum class PitchDir { UP, DOWN };

} // geena::engine::