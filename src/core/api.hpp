#pragma once

#include "core/state.hpp"
#include "types.hpp"
#include <string>

namespace geena::core::api
{
void play();
void stop();
void playPauseToggle();
void rewind();
bool loadAudioFile(std::string path);
void unloadAudioFile();
void setPitch(float v);
void setPitch(PitchDir dir);
void nudgePitch_begin(PitchDir dir);
void nudgePitch_end();
void goToFrame(Frame f);

CurrentState getCurrentState();
} // namespace geena::core::api