#pragma once


#include "types.hpp"


namespace geena::engine::api
{
void startRendering();
void stopRendering();

bool loadAudioFile(std::string path);

void setPitch(PitchDir dir);
void nudgePitch_begin(PitchDir dir);
void nudgePitch_end();
} // geena::engine::api::