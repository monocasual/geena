#pragma once


#include "types.hpp"


namespace geena::engine::api
{
void play();
void stop();
void playPauseToggle();
void rewind();
bool loadAudioFile(std::string path);
void setPitch(PitchDir dir);
void nudgePitch_begin(PitchDir dir);
void nudgePitch_end();

void refreshMainState();
const State& getState();
} // geena::engine::api::