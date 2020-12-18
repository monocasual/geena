#pragma once


#include "types.hpp"


namespace geena::engine::api
{
void setPitch(PitchDir dir);
void nudgePitch_begin(PitchDir dir);
void nudgePitch_end();
} // geena::engine::api::